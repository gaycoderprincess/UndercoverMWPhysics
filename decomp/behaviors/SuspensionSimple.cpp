void* NewSuspensionSimpleMWVTable[] = {
		(void*)&SuspensionSimpleMW::OnService,
		(void*)&SuspensionSimpleMW::Destroy,
		(void*)&SuspensionSimpleMW::Reset,
		(void*)&SuspensionSimpleMW::GetPriority,
		(void*)&SuspensionSimpleMW::OnOwnerAttached,
		(void*)&SuspensionSimpleMW::OnOwnerDetached,
		(void*)&SuspensionSimpleMW::OnTaskSimulate,
		(void*)&SuspensionSimpleMW::OnBehaviorChange,
		(void*)&SuspensionSimpleMW::OnPause,
		(void*)&SuspensionSimpleMW::OnUnPause,
		(void*)&SuspensionSimpleMW::OnDebugDraw,
		(void*)&SuspensionSimpleMW::CalculateUndersteerFactor,
		(void*)&SuspensionSimpleMW::CalculateOversteerFactor,
		(void*)&SuspensionSimpleMW::GetDownCoefficient,
		(void*)&SuspensionSimpleMW::GetDriftValue,
		(void*)&SuspensionSimpleMW::ApplyVehicleEntryForces,
};

void SuspensionSimpleMW::CreateTires() {
	for (int i = 0; i < 4; ++i) {
		bool is_front = IsFront(i);
		float diameter = Physics::Info::WheelDiameter(mAttributes, is_front);
		mTires[i] = new Tire(diameter * 0.5f, i, mMWAttributes);
	}

	UMath::Vector3 dimension;
	mRB->GetDimension(&dimension);

	float wheelbase = mAttributes.GetLayout()->WHEEL_BASE;
	float axle_width_f = mAttributes.GetLayout()->TRACK_WIDTH.At(0) - mAttributes.GetLayout()->SECTION_WIDTH.At(0) * 0.001f;
	float axle_width_r = mAttributes.GetLayout()->TRACK_WIDTH.At(1) - mAttributes.GetLayout()->SECTION_WIDTH.At(1) * 0.001f;
	float front_axle = mAttributes.GetLayout()->FRONT_AXLE;

	UMath::Vector3 fl(-axle_width_f * 0.5f, -dimension.y, front_axle);
	UMath::Vector3 fr(axle_width_f * 0.5f, -dimension.y, front_axle);
	UMath::Vector3 rl(-axle_width_r * 0.5f, -dimension.y, front_axle - wheelbase);
	UMath::Vector3 rr(axle_width_r * 0.5f, -dimension.y, front_axle - wheelbase);

	GetWheel(0).SetLocalArm(fl);
	GetWheel(1).SetLocalArm(fr);
	GetWheel(2).SetLocalArm(rl);
	GetWheel(3).SetLocalArm(rr);
}

void SuspensionSimpleMW::Create(const BehaviorParams &bp) {
	SUSPENSIONSIMPLE_FUNCTION_LOG("Create");

	ChassisMW::Create(bp);

	mChassisType = "SuspensionSimple";

	*(uintptr_t*)this = (uintptr_t)&NewSuspensionSimpleMWVTable;

	GetOwner()->QueryInterface(&mInput);
	//GetOwner()->QueryInterface(&mCheater);
	mFrictionBoost = 0.0f;
	mDraft = 0.0f;
	mPowerSliding = false;
	mWheelSteer[0] = 0.0f;
	mWheelSteer[1] = 0.0f;
	mYawControlMultiplier = 0.0f;
	mNumWheelsOnGround = 0;
	mAgainstWall = 0.0f;
	mMaxSteering = 0.0f;
	mTimeInAir = 0.0f;
	mSleepTime = 0.0f;
	mDriftPhysics = false;

	tmpCollisionListener = TempCollisionListener();
	tmpCollisionListener.vtable = &tmpCollisionListener.vt_OnCollision;
	Sim::Collision::AddListener((Sim::Collision::IListener*)&tmpCollisionListener, GetOwner(), "SuspensionSimpleMW");

	CreateTires();
}

void SuspensionSimpleMW::Destroy(char a2) {
	SUSPENSIONSIMPLE_FUNCTION_LOG("Destroy");

	Sim::Collision::RemoveListener((Sim::Collision::IListener*)&tmpCollisionListener);

	for (int i = 0; i < 4; ++i) {
		WriteLog("delete mTires[i]");
		delete mTires[i];
	}

	WriteLog("SuspensionSimpleMW::Destroy finished");

	ChassisMW::Destroy(a2);

	if ((a2 & 1) != 0) {
		WriteLog("gFastMem.Free");
		gFastMem.Free(this, sizeof(SuspensionSimpleMW), nullptr);
	}
}

SuspensionSimpleMW::Tire::Tire(float radius, int index, MWCarTuning* mwSpecs) : WheelMW(1) {
	mRadius = UMath::Max(radius, 0.1f);
	mBrake = 0.0f;
	mEBrake = 0.0f;
	mAV = 0.0f;
	mLoad = 0.0f;
	mLateralForce = 0.0f;
	mLongitudeForce = 0.0f;
	mAppliedTorque = 0.0f;
	mTractionBoost = 1.0f;
	mSlip = 0.0f;
	mLateralSpeed = 0.0f;
	mWheelIndex = index;
	mRoadSpeed = 0.0f;
	mSlipAngle = 0.0f;
	mAxleIndex = index >> 1;
	mTraction = 1.0;
	mBrakeLocked = false;
	mAllowSlip = true;
	mLastTorque = 0.0f;
	mAngularAcc = 0.0f;
	mMaxSlip = 0.5f;
	mGripBoost = 1.0f;
	mMWSpecs = mwSpecs;
}

void SuspensionSimpleMW::Tire::BeginFrame(float max_slip, float grip_scale, float traction_boost) {
	mMaxSlip = max_slip;
	mAppliedTorque = 0.0f;
	SetForce({0,0,0});
	mLateralForce = 0.0f;
	mLongitudeForce = 0.0f;
	mTractionBoost = traction_boost;
	mGripBoost = grip_scale;
	mAllowSlip = false;
}

void SuspensionSimpleMW::Tire::EndFrame(float dT) {}

extern float WheelMomentOfInertia;

void SuspensionSimpleMW::Tire::UpdateFree(float dT) {
	mTraction = 0.0f;
	mLoad = 0.0f;
	mSlip = 0.0f;
	mSlipAngle = 0.0f;

	mBrakeLocked = mEBrake > 0.0f && mAxleIndex == 1;
	if (mBrakeLocked) {
		mAV = 0.0f;
	} else {
		float angularacc = mAppliedTorque / WheelMomentOfInertia * dT;
		mAV += angularacc;
	}
	mLateralForce = 0.0f;
	mLongitudeForce = 0.0f;
	mLastTorque = 0.0f;
	mAngularAcc = 0.0f;
}

void SuspensionSimpleMW::MatchSpeed(float speed) {
	for (int i = 0; i < 4; ++i) {
		mTires[i]->MatchSpeed(speed);
	}
}

void SuspensionSimpleMW::DoAerobatics(State &state) {
	if (state.flags & 2) {
		return;
	}
	DoJumpStabilizer(state);
}

UMath::Vector3* SuspensionSimpleMW::GetWheelCenterPos(UMath::Vector3* result, unsigned int i) {
	*result = mTires[i]->GetPosition();
	if (!mRBComplex) {
		return result;
	}
	UMath::ScaleAdd(*mRB->GetUpVector(), GetIChassis()->GetWheelRadius(i), *result, *result);
	return result;
}

void SuspensionSimpleMW::Reset() {
	ISimable *owner = GetOwner();
	unsigned int numonground = 0;
	for (unsigned int i = 0; i < 4; ++i) {
		Tire &wheel = GetWheel(i);
		wheel.Reset();

		if (wheel.InitPosition(mRBComplex, mRB, wheel.GetRadius())) {
			float newCompression = wheel.GetNormal().w + GetRideHeight(i);
			if (newCompression <= FLOAT_EPSILON) {
				newCompression = 0.0f;
			}
			wheel.SetCompression(newCompression);
			if (newCompression > 0.0f) {
				numonground++;
			}
		}
	};
	mNumWheelsOnGround = numonground;
	mAgainstWall = 0.0f;
	mTimeInAir = 0.0f;
	mSleepTime = 0.0f;
}

void SuspensionSimpleMW::OnCollision(const Sim::Collision::Info &cinfo) {
	auto a2 = (uintptr_t)&cinfo;
	uint32_t cinfoType = (*(uint32_t*)(a2 + 0x1C) & 7);
	if (cinfoType != Sim::Collision::Info::WORLD) {
		return;
	}
	if ((UMath::Abs(cinfo.normal.y) < 0.1f) && (mAgainstWall == 0.0f) && mRBComplex) {
		if (UMath::Length(cinfo.closingVel) < 7.5f) {
			const UMath::Vector3 &vFoward = *mRB->GetForwardVector();
			const UMath::Vector3 &vRight = *mRB->GetRightVector();
			float dirdot = UMath::Dot(cinfo.normal, vFoward);
			if (dirdot <= 0.0f) {
				UMath::Vector3 rpos;
				UMath::Sub(cinfo.position, *mRB->GetPosition(), rpos);
				dirdot = UMath::Dot(rpos, vFoward);
				
				UMath::Vector3 dim;
				mRB->GetDimension(&dim);
				if (dirdot > dim.z * 0.75f) {
					float dot = UMath::Dot(cinfo.normal, vRight);
					mAgainstWall = (dot > 0.0f ? -1.0f : 1.0f) - dot;
				}
			}
		}
	}
}

void SuspensionSimpleMW::TempCollisionListener::OnCollision(const Sim::Collision::Info *cinfo) {
	return GetSuspensionRacer()->OnCollision(*cinfo);
}

void SuspensionSimpleMW::DoSteering(ChassisMW::State &state, UMath::Vector3 &right, UMath::Vector3 &left) {
	float steer_coeff = UMath::Max(state.ebrake_input, state.brake_input);
	mMaxSteering = UMath::Lerp(45.0f, 60.0f, UMath::Max(steer_coeff, 1.0f - state.gas_input));
	float steer_input = state.steer_input * mMaxSteering;
	float trueesteer;
	if (state.driver_style != 1) {
		steer_input *= mMWAttributes->STEERING;
	}
	UMath::Vector4 r4;
	UMath::Vector4 l4;
	ComputeAckerman(steer_input * DEG2ANGLE(1.0f), state, &l4, &r4);

	right = UMath::Vector4To3(r4);
	left = UMath::Vector4To3(l4);
	mWheelSteer[0] = l4.w;
	mWheelSteer[1] = r4.w;
	DoWallSteer(state);
}

void SuspensionSimpleMW::DoWallSteer(State &state) {
	float wall = mAgainstWall;
	if ((wall != 0.0f) && (GetIChassis()->GetNumWheelsOnGround() > 2) && (state.gas_input > 0.0f)) {
		float dW = state.steer_input * state.gas_input * 0.125f;
		if (wall * dW < 0.0f) {
			return;
		}
		dW *= UMath::Abs(wall);
		UMath::Vector3 chg = {};
		chg.y = dW;
		UMath::Rotate(chg, state.matrix, chg);
		UMath::Add(state.angular_vel, chg, chg);
		mRB->SetAngularVelocity(&chg);
	}
}

void SuspensionSimpleMW::DoDriveForces(State &state) {
	if (!mTransmission) {
		return;
	}
	float drive_torque = mTransmission->GetDriveTorque();
	if (UMath::Abs(drive_torque) <= FLOAT_EPSILON) {
		return;
	}
	float split = mMWAttributes->TORQUE_SPLIT;
	float front_drive = drive_torque * split;
	bool slip_tires = false;
	bool is_staging = state.flags & 1;
	if (is_staging || (state.gear < 4 && (state.gas_input > 0.5f))) {
		slip_tires = true;
	}
	float traction_boost = state.nos_boost * state.shift_boost;
	if (is_staging) {
		traction_boost *= 0.25f;
	}
	if (front_drive != 0.0f) {
		for (unsigned int i = 0; i < 2; ++i) {
			Tire *tire = mTires[i];
			if (tire->IsOnGround()) {
				tire->ApplyTorque(front_drive * 0.5f);
				tire->AllowSlip(slip_tires);
				tire->ScaleTractionBoost(traction_boost);
			}
		};
	}
	float rear_drive = drive_torque * (1.0f - split);
	if (rear_drive != 0.0f) {
		for (unsigned int i = 2; i < 4; ++i) {
			Tire *tire = mTires[i];
			if (tire->IsOnGround()) {
				tire->ApplyTorque(rear_drive * 0.5f);
				tire->AllowSlip(slip_tires);
				tire->ScaleTractionBoost(traction_boost);
			}
		}
	}
}

// todo this is just SuspensionRacer's ComputeLateralForce
Newtons SuspensionSimpleMW::Tire::ComputeLateralForce(float load, float slip_angle) {
	float angle = ANGLE2DEG(slip_angle);
	float norm_angle = angle * 0.5f;
	int slip_angle_table = (int)norm_angle;
	load *= 0.001f; // convert to kN
	float extra = norm_angle - slip_angle_table;
	load *= LoadFactor;

	if (slip_angle_table > 5) {
		return (mMWSpecs->GRIP_SCALE.At(mAxleIndex) * NewCorneringScale) * mGripBoost * GripFactor * LoadSensitivityTable[6]->GetValue(load);
	} else {
		float low = LoadSensitivityTable[slip_angle_table]->GetValue(load);
		float high = LoadSensitivityTable[slip_angle_table + 1]->GetValue(load);
		return (mMWSpecs->GRIP_SCALE.At(mAxleIndex) * NewCorneringScale) * mGripBoost * GripFactor * (extra * (high - low) + low);
	}
}

// todo this is just SuspensionRacer's CheckForBrakeLock
void SuspensionSimpleMW::Tire::CheckForBrakeLock(float ground_force) {
	const float brake_spec = mMWSpecs->BRAKE_LOCK.At(mAxleIndex) * FTLB2NM(mMWSpecs->BRAKES.At(mAxleIndex)) * BrakingTorque;
	const float ebrake_spec = FTLB2NM(mMWSpecs->EBRAKE) * EBrakingTorque;
	static float StaticToDynamicBrakeForceRatio = 1.2f;
	static float BrakeLockAngularVelocityFactor = 100.0f;

	float bt = mBrake * brake_spec;
	float ebt = mEBrake * ebrake_spec;
	float available_torque = (bt + ebt) * StaticToDynamicBrakeForceRatio;

	if (available_torque > ground_force * GetRadius() + UMath::Abs(mAV) * BrakeLockAngularVelocityFactor) {
		if (available_torque > 1.0f) {
			mBrakeLocked = true;
		} else {
			mBrakeLocked = false;
		}
		mAV = 0.0f;
	} else {
		mBrakeLocked = false;
	}
}

// todo this is just SuspensionRacer's UpdateLoaded
float SuspensionSimpleMW::Tire::UpdateLoaded(float lat_vel, float fwd_vel, float body_speed, float load, float dT) {
	const float brake_spec = FTLB2NM(mMWSpecs->BRAKES.At(mAxleIndex)) * BrakingTorque;
	const float ebrake_spec = FTLB2NM(mMWSpecs->EBRAKE) * EBrakingTorque;
	const float dynamicgrip_spec = mMWSpecs->DYNAMIC_GRIP.At(mAxleIndex);
	const float staticgrip_spec = mMWSpecs->STATIC_GRIP.At(mAxleIndex);
	// free rolling wheel
	if (mLoad <= 0.0f && !mBrakeLocked) {
		mAV = fwd_vel / mRadius;
	}

	float fwd_acc = (fwd_vel - mRoadSpeed) / dT;

	mRoadSpeed = fwd_vel;
	mLoad = UMath::Max(load, 0.0f);
	mLateralSpeed = lat_vel;

	float bt = mBrake * brake_spec;
	float ebt = mEBrake * ebrake_spec;
	float abs_fwd = UMath::Abs(fwd_vel);
	if (abs_fwd < 1.0f) {
		// when car is nearly stopped, apply brake torque using forward velocity and wheel load
		bt = -mBrake * load * fwd_vel / mRadius;
		ebt = -mEBrake * load * fwd_vel / mRadius;

		ApplyTorque(-mAppliedTorque * mEBrake);
		ApplyTorque(-bt);
		ApplyTorque(-ebt);
	} else {
		ApplyTorque(mAV < 0.0f ? -bt : bt);
		ApplyTorque(mAV < 0.0f ? -ebt : ebt);
	}

	mSlipAngle = UMath::Atan2a(lat_vel, abs_fwd);
	float groundfriction = 0.0f;
	float slip_speed = mAV * mRadius - fwd_vel;
	float dynamicfriction = 1.0f;
	mSlip = slip_speed;
	float skid_speed = UMath::Sqrt(slip_speed * slip_speed + lat_vel * lat_vel);
	if (skid_speed > FLOAT_EPSILON && (lat_vel != 0.0f || fwd_vel != 0.0f)) {
		dynamicfriction = dynamicgrip_spec * mTractionBoost;
		groundfriction = mLoad * dynamicfriction / skid_speed;
		float slipgroundfriction = mLoad * dynamicfriction / UMath::Sqrt(fwd_vel * fwd_vel + lat_vel * lat_vel);
		CheckForBrakeLock(abs_fwd * slipgroundfriction);
	}

	if (mTraction < 1.0f || mBrakeLocked) {
		mLongitudeForce = groundfriction;
		mLongitudeForce *= slip_speed;
		mLateralForce = -groundfriction * lat_vel;

		if (body_speed < kOneMPH && dynamicfriction > 0.1f) {
			mLateralForce /= dynamicfriction;
			mLongitudeForce /= dynamicfriction;
		}
		mLongitudeForce = UMath::Limit(mLongitudeForce, mAppliedTorque / mRadius);
	} else {
		mBrakeLocked = false;
		mLongitudeForce = mAppliedTorque / mRadius;
		float slip_ang = mSlipAngle;
		mLateralForce = ComputeLateralForce(mLoad, UMath::Abs(mSlipAngle));
		if (lat_vel > 0.0f) {
			mLateralForce = -mLateralForce;
		}
	}

	if (mTraction >= 1.0f && !mBrakeLocked) {
		float acc_diff = mAngularAcc * mRadius - fwd_acc;
		mLongitudeForce += acc_diff * WheelMomentOfInertia / mRadius;
	}

	bool use_ellipse = false;
	if (mAppliedTorque * fwd_vel > 0.0f && !mBrakeLocked) {
		use_ellipse = true;
		mLongitudeForce *= TireForceEllipseRatio;
	}

	float len_force = UMath::Sqrt(mLateralForce * mLateralForce + mLongitudeForce * mLongitudeForce);
	float max_force = mLoad * staticgrip_spec * mTractionBoost;

	mTraction = 1.0f;
	float max_slip = mMaxSlip;

	if (len_force > max_force && len_force > 0.001f) {
		float ratio = max_force / len_force;
		mTraction = ratio;
		mLateralForce *= ratio;
		mLongitudeForce *= ratio;
		max_slip = (ratio * ratio) * max_slip;
	} else if (use_ellipse) {
		mLongitudeForce *= InvTireForceEllipseRatio;
	}

	if (UMath::Abs(slip_speed) > max_slip) {
		mTraction *= max_slip / UMath::Abs(slip_speed);
	}

	// factor surface friction into the tire force
	// todo this is different in UC
	//mLateralForce *= mSurface.LATERAL_GRIP();
	//mLongitudeForce *= mSurface.DRIVE_GRIP();

	if (fwd_vel > 1.0f) {
		mLongitudeForce -= UMath::Sina(mSlipAngle) * mLateralForce * 0.0 / mMWSpecs->GRIP_SCALE.At(mAxleIndex); // mDragReduction
	} else {
		mLateralForce *= UMath::Min(UMath::Abs(lat_vel), 1.0f);
	}

	if (mBrakeLocked) {
		mAngularAcc = 0.0f;

	} else {
		if (mTraction < 1.0f) {
			float torque = (mAppliedTorque - mLongitudeForce * mRadius + mLastTorque) * 0.5f;
			mLastTorque = torque;
			//float rolling_resistance = RollingFriction * mSurface.ROLLING_RESISTANCE();
			float rolling_resistance = RollingFriction;
			float effective_torque = torque - mAV * rolling_resistance;
			mAngularAcc = (effective_torque / WheelMomentOfInertia) - (mTraction * mSlip) / (mRadius * dT);
		}

		mAngularAcc = UMath::Lerp(mAngularAcc, fwd_acc / mRadius, mTraction);
	}

	mAV += mAngularAcc * dT;
	//CheckSign();
	return mLateralForce;
}

// todo this is just SuspensionRacer's DoWheelForces
void SuspensionSimpleMW::DoWheelForces(ChassisMW::State &state) {
	const float dT = state.time;

	UMath::Vector3 steerR;
	UMath::Vector3 steerL;
	DoSteering(state, steerR, steerL);
	//TuneWheelParams(state);

	unsigned int wheelsOnGround = 0;
	float maxDelta = 0.0f;

	const UMath::Vector3 &vFwd = state.GetForwardVector();
	const UMath::Vector3 &vUp = state.GetUpVector();

	const float mass = state.mass;

	float ride_extra = 0.0f;
	const Physics::Tunings *tunings = GetVehicleMWTunings(GetVehicle());
	if (tunings) {
		ride_extra = tunings->Value[Physics::Tunings::RIDEHEIGHT];
	}

	float time = Sim::GetTime();
	float shock_specs[2];
	float spring_specs[2];
	float sway_specs[2];
	float travel_specs[2];
	float rideheight_specs[2];
	float shock_ext_specs[2];
	float shock_valving[2];
	float shock_digression[2];
	float progression[2];

	for (unsigned int i = 0; i < 2; ++i) {
		shock_specs[i] = LBIN2NM(mMWAttributes->SHOCK_STIFFNESS.At(i));
		shock_ext_specs[i] = LBIN2NM(mMWAttributes->SHOCK_EXT_STIFFNESS.At(i));
		shock_valving[i] = INCH2METERS(mMWAttributes->SHOCK_VALVING.At(i));
		shock_digression[i] = 1.0f - mMWAttributes->SHOCK_DIGRESSION.At(i);
		spring_specs[i] = LBIN2NM(mMWAttributes->SPRING_STIFFNESS.At(i));
		sway_specs[i] = LBIN2NM(mMWAttributes->SWAYBAR_STIFFNESS.At(i));
		travel_specs[i] = INCH2METERS(mMWAttributes->TRAVEL.At(i));
		rideheight_specs[i] = INCH2METERS(mMWAttributes->RIDE_HEIGHT.At(i) + ride_extra);
		progression[i] = mMWAttributes->SPRING_PROGRESSION.At(i);
	}

	float sway_stiffness[4];
	sway_stiffness[0] = (mTires[0]->GetCompression() - mTires[1]->GetCompression()) * sway_specs[0];
	sway_stiffness[1] = -sway_stiffness[0];
	sway_stiffness[2] = (mTires[2]->GetCompression() - mTires[3]->GetCompression()) * sway_specs[1];
	sway_stiffness[3] = -sway_stiffness[2];

	UMath::Vector4 steering_normals[4];
	steering_normals[0] = UMath::Vector4Make(steerL, 1.0f);
	steering_normals[1] = UMath::Vector4Make(steerR, 1.0f);
	steering_normals[2] = UMath::Vector4Make(vFwd, 1.0f);
	steering_normals[3] = UMath::Vector4Make(vFwd, 1.0f);

	bool resolve = false;
	for (unsigned int i = 0; i < 4; ++i) {
		int axle = i / 2;
		Tire &wheel = GetWheel(i);
		UMath::Vector3 wp = wheel.GetWorldArm();
		wheel.UpdatePosition(state.angular_vel, state.linear_vel, state.matrix, state.world_cog, state.time, wheel.GetRadius(), true, state.collider,
							 state.dimension.y * 2.0f);
		const UMath::Vector3 groundNormal(wheel.GetNormal());
		const UMath::Vector3 forwardNormal(steering_normals[i]);
		UMath::Vector3 lateralNormal;
		UMath::UnitCross(groundNormal, forwardNormal, lateralNormal);

		float penetration = wheel.GetNormal().w;
		// how angled the wheel is relative to the ground
		float upness = UMath::Clamp(UMath::Dot(groundNormal, vUp), 0.0f, 1.0f);
		const float oldCompression = wheel.GetCompression();
		float newCompression = rideheight_specs[axle] * upness + penetration;
		float max_compression = travel_specs[axle];
		if (wheel.GetCompression() == 0.0f) {
			maxDelta = UMath::Max(maxDelta, newCompression - max_compression);
		}
		newCompression = UMath::Max(newCompression, 0.0f);

		// handle the suspension bottoming out
		if (newCompression > max_compression) {
			float delta = newCompression - max_compression;
			maxDelta = UMath::Max(maxDelta, delta);
			// spring can't compress past the travel length
			newCompression = max_compression;
			//wheel.SetBottomOutTime(time);
		}

		if (newCompression > 0.0f && upness > VehicleSystem::ENABLE_ROLL_STOPS_THRESHOLD) {
			++wheelsOnGround;

			const float diff = newCompression - wheel.GetCompression();
			float rise = diff / dT;

			float spring = (newCompression * spring_specs[axle]) * (newCompression * progression[axle] + 1.0f);
			if (shock_valving[axle] > FLOAT_EPSILON && shock_digression[axle] < 1.0f) {
				float abs_rise = UMath::Abs(rise);
				float valving = shock_valving[axle];
				if (abs_rise > valving) {
					float digression = valving * UMath::Pow(abs_rise / valving, shock_digression[axle]);
					rise = (rise > 0.0f) ? digression : -digression;
				}
			}

			float damp = rise > 0.0f ? rise * shock_specs[axle] : rise * shock_ext_specs[axle];

			if (damp > mMWAttributes->SHOCK_BLOWOUT * 9.81f * mass) {
				damp = 0.0f;
			}

			float springForce = UMath::Max(damp + spring + sway_stiffness[i], 0.0f);

			UMath::Vector3 verticalForce(vUp * springForce);

			UMath::Vector3 driveForce;
			UMath::Vector3 lateralForce;
			UMath::Vector3 c;
			UMath::Cross(forwardNormal, groundNormal, c);
			UMath::Cross(c, forwardNormal, c);

			float d2 = UMath::Dot(c, groundNormal) * 4.0f - 3.0f;
			float load = UMath::Max(d2, 0.3f) * springForce;
			const UMath::Vector3 &pointVelocity = wheel.GetVelocity();
			float xspeed = UMath::Dot(pointVelocity, lateralNormal);
			float zspeed = UMath::Dot(pointVelocity, forwardNormal);

			float traction_force = wheel.UpdateLoaded(xspeed, zspeed, state.speed, load, state.time);
			float max_traction = UMath::Abs(xspeed / dT * (0.25f * mass));
			lateralForce = lateralNormal * UMath::Clamp(traction_force, -max_traction, max_traction);

			UMath::Vector3 force;
			UMath::UnitCross(lateralNormal, groundNormal, driveForce);
			UMath::Scale(driveForce, wheel.mLongitudeForce, driveForce);
			UMath::Add(lateralForce, driveForce, force);
			UMath::Add(force, verticalForce, force);

			wheel.SetForce(force);
			resolve = true;
		} else {
			wheel.SetForce({});
			wheel.UpdateFree(dT);
		}

		if (newCompression == 0.0f) {
			wheel.IncAirTime(dT);
		} else {
			wheel.SetAirTime(0.0f);
		}
		wheel.SetCompression(newCompression);
	}

	if (resolve) {
		UMath::Vector3 total_torque = {};
		UMath::Vector3 total_force = {};

		for (unsigned int i = 0; i < GetIChassis()->GetNumWheels(); ++i) {
			Tire &wheel = GetWheel(i);
			UMath::Vector3 p(wheel.GetLocalArm());
			p.y += wheel.GetCompression();
			p.y -= rideheight_specs[i / 2u];
			const UMath::Vector3 &force = wheel.GetForce();
			UMath::RotateTranslate(p, state.matrix, p);
			wheel.SetPosition(p);

			UMath::Vector3 torque;
			UMath::Vector3 r;
			UMath::Sub(p, state.world_cog, r);
			UMath::Sub(r, state.GetPosition(), r);
			UMath::Cross(r, force, torque);
			UMath::Add(total_force, force, total_force);
			UMath::Add(total_torque, torque, total_torque);
		}

		float yaw = UMath::Dot(UMath::Vector4To3(state.matrix.y), total_torque);
		float counter_yaw = yaw * mMWAttributes->YAW_SPEED;
		UMath::ScaleAdd(UMath::Vector4To3(state.matrix.y), counter_yaw - yaw, total_torque, total_torque);
		mRB->Resolve(&total_force, &total_torque);
	}

	if (maxDelta > 0.0f) {
		for (int i = 0; i < GetIChassis()->GetNumWheels(); ++i) {
			auto& wheel = GetWheel(i);
			wheel.SetY(wheel.GetPosition().y + maxDelta);
		}
		mRB->ModifyYPos(maxDelta);
	}

	if (wheelsOnGround != 0 && !mNumWheelsOnGround) {
		state.local_angular_vel.z *= 0.5f;
		state.local_angular_vel.y *= 0.5f;
		UMath::Rotate(state.local_angular_vel, state.matrix, state.angular_vel);
		mRB->SetAngularVelocity(&state.angular_vel);
	}
	mNumWheelsOnGround = wheelsOnGround;
}

// this wasn't in the decomp, might be wrong
void SuspensionSimpleMW::OnTaskSimulate(float dT) {
	if (!mRBComplex || !mRB) {
		return;
	}

	float ride_extra = 0.0;
	if (mNumWheelsOnGround < 2) {
		if (!mNumWheelsOnGround) {
			ride_extra = -10.0;
		}
	}
	else {
		ride_extra = 2.0;
	}
	SetCOG(0.0, ride_extra);

	State state;
	ComputeState(dT, state);

	if (mSleepTime > 3.0 && DoSleep(state) == SS_ALL) return;

	float max_slip = ComputeMaxSlip(state);
	float grip_scale = ComputeLateralGripScale(state);
	float traction_scale = ComputeTractionScale(state);
	for (unsigned int i = 0; i < 4; ++i) {
		mTires[i]->BeginFrame(max_slip, grip_scale, traction_scale);
	}

	float drag_pct = 1.0f;
	float aero_pct = 1.0f;
	if (mCheater) {
		drag_pct = ((mCheater->GetCatchupCheat() * -1.0) + 1.0);
		aero_pct = ((mCheater->GetCatchupCheat() * 0.5) + 1.0);
	}
	DoAerodynamics(state, drag_pct, aero_pct, GetWheel(0).GetLocalArm().z, GetWheel(2).GetLocalArm().z, nullptr);
	DoDriveForces(state);
	DoWheelForces(state);

	for (unsigned int i = 0; i < 4; ++i) {
		mTires[i]->UpdateTime(dT);
	}

	for (unsigned int i = 0; i < 4; ++i) {
		mTires[i]->EndFrame(dT);
	}

	mAgainstWall = 0.0;
	if (mNumWheelsOnGround > 0) {
		mTimeInAir = 0.0;
	}
	else {
		mTimeInAir += dT;
	}

	DoAerobatics(state);
	DoSleep(state);
	ChassisMW::OnTaskSimulate(dT);
}