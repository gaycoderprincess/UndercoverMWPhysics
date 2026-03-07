float UNDERCOVER_BrakesAtValue = 0.0;
float UNDERCOVER_StaticGripAtValue = 0.0;
float UNDERCOVER_RollCenterAtValue = 0.0;
float UNDERCOVER_AeroCGAtValue = 0.0;

const float ZeroDegreeTable[6] = {0.0f};
float TwoDegreeTable[] = {0.0f, 1.2f, 2.3f, 3.0f, 3.0f, 2.8f};
float FourDegreeTable[] = {0.0f, 1.7f, 3.2f, 4.3f, 5.1f, 5.2f};
float SixDegreeTable[] = {0.0f, 1.8f, 3.5f, 4.9f, 5.8f, 6.1f};
float EightDegreeTable[] = {0.0f, 1.83f, 3.6f, 5.0f, 5.96f, 6.4f};
float TenDegreeTable[] = {0.0f, 1.86f, 3.7f, 5.1f, 6.13f, 6.7f};
float TwelveDegreeTable[] = {0.0f, 1.9f, 3.8f, 5.2f, 6.3f, 7.1f};

Table ZeroDegree = Table(ZeroDegreeTable, 6, 0.0f, 10.0f);
Table TwoDegree = Table(TwoDegreeTable, 6, 0.0f, 10.0f);
Table FourDegree = Table(FourDegreeTable, 6, 0.0f, 10.0f);
Table SixDegree = Table(SixDegreeTable, 6, 0.0f, 10.0f);
Table EightDegree = Table(EightDegreeTable, 6, 0.0f, 10.0f);
Table TenDegree = Table(TenDegreeTable, 6, 0.0f, 10.0f);
Table TwelveDegree = Table(TwelveDegreeTable, 6, 0.0f, 10.0f);

#ifdef TARGET_WIN32
// DRM protected variable
int *pLatForceMultipliers = NULL;
#endif
Table *LoadSensitivityTable[] = {&ZeroDegree, &TwoDegree, &FourDegree, &SixDegree, &EightDegree, &TenDegree, &TwelveDegree};
static const float NewCorneringScale = 1000.0f;
static const float LoadFactor = 0.8f;
static const float GripFactor = 2.5f;

static const float PostCollisionSteerReductionDuration = 1.0f;
bVector2 PostCollisionSteerReductionData[] = {bVector2(0.0f, 0.2f), bVector2(0.2f, 0.5f), bVector2(0.5f, 0.7f), bVector2(0.7f, 1.0f)};
Graph PostCollisionSteerReductionTable(PostCollisionSteerReductionData, 4);
static const float Tweak_CollisionImpulseSteerMax = 40.0f;
static const float Tweak_CollisionImpulseSteerMin = 10.0f;
static const float Tweak_WallSteerClosingSpeed = 56.25f;
static const float Tweak_WallSteerBodySpeed = 6.25f;

float BrakeSteeringRangeMultiplier = 1.45f;
float CounterSteerOn = MPH2MPS(30.f);
float CounterSteerOff;
float MAX_STEERING = 45.0f;

float SteeringRangeData[] = {40.0f, 20.0f, 10.0f, 5.5f, 4.5f, 3.25f, 2.9f, 2.9f, 2.9f, 2.9f};
float SteeringSpeedData[] = {1.0f, 1.0f, 1.0f, 0.56f, 0.5f, 0.35f, 0.3f, 0.3f, 0.3f, 0.3f};
float SteeringWheelRangeData[] = {45.0f, 15.0f, 11.0f, 8.0f, 7.0f, 7.0f, 7.0f, 7.0f, 7.0f, 7.0f};
float SteeringInputSpeedData[] = {1.0f, 1.05f, 1.1f, 1.5f, 2.2f, 3.1f};
float SteeringInputData[] = {1.0f, 1.05f, 1.1f, 1.2f, 1.3f, 1.4f};

// only the first steering remap table is actually used, the rest are HP2/UG1/UG2 leftovers

static float JoystickInputToSteerRemap1[] = {-1.0f,  -0.712f, -0.453f, -0.303f, -0.216f, -0.148f, -0.116f, -0.08f, -0.061f, -0.034f, 0.0f,
											 0.034f, 0.061f,  0.08f,   0.116f,  0.148f,  0.216f,  0.303f,  0.453f, 0.712f,  1.0f};
static float JoystickInputToSteerRemap2[] = {-1.0f,  -0.736f, -0.542f, -0.4f, -0.292f, -0.214f, -0.16f, -0.123f, -0.078f, -0.036f, 0.0f,
											 0.036f, 0.078f,  0.123f,  0.16f, 0.214f,  0.292f,  0.4f,   0.542f,  0.736f,  1.0f};
static float JoystickInputToSteerRemap3[] = {-1.0f,  -0.8f,  -0.615f, -0.483f, -0.388f, -0.288f, -0.22f, -0.161f, -0.111f, -0.057f, 0.0f,
											 0.057f, 0.111f, 0.161f,  0.22f,   0.288f,  0.388f,  0.483f, 0.615f,  0.8f,    1.0f};
static float JoystickInputToSteerRemapDrift[] = {-1.0f, -1.0f,  -0.688f, -0.492f, -0.319f, -0.228f, -0.16f, -0.123f, -0.085f, -0.05f, 0.0f,
												 0.05f, 0.085f, 0.123f,  0.16f,   0.228f,  0.319f,  0.492f, 0.688f,  1.0f,    1.0f};
static const int STEER_REMAP_DEAD = 0;
static const int STEER_REMAP_MEDIUM = 1;
static const int STEER_REMAP_TWITCHY = 2;
static const int STEER_REMAP_DRIFT = 3;
static const int SteerInputRemapping = STEER_REMAP_MEDIUM;
static const int SteerInputRemappingDrift = STEER_REMAP_DRIFT;
static Table SteerInputRemapTables[] = {Table(JoystickInputToSteerRemap1, 21, -1.0f, 1.0f), Table(JoystickInputToSteerRemap2, 21, -1.0f, 1.0f),
										Table(JoystickInputToSteerRemap3, 21, -1.0f, 1.0f), Table(JoystickInputToSteerRemapDrift, 21, -1.0f, 1.0f)};

float SteeringRangeCoeffData[] = {1.0f, 1.0f, 1.1f, 1.2f, 1.25f, 1.35f};
Table SteeringRangeTable = Table(SteeringRangeData, 10, 0.0f, 160.0f);
Table SteeringWheelRangeTable = Table(SteeringWheelRangeData, 10, 0.0f, 160.0f);
Table SteeringRangeCoeffTable = Table(SteeringRangeCoeffData, 6, 0.0f, 1.0f);
Table SteeringSpeedTable = Table(SteeringSpeedData, 10, 0.0f, 160.0f);
Table SteeringInputSpeedCoeffTable = Table(SteeringInputSpeedData, 6, 0.0f, 10.0f);
Table SteeringInputCoeffTable = Table(SteeringInputData, 6, 0.0f, 1.0f);

static const float HardTurnSteeringThreshold = 0.5f;
static const float HardTurnTightenSpeed = 0.0f;
static const float Tweak_GameBreakerMaxSteer = 60.0f;
static const float Tweak_TuningSteering_Ratio = 0.2f;

GraphEntry<float> BurnoutFrictionData[] = {{0.0f, 1.0f}, {5.0f, 0.8f}, {9.0f, 0.9f}, {12.6f, 0.833f}, {17.1f, 0.72f}, {25.0f, 0.65f}};
tGraph<float> BurnoutFrictionTable(BurnoutFrictionData, 6);
float BurnOutCancelSlipValue = 0.5f;
float asd[] = {1.0f, 2.0f, 3.0f};
float BurnOutYawCancel = 0.5f;
float BurnOutAllowTime = 1.0f;
float BurnOutMaxSpeed = 20.0f;
float BurnOutFishTailTime = 2.0f;
int BurnOutFishTails = 6;
static const float Tweak_MinThrottleForBurnout = 1.0f;

// Credits: Brawltendo
void SuspensionRacer::Burnout::Update(float dT, float speedmph, float max_slip, int max_slip_wheel, float yaw) {
	// continue burnout/fishtailing state
	if (GetState()) {
		if (speedmph > 5.0f && UMath::Abs(ANGLE2RAD(yaw)) > BurnOutYawCancel) {
			Reset();
		} else if (max_slip < BurnOutCancelSlipValue) {
			IncBurnOutAllow(dT);
			if (mBurnOutAllow > BurnOutAllowTime)
				Reset();
		} else {
			ClearBurnOutAllow();
			DecBurnOutTime(dT);
			if (mBurnOutTime < 0.0f) {
				SetState(GetState() - 1);
				SetBurnOutTime(BurnOutFishTailTime);
			}
		}
	}
		// initialize burnout/fishtailing state
	else if (speedmph < BurnOutMaxSpeed) {
		const float friction_mult = 1.4f;
		// these conditions were split, there was probably some debug stuff here
		if (max_slip > 0.5f) {
			float burnout_coeff;
			BurnoutFrictionTable.GetValue(&burnout_coeff, max_slip);
			SetTraction(burnout_coeff / friction_mult);
			float friction_cut = 1.5f - burnout_coeff;
			// burnout state changes according to what side of the axle the wheel that's slipping the most is on
			SetState(BurnOutFishTails * friction_cut + (max_slip_wheel & 1));
			SetBurnOutTime(BurnOutFishTailTime);
			ClearBurnOutAllow();
		}
	}
}

void SuspensionRacer::Tire::BeginFrame(float max_slip, float grip_boost, float traction_boost, float drag_reduction) {
	mMaxSlip = max_slip;
	mDriveTorque = 0.0f;
	mBrakeTorque = 0.0f;
	SetForce({0,0,0});
	mLateralForce = 0.0f;
	mLongitudeForce = 0.0f;
	mTractionCircle = UMath::Vector2(1.0f, 1.0f);
	mTractionBoost = traction_boost;
	mGripBoost = grip_boost;
	mDriftFriction = 1.0f;
	mDragReduction = drag_reduction;
}

void SuspensionRacer::Tire::EndFrame(float dT) {}

// Credits: Brawltendo
Newtons SuspensionRacer::Tire::ComputeLateralForce(float load, float slip_angle) {
	float angle = ANGLE2DEG(slip_angle);
	float norm_angle = angle * 0.5f;
	int slip_angle_table = (int)norm_angle;
	load *= 0.001f; // convert to kN
	float extra = norm_angle - slip_angle_table;
	load *= LoadFactor;

	if (slip_angle_table > 5) {
		return (mSpecs->GetLayout()->GRIP_SCALE.At(mAxleIndex) * NewCorneringScale) * mGripBoost * GripFactor * LoadSensitivityTable[6]->GetValue(load);
	} else {
		float low = LoadSensitivityTable[slip_angle_table]->GetValue(load);
		float high = LoadSensitivityTable[slip_angle_table + 1]->GetValue(load);
		return (mSpecs->GetLayout()->GRIP_SCALE.At(mAxleIndex) * NewCorneringScale) * mGripBoost * GripFactor * (extra * (high - low) + low);
	}
}

// Credits: Brawltendo
float SuspensionRacer::Tire::GetPilotFactor(const float speed) {
	float PilotFactor = 0.85f;

	if (mBrakeLocked) {
		return 1.0f;
	}
	if (mAV < 0.0f) {
		return 1.0f;
	}
	if (IsSteeringWheel()) {
		return 1.0f;
	}

	float speed_factor = (speed - MPH2MPS(30.0f)) / MPH2MPS(20.0f);
	float val = UMath::Clamp(speed_factor, 0.0f, 1.0f);
	return val * (1.0f - PilotFactor) + PilotFactor;
}

float BrakingTorque = 4.0f;
float EBrakingTorque = 10.0f;

// Credits: Brawltendo
void SuspensionRacer::Tire::CheckForBrakeLock(float ground_force) {
	const float brake_spec = mBrakes->GetLayout()->BRAKE_LOCK.At(mAxleIndex) * FTLB2NM(mBrakes->GetLayout()->BRAKES.At(mAxleIndex).GetValue(UNDERCOVER_BrakesAtValue)) * BrakingTorque;
	const float ebrake_spec = FTLB2NM(mBrakes->GetLayout()->EBRAKE.GetValue(UNDERCOVER_BrakesAtValue)) * EBrakingTorque;
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

// Credits: Brawltendo
void SuspensionRacer::Tire::CheckSign() {
	if (mLastSign == WAS_POSITIVE) {
		if (mAV < 0.0f) {
			mAV = 0.0f;
		}
	} else if (mLastSign == WAS_NEGATIVE && mAV > 0.0f) {
		mAV = 0.0f;
	}

	if (mAV > FLOAT_EPSILON) {
		mLastSign = WAS_POSITIVE;
	} else if (mAV < -FLOAT_EPSILON) {
		mLastSign = WAS_NEGATIVE;
	} else {
		mLastSign = WAS_ZERO;
	}
}

float WheelMomentOfInertia = 10.0f;
static const float kOneMPH = 0.44703f;

// Credits: Brawltendo
// Updates forces for an unloaded/airborne tire
void SuspensionRacer::Tire::UpdateFree(float dT) {
	mLoad = 0.0f;
	mSlip = 0.0f;
	mTraction = 0.0f;
	mSlipAngle = 0.0f;
	CheckForBrakeLock(0.0f);

	if (mBrakeLocked) {
		mAngularAcc = 0.0f;
		mAV = 0.0f;
	} else {
		const float brake_spec = FTLB2NM(mBrakes->GetLayout()->BRAKES.At(mAxleIndex).GetValue(UNDERCOVER_BrakesAtValue)) * BrakingTorque;
		const float ebrake_spec = FTLB2NM(mBrakes->GetLayout()->EBRAKE.GetValue(UNDERCOVER_BrakesAtValue)) * EBrakingTorque;
		float bt = mBrake * brake_spec;
		float ebt = mEBrake * ebrake_spec;
		ApplyBrakeTorque(mAV > 0.0f ? -bt : bt);
		ApplyBrakeTorque(mAV > 0.0f ? -ebt : ebt);

		mAngularAcc = GetTotalTorque() / WheelMomentOfInertia;
		mAV += mAngularAcc * dT;
	}
	CheckSign();
	mLateralForce = 0.0f;
	mLongitudeForce = 0.0f;
}

float RollingFriction = 2.0f;
static const float TireForceEllipseRatio = 1.5f;
static const float InvTireForceEllipseRatio = 1.0 / TireForceEllipseRatio;

// Credits: Brawltendo
float SuspensionRacer::Tire::UpdateLoaded(float lat_vel, float fwd_vel, float body_speed, float load, float dT) {
	const float brake_spec = FTLB2NM(mBrakes->GetLayout()->BRAKES.At(mAxleIndex).GetValue(UNDERCOVER_BrakesAtValue)) * BrakingTorque;
	const float ebrake_spec = FTLB2NM(mBrakes->GetLayout()->EBRAKE.GetValue(UNDERCOVER_BrakesAtValue)) * EBrakingTorque;
	const float dynamicgrip_spec = mSpecs->GetLayout()->DYNAMIC_GRIP.At(mAxleIndex);
	const float staticgrip_spec = mSpecs->GetLayout()->STATIC_GRIP.At(mAxleIndex).GetValue(UNDERCOVER_StaticGripAtValue);
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

		ApplyDriveTorque(-GetDriveTorque() * mEBrake);
		ApplyBrakeTorque(bt);
		ApplyBrakeTorque(ebt);
	} else {
		ApplyBrakeTorque(mAV > 0.0f ? -bt : bt);
		ApplyBrakeTorque(mAV > 0.0f ? -ebt : ebt);
	}

	mSlipAngle = UMath::Atan2a(lat_vel, abs_fwd);
	float groundfriction = 0.0f;
	float slip_speed = mAV * mRadius - fwd_vel;
	float dynamicfriction = 1.0f;
	mSlip = slip_speed;
	float skid_speed = UMath::Sqrt(slip_speed * slip_speed + lat_vel * lat_vel);
	float pilot_factor = GetPilotFactor(body_speed);
	if (skid_speed > FLOAT_EPSILON && (lat_vel != 0.0f || fwd_vel != 0.0f)) {
		dynamicfriction = dynamicgrip_spec * mTractionBoost;
		dynamicfriction *= pilot_factor;
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
		mLongitudeForce = UMath::Limit(mLongitudeForce, GetTotalTorque() / mRadius);
	} else {
		mBrakeLocked = false;
		mLongitudeForce = GetTotalTorque() / mRadius;
		float slip_ang = mSlipAngle;
		mLateralForce = ComputeLateralForce(mLoad, UMath::Abs(mSlipAngle));
		if (lat_vel > 0.0f) {
			mLateralForce = -mLateralForce;
		}
	}

	mLateralForce *= mLateralBoost;
	if (mTraction >= 1.0f && !mBrakeLocked) {
		float acc_diff = mAngularAcc * mRadius - fwd_acc;
		mLongitudeForce += acc_diff * WheelMomentOfInertia / mRadius;
	}

	bool use_ellipse = false;
	if (GetTotalTorque() * fwd_vel > 0.0f && !mBrakeLocked) {
		use_ellipse = true;
		mLongitudeForce *= TireForceEllipseRatio;
	}

	mLateralForce *= mTractionCircle.x;
	mLongitudeForce *= mTractionCircle.y;

	float len_force = UMath::Sqrt(mLateralForce * mLateralForce + mLongitudeForce * mLongitudeForce);
	float max_force = mLoad * staticgrip_spec * mTractionBoost * mDriftFriction;

	max_force *= pilot_factor;

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
	mLateralForce *= mSurface.GetLayout()->LATERAL_GRIP;
	mLongitudeForce *= mSurface.GetLayout()->DRIVE_GRIP;

	if (fwd_vel > 1.0f) {
		mLongitudeForce -= UMath::Sina(mSlipAngle) * mLateralForce * mDragReduction / mSpecs->GetLayout()->GRIP_SCALE.At(mAxleIndex);
	} else {
		mLateralForce *= UMath::Min(UMath::Abs(lat_vel), 1.0f);
	}

	if (mBrakeLocked) {
		mAngularAcc = 0.0f;

	} else {
		if (mTraction < 1.0f) {
			float torque = (GetTotalTorque() - mLongitudeForce * mRadius + mLastTorque) * 0.5f;
			mLastTorque = torque;
			float rolling_resistance = RollingFriction * mSurface.GetLayout()->ROLLING_RESISTANCE;
			float effective_torque = torque - mAV * rolling_resistance;
			mAngularAcc = (effective_torque / WheelMomentOfInertia) - (mTraction * mSlip) / (mRadius * dT);
		}

		mAngularAcc = UMath::Lerp(mAngularAcc, fwd_acc / mRadius, mTraction);
	}

	mAV += mAngularAcc * dT;
	CheckSign();
	return mLateralForce;
}

float WheelDiameter(const Attrib::Gen::car_tuning &tires, bool front) {
	int axle = front ? 0 : 1;
	float diameter = INCH2METERS(tires.GetLayout()->RIM_SIZE.At(axle));
	return diameter + tires.GetLayout()->SECTION_WIDTH.At(axle) * 0.001f * 2.0f * (tires.GetLayout()->ASPECT_RATIO.At(axle) * 0.01f);
}

void SuspensionRacer::CreateTires() {
	for (int i = 0; i < 4; ++i) {
		delete mTires[i];
		bool is_front = IsFront(i);
		float diameter = WheelDiameter(mCarInfo, is_front);
		mTires[i] = new Tire(diameter * 0.5f, i, &mCarInfo, &mCarInfo);
	}
	UMath::Vector3 dimension;
	GetOwner()->GetRigidBody()->GetDimension(&dimension);

	float wheelbase = mCarInfo.GetLayout()->WHEEL_BASE;
	float axle_width_f = mCarInfo.GetLayout()->TRACK_WIDTH.At(0) - mCarInfo.GetLayout()->SECTION_WIDTH.At(0) * 0.001f;
	float axle_width_r = mCarInfo.GetLayout()->TRACK_WIDTH.At(1) - mCarInfo.GetLayout()->SECTION_WIDTH.At(1) * 0.001f;
	float front_axle = mCarInfo.GetLayout()->FRONT_AXLE;

	UMath::Vector3 fl{-axle_width_f * 0.5f, -dimension.y, front_axle};
	UMath::Vector3 fr{axle_width_f * 0.5f, -dimension.y, front_axle};
	UMath::Vector3 rl{-axle_width_r * 0.5f, -dimension.y, front_axle - wheelbase};
	UMath::Vector3 rr{axle_width_r * 0.5f, -dimension.y, front_axle - wheelbase};

	GetWheel(0).SetLocalArm(fl);
	GetWheel(1).SetLocalArm(fr);
	GetWheel(2).SetLocalArm(rl);
	GetWheel(3).SetLocalArm(rr);
}

void SuspensionRacer::OnBehaviorChange(const UCrc32 &mechanic) {
	if (mechanic.mCRC == BEHAVIOR_MECHANIC_ENGINE.mHash32) {
		GetOwner()->QueryInterface(&mTransmission);
		GetOwner()->QueryInterface(&mEngine);
		GetOwner()->QueryInterface(&mEngineDamage);
	} else if (mechanic.mCRC == BEHAVIOR_MECHANIC_INPUT.mHash32) {
		GetOwner()->QueryInterface(&mInput);
	} else if (mechanic.mCRC == BEHAVIOR_MECHANIC_DAMAGE.mHash32) {
		GetOwner()->QueryInterface(&mSpikeDamage);
	} else if (mechanic.mCRC == BEHAVIOR_MECHANIC_RIGIDBODY.mHash32) {
		GetOwner()->QueryInterface(&mCollisionBody);
		GetOwner()->QueryInterface(&mRB);
	} else if (mechanic.mCRC == BEHAVIOR_MECHANIC_AI.mHash32) {
		GetOwner()->QueryInterface(&mHumanAI);
	}
}

void SuspensionRacer::Create(const BehaviorParams& bp) {
	// ISuspension(bp.fowner), mAttributes(this, 0)

	mJumpTime = 0.0f;
	mJumpAlititude = 0.0f;
	mTireHeat = 0.0f;
	ctor_cartuning(&mCarInfo, cartuning_LookupKey(&mCarInfo, GetOwner(), 0));  // todo does this work
	mRB = nullptr;
	mCollisionBody = nullptr;
	mGameBreaker = 0.0;
	mNumWheelsOnGround = 0;
	mLastGroundCollision = 0.0;
	mDrift = Drift();
	mBurnOut = Burnout();
	mSteering = Steering();

	GetOwner()->QueryInterface(&mRB);
	GetOwner()->QueryInterface(&mCollisionBody);
	GetOwner()->QueryInterface(&mTransmission);
	GetOwner()->QueryInterface(&mHumanAI);
	GetOwner()->QueryInterface(&mInput);
	GetOwner()->QueryInterface(&mEngine);
	GetOwner()->QueryInterface(&mEngineDamage);
	GetOwner()->QueryInterface(&mSpikeDamage);
	//Sim::Collision::AddListener(this, GetOwner(), "SuspensionRacer"); // todo

	for (int i = 0; i < 4; ++i) {
		mTires[i] = NULL;
	}
	CreateTires();
}

static const int bJumpStabilizer = 1;
bVector2 JumpStabilizationGraph[] = {bVector2(0.0f, 0.0f), bVector2(0.4f, 0.15f), bVector2(2.0f, 5.0f)};
Graph JumpStabilization(JumpStabilizationGraph, 3);
static const int bActiveStabilizer = 1;
static const float fPitchStabilizerAction = 40.0f;
static const float fRollStabilizerAction = 20.0f;
static const float fStablizationAltitude = 15.0f;
static const float fStabilizerUp = 0.8f;
static const float fStabilizerSpeed = 5.0f;
static const float fStabilizerMaxAngVel = 0.0f;
static const int bDoLandingGravity = 1;
static const float fExtraLandingGravity = 3.0f;
static const float fLandingGravitySpeed = 20.0f;
static const float fLandingGravityUpThreshold = 0.96f;
static const float fLandingGravityMinTime = 1.0f;
static const float fLandingGravityMinAltitude = 2.0f;
static const float fLandingGravityMaxAltitude = 6.0f;

void SuspensionRacer::DoJumpStabilizer(const State &state) {
	if (!bJumpStabilizer || !mCollisionBody) {
		return;
	}

	int nTouching = mNumWheelsOnGround;
	bool resolve = false;
	UMath::Vector3 ground_normal = *mCollisionBody->GetGroundNormal(); // NOTE: it returns a vec3 but heightaboveground is right after it anyway
	//float altitude = -mCollisionBody->GetHeightAboveGround();
	float altitude = mCollisionBody->GetHeightAboveGround();
	float ground_dot = UMath::Dot(state.GetUpVector(), ground_normal);

	UMath::Vector3 damping_torque = {0,0,0};
	UMath::Vector3 damping_force = {0,0,0};

	if (!nTouching) {
		mJumpTime += state.time;
		mJumpAlititude = UMath::Max(mJumpAlititude, altitude);

		if (bDoLandingGravity) {
			float accel = fExtraLandingGravity;
			// apply more downforce when the car has been airborne for a long time
			if (mJumpTime > fLandingGravityMinTime && ground_dot > fLandingGravityUpThreshold && mJumpAlititude > fLandingGravityMinAltitude &&
				state.linear_vel.y < 0.0f && altitude < fLandingGravityMaxAltitude) {
				float alt_ratio = 1.0f - UMath::Ramp(altitude, 0.0f, fLandingGravityMaxAltitude);
				float speed_ratio = UMath::Ramp(state.speed, 0.0f, fLandingGravitySpeed);
				accel += alt_ratio * 10.0f * speed_ratio;
			}

			UMath::Vector3 df_extra{0.0f, -state.mass * accel, 0.0f};
			UMath::Add(damping_force, df_extra, damping_force);
			resolve = true;
		}
	} else {
		mJumpTime = 0.0f;
		mJumpAlititude = 0.0f;
	}

	if (bJumpStabilizer && nTouching < 2 && state.GetUpVector().y > fStabilizerUp && !mCollisionBody->IsInGroundContact()) {
		float speed_ramp = UMath::Ramp(state.speed, 0.0f, fStabilizerSpeed);
		float avelmag = UMath::Length(state.local_angular_vel);
		float damping = speed_ramp * JumpStabilization.GetValue(avelmag);

		UMath::Vector3 damping_moment;
		UMath::Scale(state.local_angular_vel, state.inertia, damping_moment);
		UMath::Scale(damping_moment, -damping, damping_moment);
		damping_moment.y = 0.0f;
		UMath::Rotate(damping_moment, state.matrix, damping_moment);
		UMath::Add(damping_moment, damping_torque, damping_torque);
		resolve = true;
	}

	if (bActiveStabilizer && nTouching == 0 && ground_normal.y > 0.9f && state.GetUpVector().y > 0.1f && ground_dot > 0.8f &&
		altitude > FLOAT_EPSILON) {
		float altitude_ramp = 1.0f - UMath::Ramp(altitude, 0.0f, fStablizationAltitude);
		float speed_ramp = UMath::Ramp(state.speed, 0.0f, fStabilizerSpeed);

		UMath::Vector3 vMoment;
		UMath::Vector3 vFlatFwd;
		UMath::UnitCross(state.GetRightVector(), ground_normal, vFlatFwd);
		float dot = UMath::Dot(vFlatFwd, state.GetForwardVector());
		if (dot < 0.99f) {
			UMath::UnitCross(vFlatFwd, state.GetForwardVector(), vMoment);
			float fMag = state.mass * speed_ramp * altitude_ramp * fPitchStabilizerAction * (dot - 1.0f);
			UMath::ScaleAdd(vMoment, fMag, damping_torque, damping_torque);
			resolve = true;
		}

		UMath::Vector3 vFlatRight;
		UMath::UnitCross(ground_normal, state.GetForwardVector(), vFlatRight);
		dot = UMath::Dot(vFlatRight, state.GetRightVector());
		if (dot < 0.99f) {
			UMath::UnitCross(vFlatRight, state.GetRightVector(), vMoment);
			float fMag = state.mass * speed_ramp * fRollStabilizerAction * altitude_ramp * (dot - 1.0f);
			UMath::ScaleAdd(vMoment, fMag, damping_torque, damping_torque);
			resolve = true;
		}
	}

	if (resolve) {
		IRigidBody *irb = GetOwner()->GetRigidBody();
		irb->Resolve(&damping_force, &damping_torque);
	}
}

/*float SuspensionRacer::CalculateUndersteerFactor() const {
	float magnitude = 0.0f;
	float slip_avg = (GetWheelSkid(0) + GetWheelSkid(1)) / 2.0f;
	float steer = (GetWheelSteer(0) + GetWheelSteer(1)) / 2.0f;
	float speed = GetOwner()->GetRigidBody()->GetSpeed();
	if ((GetVehicle()->GetSpeed() > 0.0f) && (speed > 1.0f) && (steer * slip_avg < 0.0f)) {
		magnitude = UMath::Abs(slip_avg) / speed;
	}
	return UMath::Min(magnitude, 1.0f);
}*/

Mps SuspensionRacer::ComputeMaxSlip(const State &state) const {
	float ramp = UMath::Ramp(state.speed, 10.0f, 71.0f);
	float result = ramp + 0.5f;
	if (state.gear == G_REVERSE)
		result = 71.0f;
	return result;
}

float GripVsSpeed[] = {0.833f, 0.958f, 1.008f, 1.0167f, 1.033f, 1.033f, 1.033f, 1.0167f, 1.0f, 1.0f};
Table GripRangeTable(GripVsSpeed, 10, 0.0f, 1.0f);

// Credits: Brawltendo
float SuspensionRacer::ComputeLateralGripScale(const State &state) const {
	// lateral grip is tripled when in a drag race
	if (state.driver_style == STYLE_DRAG) {
		return 3.0f;
	}

	float ratio = UMath::Ramp(state.speed, 0.0f, MPH2MPS(85.0f));
	return GripRangeTable.GetValue(ratio) * 1.2f;
}

float TractionVsSpeed[] = {0.90899998f, 1.045f, 1.09f, 1.09f, 1.09f, 1.09f, 1.09f, 1.045f, 1.0f, 1.0f};
Table TractionRangeTable(TractionVsSpeed, 10, 0.0f, 1.0f);
static const float Traction_RangeMaxSpeedMPH = 85.0f;
static const float Tweak_GlobalTractionScale = 1.1f;
static const float Tweak_ReverseTractionScale = 2.0f;

// Credits: Brawltendo
float SuspensionRacer::ComputeTractionScale(const State &state) const {
	float result = 1.0f;

	if (state.driver_style == STYLE_DRAG) {
		result = Tweak_GlobalTractionScale;
	} else {
		float ratio = UMath::Ramp(state.speed, 0.0f, MPH2MPS(Traction_RangeMaxSpeedMPH));
		result = TractionRangeTable.GetValue(ratio) * Tweak_GlobalTractionScale;
	}

	if (state.gear == G_REVERSE) {
		result = Tweak_ReverseTractionScale;
	}

	return result;
}

void SuspensionRacer::SetCOG(float extra_bias, float extra_ride) {
	float front_z = mCarInfo.GetLayout()->FRONT_AXLE;
	float rear_z = front_z - mCarInfo.GetLayout()->WHEEL_BASE;
	IRigidBody *irb = GetOwner()->GetRigidBody();

	UMath::Vector3 dim;
	irb->GetDimension(&dim);

	//float fwbias = (mCarInfo.GetLayout()->FRONT_WEIGHT_BIAS + extra_bias) * 0.01f; // todo is there no equivalent for this?
	float fwbias = extra_bias * 0.01f;
	if (mNumWheelsOnGround == 0) {
		fwbias = 0.5f;
	}
	float cg_z = (front_z - rear_z) * fwbias + rear_z;
	float cg_y = INCH2METERS(mCarInfo.GetLayout()->ROLL_CENTER.GetValue(UNDERCOVER_RollCenterAtValue)) - (dim.y + UMath::Max(INCH2METERS(mCarInfo.GetLayout()->RIDE_HEIGHT.At(0) + extra_ride),
																			  INCH2METERS(mCarInfo.GetLayout()->RIDE_HEIGHT.At(1) + extra_ride)));
	UMath::Vector3 cog{0.0f, cg_y, cg_z};
	mRB->SetCenterOfGravity(&cog);
}

void SuspensionRacer::DoTireHeat(const State &state) {
	if (state.flags & 1) {
		for (unsigned int i = 0; i < 4; ++i) {
			if (mTires[i]->GetCurrentSlip() > 0.5f) {
				this->mTireHeat += state.time / 3.0f;
				this->mTireHeat = UMath::Min(this->mTireHeat, 1.0f);
				return;
			}
		}
	} else {
		if (this->mTireHeat > 0.0f) {
			this->mTireHeat -= state.time / 6.0f;
			this->mTireHeat = UMath::Max(this->mTireHeat, 0.0f);
		}
	}
}

static float AeroDropOff = 0.5f;
static float AeroDropOffMin = 0.4f;
static float OffThrottleDragFactor = 2.0f;
static float OffThrottleDragCenterHeight = -0.1f;
static const float Tweak_TuningAero_Drag = 0.25f;
static const float Tweak_TuningAero_DownForce = 0.25f;
static const float Tweak_PlaneDynamics = 0.0f;

// Credits: Brawltendo
// TODO stack frame is slightly off
void SuspensionRacer::DoAerodynamics(const State &state, float drag_pct, float aero_pct, float aero_front_z, float aero_rear_z,
							 const Physics::Tunings *tunings) {
	IRigidBody *irb = this->GetOwner()->GetRigidBody();

	if (drag_pct > 0.0f) {
		const float dragcoef_spec = mCarInfo.GetLayout()->DRAG_COEFFICIENT;
		// drag increases relative to the car's speed
		// letting off the throttle will increase drag by OffThrottleDragFactor
		float drag = state.speed * drag_pct * dragcoef_spec;
		drag += drag * (OffThrottleDragFactor - 1.0f) * (1.0f - state.gas_input);
		// todo?
		//if (tunings) {
		//	drag += drag * Tweak_TuningAero_Drag * tunings->Value[Physics::Tunings::AERODYNAMICS];
		//}

		UMath::Vector3 drag_vector(state.linear_vel);
		drag_vector *= -drag;
		UMath::Vector3 drag_center(state.cog);

		// manipulate drag height based on off-throttle amount when 2 or more wheels are grounded
		if (state.ground_effect >= 0.5f)
			drag_center.y += OffThrottleDragCenterHeight * (1.0f - state.gas_input);

		UMath::RotateTranslate(drag_center, state.matrix, drag_center);
		irb->ResolveForce(&drag_vector, &drag_center);
	}

	if (aero_pct > 0.0f) {
		// scale downforce by the gradient when less than 2 wheels are grounded
		float upness = UMath::Max(state.GetUpVector().y, 0.0f);
		if (state.ground_effect >= 0.5f)
			upness = 1.0f;

		// in reverse, the car's forward vector is used as the movement direction
		UMath::Vector3 movement_dir(state.GetForwardVector());
		if (state.speed > 0.0001f) {
			movement_dir = state.linear_vel;
			movement_dir *= 1.0f / state.speed;
		}

		float forwardness = UMath::Max(UMath::Dot(movement_dir, state.GetForwardVector()), 0.0f);
		forwardness = UMath::Max(AeroDropOffMin, UMath::Pow(forwardness, AeroDropOff));
		float downforce = aero_pct * upness * forwardness * Physics::Info::AerodynamicDownforce(mAttributes, state.speed); // todo
		// lower downforce when car is in air
		if (state.ground_effect == 0.0f) {
			downforce *= 0.8f;
		}
		// todo
		//if (tunings) {
		//	downforce += downforce * Tweak_TuningAero_DownForce * tunings->Value[Physics::Tunings::AERODYNAMICS];
		//}

		if (downforce > 0.0f) {
			UMath::Vector3 aero_center{state.cog.x, state.cog.y, state.cog.z};
			// when at least 1 wheel is grounded, change the downforce forward position using the aero CG and axle positions
			if (state.ground_effect != 0.0f) {
				aero_center.z = (aero_front_z - aero_rear_z) * (mCarInfo.GetLayout()->AERO_CG.GetValue(UNDERCOVER_AeroCGAtValue) * 0.01f) + aero_rear_z;
			}

			if (Tweak_PlaneDynamics != 0.0f) {
				// just some random nonsense because the DWARF says there was a block here
				float pitch = UMath::Atan2a(UMath::Abs(state.matrix.z.z), state.matrix.z.x);
				aero_center.z *= pitch;
			}

			UMath::Vector3 force{0.0f, -downforce, 0.0f};
			UMath::RotateTranslate(aero_center, state.matrix, aero_center);
			UMath::Rotate(force, state.matrix, force);
			irb->ResolveForce(&force, &aero_center);
		}
	}
}

static const float Tweak_SteerDragReduction = 0.15f;
static const float Tweak_GameBreakerSteerDragReduction = 0.15f;
static const float Tweak_GameBreakerExtraGs = -2.0f;
static const float TweakGameBreakerRampOutPhysicsTime = 1.0f / 3.0f;
static const float Tweak_DragAeroMult = 1.5f;

void SuspensionRacer::OnTaskSimulate(float dT) {
	if (!mCollisionBody || !mRB) {
		return;
	}

	ISimable *owner = GetOwner();

	float ride_extra = 0.0f;
	const Physics::Tunings *tunings = GetVehicle()->GetTunings();
	// todo
	//if (tunings) {
	//	ride_extra = tunings->Value[Physics::Tunings::RIDEHEIGHT];
	//}
	SetCOG(0.0, ride_extra);

	State state;
	ComputeState(dT, state);

	mSteering.CollisionTimer = UMath::Max(mSteering.CollisionTimer - state.time, 0.0f);
	mGameBreaker = 0.0f;

	IPlayer *player = GetOwner()->GetPlayer();
	if (player && player->InGameBreaker()) {
		mGameBreaker = 1.0f;
	} else if (mGameBreaker > 0.0f) {
		mGameBreaker -= state.time * TweakGameBreakerRampOutPhysicsTime;
		mGameBreaker = UMath::Max(mGameBreaker, 0.0f);
	}
	if (mGameBreaker > 0.0f) {
		UMath::Vector3 extra_df;
		UMath::Scale(state.GetUpVector(), Tweak_GameBreakerExtraGs * mGameBreaker * state.mass * 9.81f, extra_df);
		mRB->ResolveForce(&extra_df);
	}

	float max_slip = ComputeMaxSlip(state);
	float grip_scale = ComputeLateralGripScale(state);
	float traction_scale = ComputeTractionScale(state);
	float steerdrag_reduction = UMath::Lerp(Tweak_SteerDragReduction, 1.0f, mGameBreaker);
	if ((state.flags & 1) == 0) {
		float launch = GetVehicle()->GetPerfectLaunch();
		if (launch > 0.0f) {
			traction_scale += launch * 0.25f;
		}
	}

	for (unsigned int i = 0; i < 4; ++i) {
		mTires[i]->BeginFrame(max_slip, grip_scale, traction_scale, steerdrag_reduction);
	}

	float drag_pct = 1.0f - mGameBreaker * 0.75f;
	float aero_pct = 1.0f;
	if (state.driver_style == STYLE_DRAG) {
		aero_pct = Tweak_DragAeroMult;
	}
	DoAerodynamics(state, drag_pct, aero_pct, GetWheel(0).GetLocalArm().z, GetWheel(2).GetLocalArm().z, tunings);
	DoDriveForces(state);
	DoWheelForces(state);

	for (unsigned int i = 0; i < 4; ++i) {
		mTires[i]->UpdateTime(dT);
	}

	mSteering.WallNoseTurn = 0.0f;
	mSteering.WallSideTurn = 0.0f;

	for (unsigned int i = 0; i < 4; ++i) {
		mTires[i]->EndFrame(dT);
	}

	DoTireHeat(state);
	DoJumpStabilizer(state);
	//DoSleep(state); // todo?
	//Chassis::OnTaskSimulate(dT);
}