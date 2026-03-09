float GetSimTimeNew() {
	return Sim::GetTime() / 1000.0;
}

struct MWCarTuning {
	struct TempTable {
		float value;

		TempTable() {}
		TempTable(float f) { value = f; }
		float GetValue(float) { return value; }
	};

	struct TempAxlePairTable {
		TempTable pair[2];

		TempTable& At(int i) { return pair[i]; }
	};

	// chassis
	TempTable AERO_CG;
	TempTable AERO_COEFFICIENT;
	float DRAG_COEFFICIENT;
	float FRONT_AXLE;
	float FRONT_WEIGHT_BIAS;
	float RENDER_MOTION;
	AxlePair RIDE_HEIGHT;
	TempTable ROLL_CENTER;
	float SHOCK_BLOWOUT;
	AxlePair SHOCK_DIGRESSION;
	TempAxlePairTable SHOCK_EXT_STIFFNESS;
	TempAxlePairTable SHOCK_STIFFNESS;
	AxlePair SHOCK_VALVING;
	AxlePair SPRING_PROGRESSION;
	AxlePair SPRING_STIFFNESS;
	TempAxlePairTable SWAYBAR_STIFFNESS;
	AxlePair TRACK_WIDTH;
	AxlePair TRAVEL;
	float WHEEL_BASE;

	// brakes
	AxlePair BRAKE_LOCK;
	TempAxlePairTable BRAKES;
	TempTable EBRAKE;

	// tires
	AxlePair ASPECT_RATIO;
	AxlePair DYNAMIC_GRIP;
	AxlePair GRIP_SCALE;
	AxlePair RIM_SIZE;
	AxlePair SECTION_WIDTH;
	TempAxlePairTable STATIC_GRIP;
	TempTable STEERING;
	float YAW_CONTROL[4];
	float YAW_SPEED;

	// transmission
	//float CLUTCH_SLIP;
	float DIFFERENTIAL[3];
	//float FINAL_GEAR;
	//float GEAR_EFFICIENCY[9];
	//float GEAR_RATIO[9];
	//float OPTIMAL_SHIFT;
	//float SHIFT_SPEED;
	//float TORQUE_CONVERTER;
	float TORQUE_SPLIT;
};

// data from mw elise
Attrib::Gen::car_tuning::_LayoutStruct* gCurrentCarInfo = nullptr;
auto GetMWCarData() {
	//return gCurrentCarInfo;

	static MWCarTuning tmp;
	// chassis
	tmp.AERO_CG = 50.0;
	tmp.AERO_COEFFICIENT = 0.14;
	tmp.DRAG_COEFFICIENT = 0.22;
	tmp.FRONT_AXLE = 1.13;
	tmp.FRONT_WEIGHT_BIAS = 53;
	tmp.RENDER_MOTION = 1.0;
	tmp.RIDE_HEIGHT = {8,8};
	tmp.ROLL_CENTER = 11;
	tmp.SHOCK_BLOWOUT = 5;
	tmp.SHOCK_DIGRESSION = {0.1,0.1};
	tmp.SHOCK_EXT_STIFFNESS = {35,35};
	tmp.SHOCK_STIFFNESS = {30,30};
	tmp.SHOCK_VALVING = {18,18};
	tmp.SPRING_PROGRESSION = {6.1,6.1};
	tmp.SPRING_STIFFNESS = {450,400};
	tmp.SWAYBAR_STIFFNESS = {200,200};
	tmp.TRACK_WIDTH = {1.465,1.522};
	tmp.TRAVEL = {6.5,6.5};
	tmp.WHEEL_BASE = 2.35;

	// brakes
	tmp.BRAKE_LOCK = {1.0,2.75};
	tmp.BRAKES = {280,350};
	tmp.EBRAKE = 400;

	// tires
	tmp.ASPECT_RATIO = {45,45};
	tmp.DYNAMIC_GRIP = {1.4,1.5};
	tmp.GRIP_SCALE = {1.0,1.0};
	tmp.RIM_SIZE = {17,17};
	tmp.SECTION_WIDTH = {225,225};
	tmp.STATIC_GRIP = {1.6,1.7};
	tmp.STEERING = 1.0;
	tmp.YAW_CONTROL[0] = 0.0;
	tmp.YAW_CONTROL[1] = 0.1;
	tmp.YAW_CONTROL[2] = 0.3;
	tmp.YAW_CONTROL[3] = 1.0;
	tmp.YAW_SPEED = 0.4;

	// transmission
	tmp.DIFFERENTIAL[0] = 0.85;
	tmp.DIFFERENTIAL[1] = 0.85;
	tmp.DIFFERENTIAL[2] = 0;
	tmp.TORQUE_SPLIT = 0;
	return &tmp;
}

namespace Physics {
	namespace Info {
		float AerodynamicDownforce(const Attrib::Gen::car_tuning &chassis, const float speed) {
			return speed * 2 * GetMWCarData()->AERO_COEFFICIENT.GetValue(UNDERCOVER_AeroCoeffAtValue) * 1000.0f;
		}
	}
}

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
		return (GetMWCarData()->GRIP_SCALE.At(mAxleIndex) * NewCorneringScale) * mGripBoost * GripFactor * LoadSensitivityTable[6]->GetValue(load);
	} else {
		float low = LoadSensitivityTable[slip_angle_table]->GetValue(load);
		float high = LoadSensitivityTable[slip_angle_table + 1]->GetValue(load);
		return (GetMWCarData()->GRIP_SCALE.At(mAxleIndex) * NewCorneringScale) * mGripBoost * GripFactor * (extra * (high - low) + low);
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
	const float brake_spec = GetMWCarData()->BRAKE_LOCK.At(mAxleIndex) * FTLB2NM(GetMWCarData()->BRAKES.At(mAxleIndex).GetValue(UNDERCOVER_BrakesAtValue)) * BrakingTorque;
	const float ebrake_spec = FTLB2NM(GetMWCarData()->EBRAKE.GetValue(UNDERCOVER_BrakesAtValue)) * EBrakingTorque;
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
		const float brake_spec = FTLB2NM(GetMWCarData()->BRAKES.At(mAxleIndex).GetValue(UNDERCOVER_BrakesAtValue)) * BrakingTorque;
		const float ebrake_spec = FTLB2NM(GetMWCarData()->EBRAKE.GetValue(UNDERCOVER_BrakesAtValue)) * EBrakingTorque;
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
	const float brake_spec = FTLB2NM(GetMWCarData()->BRAKES.At(mAxleIndex).GetValue(UNDERCOVER_BrakesAtValue)) * BrakingTorque;
	const float ebrake_spec = FTLB2NM(GetMWCarData()->EBRAKE.GetValue(UNDERCOVER_BrakesAtValue)) * EBrakingTorque;
	const float dynamicgrip_spec = GetMWCarData()->DYNAMIC_GRIP.At(mAxleIndex);
	const float staticgrip_spec = GetMWCarData()->STATIC_GRIP.At(mAxleIndex).GetValue(UNDERCOVER_StaticGripAtValue);
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

	// todo this is different in UC
	//auto fDriveGrip = mSurface.GetLayout()->DRIVE_GRIP;
	//auto fLateralGrip = mSurface.GetLayout()->LATERAL_GRIP;
	//auto fRollingResistance = mSurface.GetLayout()->ROLLING_RESISTANCE;
	auto fDriveGrip = 1.0f;
	auto fLateralGrip = 1.0f;
	auto fRollingResistance = 1.0f;

	// factor surface friction into the tire force
	mLateralForce *= fLateralGrip;
	mLongitudeForce *= fDriveGrip;

	if (fwd_vel > 1.0f) {
		mLongitudeForce -= UMath::Sina(mSlipAngle) * mLateralForce * mDragReduction / GetMWCarData()->GRIP_SCALE.At(mAxleIndex);
	} else {
		mLateralForce *= UMath::Min(UMath::Abs(lat_vel), 1.0f);
	}

	if (mBrakeLocked) {
		mAngularAcc = 0.0f;

	} else {
		if (mTraction < 1.0f) {
			float torque = (GetTotalTorque() - mLongitudeForce * mRadius + mLastTorque) * 0.5f;
			mLastTorque = torque;
			float rolling_resistance = RollingFriction * fRollingResistance;
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
	FUNCTION_LOG("SuspensionRacer::CreateTires");

	for (int i = 0; i < 4; ++i) {
		delete mTires[i];
		bool is_front = IsFront(i);
		float diameter = WheelDiameter(mCarInfo, is_front);
		mTires[i] = new Tire(diameter * 0.5f, i, &mCarInfo, &mCarInfo);
	}
	UMath::Vector3 dimension;
	GetOwner()->GetRigidBody()->GetDimension(&dimension);

	float wheelbase = GetMWCarData()->WHEEL_BASE;
	float axle_width_f = GetMWCarData()->TRACK_WIDTH.At(0) - GetMWCarData()->SECTION_WIDTH.At(0) * 0.001f;
	float axle_width_r = GetMWCarData()->TRACK_WIDTH.At(1) - GetMWCarData()->SECTION_WIDTH.At(1) * 0.001f;
	float front_axle = GetMWCarData()->FRONT_AXLE;

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
	FUNCTION_LOG("SuspensionRacer::OnBehaviorChange");
	if (mechanic.mCRC == BEHAVIOR_MECHANIC_ENGINE.mHash32) {
		GetOwner()->QueryInterface(&mTransmission);
		GetOwner()->QueryInterface(&mEngine);
		GetOwner()->QueryInterface(&mEngineDamage);
		WriteLog("BEHAVIOR_MECHANIC_ENGINE");
	} else if (mechanic.mCRC == BEHAVIOR_MECHANIC_INPUT.mHash32 || mechanic.mCRC == BEHAVIOR_MECHANIC_AI.mHash32) {
		WriteLog("BEHAVIOR_MECHANIC_AI");
		GetOwner()->QueryInterface(&mInput);
		GetOwner()->QueryInterface(&mHumanAI);
	} else if (mechanic.mCRC == BEHAVIOR_MECHANIC_DAMAGE.mHash32) {
		WriteLog("BEHAVIOR_MECHANIC_DAMAGE");
		GetOwner()->QueryInterface(&mSpikeDamage);
	} else if (mechanic.mCRC == BEHAVIOR_MECHANIC_RIGIDBODY.mHash32) {
		WriteLog("BEHAVIOR_MECHANIC_RIGIDBODY");
		GetOwner()->QueryInterface(&mCollisionBody);
		GetOwner()->QueryInterface(&mRB);
	}
	WriteLog("OnBehaviorChange finished");
}

void* NewSuspensionRacerVTable[] = {
		(void*)0x69F570, // generic OnService
		(void*)&SuspensionRacer::dtor,
		(void*)&SuspensionRacer::Reset,
		(void*)&SuspensionRacer::GetPriority,
		(void*)&SuspensionRacer::OnOwnerAttached,
		(void*)&SuspensionRacer::OnOwnerDetached,
		(void*)&SuspensionRacer::OnTaskSimulate,
		(void*)&SuspensionRacer::OnBehaviorChange,
		(void*)&SuspensionRacer::OnPause,
		(void*)&SuspensionRacer::OnUnPause,
		(void*)&SuspensionRacer::OnDebugDraw,
		(void*)&SuspensionRacer::CalculateUndersteerFactor,
		(void*)&SuspensionRacer::CalculateOversteerFactor,
		(void*)&SuspensionRacer::GetDownCoefficient,
		(void*)&SuspensionRacer::GetDynamicRideHeight,
		(void*)&SuspensionRacer::GetDriftValue,
		(void*)&SuspensionRacer::ApplyVehicleEntryForces,
};

void SuspensionRacer::Create(const BehaviorParams& bp) {
	FUNCTION_LOG("SuspensionRacer::Create");

	*(uintptr_t*)this = (uintptr_t)&NewSuspensionRacerVTable;
	*(uintptr_t*)&tmpChassis = (uintptr_t)&NewChassisVTable;
	tmpChassis.mCOMObject = &bp.fowner->Object;
	bp.fowner->Object.Add(&tmpChassis);

	mJumpTime = 0.0f;
	mJumpAlititude = 0.0f;
	mTireHeat = 0.0f;
	ctor_cartuning(&mCarInfo, cartuning_LookupKey(&mCarInfo, GetOwner(), 0));
	gCurrentCarInfo = mCarInfo.GetLayout();
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
	float front_z = GetMWCarData()->FRONT_AXLE;
	float rear_z = front_z - GetMWCarData()->WHEEL_BASE;
	IRigidBody *irb = GetOwner()->GetRigidBody();

	UMath::Vector3 dim;
	irb->GetDimension(&dim);

	//float fwbias = (GetMWCarData()->FRONT_WEIGHT_BIAS + extra_bias) * 0.01f; // todo is there no equivalent for this?
	//float fwbias = (*(float*)Attrib::Instance::GetAttributePointer(&mCarInfo, Attrib::StringHash32("FRONT_WEIGHT_BIAS"), 0) + extra_bias) * 0.01f;
	float fwbias = extra_bias * 0.01f;
	if (mNumWheelsOnGround == 0) {
		fwbias = 0.5f;
	}
	float cg_z = (front_z - rear_z) * fwbias + rear_z;
	float cg_y = INCH2METERS(GetMWCarData()->ROLL_CENTER.GetValue(UNDERCOVER_RollCenterAtValue)) - (dim.y + UMath::Max(INCH2METERS(GetMWCarData()->RIDE_HEIGHT.At(0) + extra_ride),
																			  INCH2METERS(GetMWCarData()->RIDE_HEIGHT.At(1) + extra_ride)));
	UMath::Vector3 cog{0.0f, cg_y, cg_z};
	mRB->SetCenterOfGravity(&cog);
}

void SuspensionRacer::DoTireHeat(const State &state) {
	if (state.flags & State::IS_STAGING) {
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
void SuspensionRacer::DoAerodynamics(const State &state, float drag_pct, float aero_pct, float aero_front_z, float aero_rear_z,
							 const Physics::Tunings *tunings) {
	IRigidBody *irb = this->GetOwner()->GetRigidBody();

	if (drag_pct > 0.0f) {
		const float dragcoef_spec = GetMWCarData()->DRAG_COEFFICIENT;
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
		float downforce = aero_pct * upness * forwardness * Physics::Info::AerodynamicDownforce(mCarInfo, state.speed);
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
				aero_center.z = (aero_front_z - aero_rear_z) * (GetMWCarData()->AERO_CG.GetValue(UNDERCOVER_AeroCGAtValue) * 0.01f) + aero_rear_z;
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

// Credits: Brawltendo
void SuspensionRacer::Differential::CalcSplit(bool locked) {
	if (!has_traction[0] || !has_traction[1] || locked || (factor <= 0.0f)) {
		torque_split[0] = bias;
		torque_split[1] = 1.0f - bias;
		return;
	}
	float av_0 = angular_vel[0] * (1.0f - bias);
	float av_1 = angular_vel[1] * bias;
	float combined_av = UMath::Abs(av_0 + av_1);

	if (combined_av > FLOAT_EPSILON) {
		torque_split[0] = ((1.0f - factor) * bias) + ((factor * UMath::Abs(av_1)) / combined_av);
		torque_split[1] = ((1.0f - factor) * (1.0f - bias)) + ((factor * UMath::Abs(av_0)) / combined_av);
	} else {
		torque_split[0] = bias;
		torque_split[1] = 1.0f - bias;
	}

	torque_split[0] = UMath::Clamp(torque_split[0], 0.0f, 1.0f);
	torque_split[1] = UMath::Clamp(torque_split[1], 0.0f, 1.0f);
}

// Credits: Brawltendo
void SuspensionRacer::DoDriveForces(State &state) {
	if (!mTransmission) {
		return;
	}

	float drive_torque = mTransmission->GetDriveTorque();
	SuspensionRacer::Differential center_diff{};
	if (drive_torque == 0.0f) {
		return;
	}

	center_diff.factor = GetMWCarData()->DIFFERENTIAL[2];
	if (center_diff.factor > 0.0f) {
		center_diff.bias = GetMWCarData()->TORQUE_SPLIT;
		center_diff.angular_vel[0] = mTires[0]->GetAngularVelocity() + mTires[1]->GetAngularVelocity();
		center_diff.angular_vel[1] = mTires[2]->GetAngularVelocity() + mTires[3]->GetAngularVelocity();
		center_diff.has_traction[0] = mTires[0]->IsOnGround() || mTires[1]->IsOnGround();
		center_diff.has_traction[1] = mTires[2]->IsOnGround() || mTires[3]->IsOnGround();
		center_diff.CalcSplit(false);
	} else {
		center_diff.torque_split[0] = GetMWCarData()->TORQUE_SPLIT;
		center_diff.torque_split[1] = 1.0f - center_diff.torque_split[0];
	}

	for (unsigned int axle = 0; axle < 2; ++axle) {
		float axle_torque = drive_torque * center_diff.torque_split[axle];
		if (UMath::Abs(axle_torque) > FLOAT_EPSILON) {
			SuspensionRacer::Differential diff{};
			diff.bias = 0.5f;

			float fwd_slip = 0.0f;
			float lat_slip = 0.0f;

			float traction_control[2] = {1.0f, 1.0f};
			float traction_boost[2] = {1.0f, 1.0f};
			diff.factor = GetMWCarData()->DIFFERENTIAL[axle];

			for (unsigned int i = 0; i < 2; ++i) {
				unsigned int tire = axle * 2 + i;
				diff.angular_vel[i] = mTires[tire]->GetAngularVelocity();
				diff.has_traction[i] = mTires[tire]->IsOnGround();

				fwd_slip += center_diff.torque_split[axle] * mTires[tire]->GetCurrentSlip() * 0.5f;
				lat_slip += center_diff.torque_split[axle] * mTires[tire]->GetLateralSpeed() * 0.5f;
			}

			bool locked_diff = false;
			if ((mBurnOut.GetState() & 1) != 0) {
				traction_boost[1] = mBurnOut.GetTraction();
				diff.bias = mBurnOut.GetTraction() * 0.5f;
				locked_diff = true;
			} else if ((mBurnOut.GetState() & 2) != 0) {
				traction_boost[0] = mBurnOut.GetTraction();
				diff.bias = 1.0f - mBurnOut.GetTraction() * 0.5f;
				locked_diff = true;
			} else {
				float delta_lat_slip = lat_slip - state.local_vel.x;
				if (delta_lat_slip * state.steer_input > 0.0f && axle_torque * fwd_slip > 0.0f) {
					float delta_fwd_slip = fwd_slip - state.local_vel.z;
					float traction_control_limit = UMath::Ramp(delta_fwd_slip, 1.0f, 20.0f);

					if (traction_control_limit > 0.0f) {
						float traction_angle = UMath::Abs(state.steer_input * UMath::Atan2d(delta_lat_slip, UMath::Abs(delta_fwd_slip)));
						traction_control_limit *= UMath::Ramp(traction_angle, 1.0f, 16.0f);
						if (traction_control_limit > 0.0f) {
							if (delta_lat_slip > 0.0f) {
								traction_control[1] = 1.0f - traction_control_limit;
								traction_control[0] = 1.0f - traction_control_limit * 0.5f;
								traction_boost[0] = traction_control_limit + 1.0f;
							} else {
								traction_control[0] = 1.0f - traction_control_limit;
								traction_control[1] = 1.0f - traction_control_limit * 0.5f;
								traction_boost[1] = traction_control_limit + 1.0f;
							}
						}
					}
				}
			}
			diff.CalcSplit(locked_diff);

			for (unsigned int i = 0; i < 2; ++i) {
				unsigned int tire = axle * 2 + i;
				if (mTires[tire]->IsOnGround()) {
					mTires[tire]->ApplyDriveTorque((axle_torque * diff.torque_split[i] * traction_control[i]));
					mTires[tire]->ScaleTractionBoost(traction_boost[i]);
				}
			}
		}
	}
}

// Calculates artificial steering for when the car is touching a wall
// Credits: Brawltendo
void SuspensionRacer::DoWallSteer(State &state) {
	float wall = mSteering.WallNoseTurn;
	// nose turn is applied when the car is perpendicular to the wall
	// allows the player to easily turn their car away from the wall after a head-on crash without reversing
	if (wall != 0.0f && mNumWheelsOnGround > 2 && state.gas_input > 0.0f) {
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

	wall = mSteering.WallSideTurn;
	// side turn is only applied when in reverse and if touching a wall parallel to the car
	// it helps the player move their car away from a wall when backing up
	if (wall * state.steer_input * state.gas_input > 0.0f && mNumWheelsOnGround > 2 && !state.gear) {
		float dW = -state.steer_input * state.gas_input * 0.125f;
		dW *= UMath::Abs(wall);

		UMath::Vector3 chg = {};
		chg.y = dW;
		UMath::Rotate(chg, state.matrix, chg);
		UMath::Add(state.angular_vel, chg, chg);
		mRB->SetAngularVelocity(&chg);
	}
}

// Credits: Brawltendo
void SuspensionRacer::ComputeAckerman(const float steering, const State &state, UMath::Vector4 *left, UMath::Vector4 *right) const {
	int going_right = true;
	float wheelbase = GetMWCarData()->WHEEL_BASE;
	float wheeltrack = GetMWCarData()->TRACK_WIDTH.Front;
	float steer_inside = ANGLE2RAD(steering);

	// clamp steering angle <= 180 degrees
	if (steer_inside > (float)M_PI)
		steer_inside -= (float)(std::numbers::pi*2);

	// negative steering angle indicates a left turn
	if (steer_inside < 0.0f) {
		going_right = false;
		steer_inside = -steer_inside;
	}

	// Ackermann steering geometry causes the outside wheel to have a smaller turning angle than the inside wheel
	// this is determined by the distance of the wheel to the center of the rear axle
	// this equation is a modified version of 1/tan(L/(R+T/2)), where L is the wheelbase, R is the steering radius, and T is the track width
	float steer_outside = (wheelbase * steer_inside) / (wheeltrack * steer_inside + wheelbase);
	float steer0, steer1; // 0 - right, 1 - left
	if (going_right) {
		steer0 = steer_inside;
		steer1 = steer_outside;
	} else {
		steer1 = -steer_inside;
		steer0 = -steer_outside;
	}

	float ca, sa;
	// calculate forward vector for front wheels
	UMath::Vector3 r;
	ca = cosf(steer0);
	sa = sinf(steer0);
	r.z = ca;
	r.x = sa;
	r.y = 0.0f;
	UMath::Rotate(r, state.matrix, r);
	*right = UMath::Vector4Make(r, steer0);

	UMath::Vector3 l;
	ca = cosf(steer1);
	sa = sinf(steer1);
	l.z = ca;
	l.x = sa;
	l.y = 0.0f;
	UMath::Rotate(l, state.matrix, l);
	*left = UMath::Vector4Make(l, steer1);
}

// Credits: Brawltendo
float SuspensionRacer::DoAISteering(State &state) {
	mSteering.Maximum = 45.0f;
	if (state.driver_style != STYLE_DRAG) {
		mSteering.Maximum = GetMWCarData()->STEERING.GetValue(UNDERCOVER_SteeringAtValue) * 45.0f;
	}

	return DEG2ANGLE(mSteering.Maximum * state.steer_input);
}

// Credits: Brawltendo
float SuspensionRacer::CalculateMaxSteering(State &state, ISteeringWheel::SteeringType steer_type) {
	const float steer_input = state.steer_input;

	// max possible steering output scales with the car's forward speed
	float max_steering = SteeringRangeTable.GetValue(state.local_vel.z);
	// there are 2 racing wheel input types, one scales with speed and the other doesn't
	if (steer_type == ISteeringWheel::kWheelSpeedSensitive) {
		max_steering = SteeringWheelRangeTable.GetValue(state.local_vel.z);
	} else if (steer_type == ISteeringWheel::kWheelSpeedInsensitive) {
		return MAX_STEERING;
	}

	float tbcoeff = 1.0f - (state.gas_input + 1.0f - (state.brake_input + state.ebrake_input) * 0.5f) * 0.5f;
	max_steering *= BrakeSteeringRangeMultiplier * tbcoeff * SteeringSpeedTable.GetValue(state.local_vel.z) + 1.0f;
	max_steering *= SteeringRangeCoeffTable.GetValue(std::abs(mSteering.InputAverage.GetValue()));

	const Physics::Tunings *tunings = GetVehicle()->GetTunings();
	// todo
	//if (tunings) {
	//	max_steering *= tunings->Value[Physics::Tunings::STEERING] * Tweak_TuningSteering_Ratio + 1.0f;
	//}

	float collision_coeff;
	// reduce steering range after collisions
	if (mSteering.CollisionTimer > 0.0f) {
		float secs = PostCollisionSteerReductionDuration - mSteering.CollisionTimer;
		if (secs < PostCollisionSteerReductionDuration && secs > 0.0f) {
			float speed_coeff = std::min(1.0f, state.local_vel.z / (MPH2MPS(170.0f) * 0.7f));
			speed_coeff = 1.0f - speed_coeff;
			collision_coeff = PostCollisionSteerReductionTable.GetValue(secs);
			max_steering *= speed_coeff * (1.0f - collision_coeff) + collision_coeff;
		}
	}

	float yaw_left = ANGLE2DEG(mTires[2]->GetSlipAngle());
	float yaw_right = ANGLE2DEG(mTires[3]->GetSlipAngle());
	// clamp the max steering range to [rear slip, MAX_STEERING] when countersteering
	if (steer_input > 0.0f && yaw_right > 0.0f) {
		max_steering = UMath::Max(max_steering, yaw_right);
		max_steering = UMath::Min(max_steering, MAX_STEERING);
	} else if (steer_input < 0.0f && yaw_left < 0.0f) {
		max_steering = UMath::Max(max_steering, -yaw_left);
		max_steering = UMath::Min(max_steering, MAX_STEERING);
	} else if (std::abs(mSteering.InputAverage.GetValue()) >= HardTurnSteeringThreshold) {
		max_steering = std::max(max_steering, mSteering.LastMaximum - state.time * HardTurnTightenSpeed);
	}

	max_steering = std::min(max_steering, MAX_STEERING);
	mSteering.LastMaximum = max_steering;
	return max_steering;
}

// Credits: Brawltendo
float SuspensionRacer::CalculateSteeringSpeed(State &state) {
	// get a rough approximation of how fast the player is steering
	// this ends up creating a bit of a difference in how fast you can actually steer on a controller under normal circumstances
	// using a keyboard will always give you the fastest steering possible
	float steer_input_speed = (state.steer_input - mSteering.LastInput) / state.time;

	mSteering.InputSpeedCoeffAverage.Record(SteeringInputSpeedCoeffTable.GetValue(std::abs(steer_input_speed)), GetSimTimeNew());

	// steering speed scales with vehicle forward speed
	float steer_speed = 180.0f;
	steer_speed *= (SteeringSpeedTable.GetValue(state.local_vel.z));
	steer_speed *= mSteering.InputSpeedCoeffAverage.GetValue();

	float steer_input = std::abs(mSteering.InputAverage.GetValue());
	return steer_speed * SteeringInputCoeffTable.GetValue(steer_input);
}

// Credits: Brawltendo
float SuspensionRacer::DoHumanSteering(State &state) {
	float steer_input = state.steer_input;
	float steer = mSteering.Previous;

	if (steer >= 180.0f) {
		steer -= 360.0f;
	}

	float steering_coeff = GetMWCarData()->STEERING.GetValue(UNDERCOVER_SteeringAtValue);
	ISteeringWheel::SteeringType steer_type = ISteeringWheel::kGamePad;

	IPlayer *player = GetOwner()->GetPlayer();
	if (player) {
		ISteeringWheel *device = player->GetSteeringDevice();

		if (device && device->IsConnected()) {
			steer_type = device->GetSteeringType();
		}
	}

	float max_steering;
	float newsteer = steer_input * CalculateMaxSteering(state, steer_type) * steering_coeff;
	newsteer = bClamp(newsteer, -45.0f, 45.0f);

	if (steer_type == ISteeringWheel::kGamePad) {
		int steer_remapping = SteerInputRemapping;
		steer_input = SteerInputRemapTables[steer_remapping].GetValue(steer_input);
		float steering_speed = (CalculateSteeringSpeed(state) * steering_coeff) * state.time;
		float max_diff = steer + steering_speed;
		newsteer = bClamp(newsteer, steer - steering_speed, max_diff);
		// this is absolutely pointless but it's part of the steering calculation for whatever reason
		if (std::abs(newsteer) < 0.0f) {
			newsteer = 0.0f;
		}
	}
	mSteering.LastInput = steer_input;
	mSteering.Previous = newsteer;

	// speedbreaker increases the current steering angle beyond the normal maximum
	// this change is instant, so the visual steering angle while using speedbreaker doesn't accurately represent this
	// instead it interpolates to this value so it looks nicer
	if (mGameBreaker > 0.0f) {
		newsteer = UMath::Lerp(newsteer, state.steer_input * Tweak_GameBreakerMaxSteer, mGameBreaker);
	}

	mSteering.InputAverage.Record(mSteering.LastInput, GetSimTimeNew());
	return DEG2ANGLE(newsteer);
}

// Credits: Brawltendo
void SuspensionRacer::DoSteering(State &state, UMath::Vector3 &right, UMath::Vector3 &left) {
	float truesteer;
	UMath::Vector4 r4;
	UMath::Vector4 l4;

	if (mHumanAI && mHumanAI->IsPlayerSteering()) {
		truesteer = DoHumanSteering(state);
	} else {
		truesteer = DoAISteering(state);
	}

	ComputeAckerman(truesteer, state, &l4, &r4);
	right = Vector4To3(r4);
	left = Vector4To3(l4);
	mSteering.Wheels[0] = l4.w;
	mSteering.Wheels[1] = r4.w;
	DoWallSteer(state);
}

// Credits: Brawltendo
float SuspensionRacer::CalcYawControlLimit(float speed) const {
	if (mTransmission) {
		float maxspeed = mTransmission->GetMaxSpeedometer();
		if (maxspeed <= 0.0f) {
			return 0.0f;
		}
		float percent = UMath::Min(UMath::Abs(speed) / maxspeed, 1.0f);

		// todo!! these are different in uc!
		unsigned int numunits = UNDERCOVER_YawControl.size();
		if (numunits > 1) {
			float ratio = (numunits - 1) * percent;
			unsigned int index1 = static_cast<unsigned int>(ratio);
			ratio -= index1;
			unsigned int index2 = UMath::Min(numunits - 1, index1 + 1);
			float a = UNDERCOVER_YawControl[index1];
			float b = UNDERCOVER_YawControl[index2];
			return a + (b - a) * ratio;
		}
	}
	return UNDERCOVER_YawControl[0];
}

static float LowSpeedSpeed = 0.0f;
static float HighSpeedSpeed = 30.0f;
static float MaxYawBonus = 0.35f;
static float LowSpeedYawBoost = 0.0f;
static float HighSpeedYawBoost = 1.0f;
static float YawEBrakeThreshold = 0.5f;
static float YawAngleThreshold = 20.0f;

// Credits: Brawltendo
float YawFrictionBoost(float yaw, float ebrake, float speed, float yawcontrol, float grade) {
	yaw = std::abs(yaw);
	float retval = 1.0f;
	retval += std::abs(grade);
	if (ebrake > YawEBrakeThreshold && yaw < DEG2RAD(YawAngleThreshold))
		return retval;

	float speed_factor = (speed - LowSpeedSpeed) / (HighSpeedSpeed - LowSpeedSpeed);
	float boost = LowSpeedYawBoost + (HighSpeedYawBoost - LowSpeedYawBoost) * speed_factor;
	float bonus = yaw * yawcontrol * boost;
	if (bonus > MaxYawBonus)
		bonus = MaxYawBonus;
	return retval + bonus;
}

GraphEntry<float> DriftStabilizerData[] = {{0.0f, 0.0f},        {0.2617994f, 0.1f},  {0.52359879f, 0.45f}, {0.78539819f, 0.85f},
										   {1.0471976f, 0.95f}, {1.5533431f, 1.15f}, {1.5707964f, 0.0f}};
float DriftRearFrictionData[] = {1.1f, 0.95f, 0.87f, 0.77f, 0.67f, 0.6f, 0.51f, 0.43f, 0.37f, 0.34f};
tGraph<float> DriftStabilizerTable(DriftStabilizerData, 7);
Table DriftRearFrictionTable(DriftRearFrictionData, 10, 0.0f, 1.0f);
static const float DriftRotationalStabalizer = 4.0f;
static const float DriftYawAngularVelCoeff = 0.5f;
static const float Tweak_MinDriftSpeedMPH = 30.0f;
static const float Tweak_DriftSlipAngle = 12.0f;
static const float Tweak_DriftEnterSpeed = 30.0f;
static const float Tweak_DriftExitSpeed = 30.0f;
static const float Tweak_DriftCounterSteer = 4.0f;
static const float Tweak_GameBreakerDriftRechargePerSec = 0.5f;
static const float Tweak_GameBreakerDriftRechargeMinSpeed = 35.0f;
static const float Tweak_GameBreakerDriftRechargeYaw = 30.0f;
static const bool Tweak_AlwaysDrift = false;
static const bool DoDriftPhysics = true;

// Credits: Brawltendo
void SuspensionRacer::DoDrifting(const State &state) {
	if (mDrift.State && ((state.flags & State::IS_STAGING) || state.driver_style == STYLE_DRAG)) {
		mDrift.Reset();
		return;
	}

	float drift_change = 0.0f;
	switch (mDrift.State) {
		case SuspensionRacer::Drift::D_IN:
		case SuspensionRacer::Drift::D_ENTER:
			// the drift value will increment by (dT * 8) when entering and holding a drift
			drift_change = 8.0f;
			break;
		case SuspensionRacer::Drift::D_OUT:
		case SuspensionRacer::Drift::D_EXIT:
			// the drift value will decrement by (dT * 2) when not drifting or exiting a drift
			drift_change = -2.0f;
			break;
		default:
			break;
	}

	mDrift.Value += drift_change * state.time;
	// clamp the drift value between 0 and 1
	if (mDrift.Value <= 0.0f) {
		mDrift.State = SuspensionRacer::Drift::D_OUT;
		mDrift.Value = 0.0f;
	} else if (mDrift.Value >= 1.0f) {
		mDrift.State = SuspensionRacer::Drift::D_IN;
		mDrift.Value = 1.0f;
	}

	if (mDrift.State > SuspensionRacer::Drift::D_ENTER) {
		float avg_steer = mSteering.InputAverage.GetValue();
		if ((state.local_angular_vel.y * state.slipangle) < 0.0f && UMath::Abs(state.slipangle) <= 0.25f &&
			state.speed > MPH2MPS(Tweak_MinDriftSpeedMPH) && (avg_steer * state.slipangle) <= 0.0f &&
			UMath::Abs(state.slipangle) > DEG2ANGLE(Tweak_DriftSlipAngle)) {
			mDrift.State = SuspensionRacer::Drift::D_IN;
		} else if (state.gas_input * state.steer_input * state.slipangle > DEG2ANGLE(Tweak_DriftSlipAngle) &&
				   state.speed > MPH2MPS(Tweak_MinDriftSpeedMPH)) {
			mDrift.State = SuspensionRacer::Drift::D_IN;
		} else if (state.gas_input * UMath::Abs(state.slipangle) > DEG2ANGLE(Tweak_DriftSlipAngle)) {
			mDrift.State = SuspensionRacer::Drift::D_ENTER;
		} else {
			mDrift.State = SuspensionRacer::Drift::D_EXIT;
		}
	} else if (state.speed > MPH2MPS(Tweak_DriftEnterSpeed) &&
			   (state.ebrake_input > 0.5f || UMath::Abs(state.slipangle) > DEG2ANGLE(Tweak_DriftSlipAngle))) {
		mDrift.State = SuspensionRacer::Drift::D_ENTER;
	}

	if (mDrift.Value > 0.0f) {
		float yaw = ANGLE2RAD(state.slipangle);
		float ang_vel = state.local_angular_vel.y;

		// charge speedbreaker if not in use and drifting is detected
		if (mGameBreaker <= 0.0f && state.speed > MPH2MPS(Tweak_GameBreakerDriftRechargeMinSpeed) &&
			UMath::Abs(yaw) > DEG2RAD(Tweak_GameBreakerDriftRechargeYaw)) {
			IPlayer *player = GetOwner()->GetPlayer();
			if (player) {
				player->ChargeGameBreaker(state.time * Tweak_GameBreakerDriftRechargePerSec * mDrift.Value);
			}
		}

		// apply yaw damping torque
		if ((yaw * ang_vel) < 0.0f && mNumWheelsOnGround >= 2) {
			float yaw_coef = DriftStabilizerTable.GetValue(UMath::Abs(yaw)) * DriftRotationalStabalizer;
			UMath::Vector3 moment;

			UMath::Scale(state.GetUpVector(), (mDrift.Value * -ang_vel) * yaw_coef * state.inertia.y, moment);
			mRB->ResolveTorque(&moment);
		}

		// detect counter steering
		float countersteer = 0.0f;
		if (state.steer_input * yaw > 0.0f) {
			countersteer = UMath::Abs(state.steer_input);
		}

		float yawangularvel_coeff = DriftYawAngularVelCoeff;
		float driftcoeff = UMath::Abs(yaw) * yawangularvel_coeff + countersteer * Tweak_DriftCounterSteer + UMath::Abs(ang_vel) * yawangularvel_coeff;
		float driftmult_rear = DriftRearFrictionTable.GetValue(driftcoeff * mDrift.Value);
		mTires[2]->SetDriftFriction(driftmult_rear);
		mTires[3]->SetDriftFriction(driftmult_rear);
	}
}

float EBrakeYawControlMin = 0.5f;
float EBrakeYawControlOnSpeed = 1.0f;
float EBrakeYawControlOffSpeed = 20.0f;
float EBrake180Yaw = 0.3f;
float EBrake180Speed = 80.0f;
static const float Tweak_GameBreakerGripIncrease = 0.75f;
static const float Tweak_StagingTraction = 0.25f;
static const float Tweak_DragYawControl = 0.1f;
static const float Tweak_StabilityControl = 2.5f;
static const float Tweak_BlownTireEbrake = 0.0f;
static const float Tweak_BlownTireBrake = 1.0f;
static const float Tweak_BlownTireTraction = 0.3f;

// Credits: Brawltendo
void SuspensionRacer::TuneWheelParams(State &state) {
	float ebrake = state.ebrake_input;
	float t = state.time;
	float speedmph = MPS2MPH(state.local_vel.z);
	float car_yaw = ANGLE2RAD(state.slipangle);
	float yawcontrol = mSteering.YawControl;

	// engaging the handbrake decreases steering yaw control
	if (ebrake >= 0.5f) {
		yawcontrol -= EBrakeYawControlOffSpeed * t;
		if (yawcontrol < EBrakeYawControlMin) {
			yawcontrol = EBrakeYawControlMin;
		}
	} else {
		yawcontrol += EBrakeYawControlOnSpeed * t;
		if (yawcontrol > 1.0f) {
			yawcontrol = 1.0f;
		}
	}
	mSteering.YawControl = yawcontrol;

	float brake_biased[2] = {state.brake_input, state.brake_input};
	yawcontrol *= (1.0f - mDrift.Value); // pointless parentheses for matching purposes
	const Physics::Tunings *tunings = GetVehicle()->GetTunings();
	//if (tunings) {
	//	// brake tuning adjusts the brake bias
	//	brake_biased[0] += brake_biased[0] * tunings->Value[Physics::Tunings::BRAKES] * 0.5f;
	//	brake_biased[1] -= brake_biased[1] * tunings->Value[Physics::Tunings::BRAKES] * 0.5f;
	//}
	float suspension_yaw_control_limit = CalcYawControlLimit(state.speed);
	IPlayer *player = GetOwner()->GetPlayer();
	if (state.driver_style == STYLE_DRAG) {
		suspension_yaw_control_limit = 0.1f;
	} else if (player) {
		PlayerSettings *settings = player->GetSettings();
		if (settings) {
			// increase yaw control limit when stability control is off (unused by normal means)
			if (!settings->Handling) {
				suspension_yaw_control_limit += 2.5f;
			}
		}
	}

	float max_slip = 0.0f;
	int max_slip_wheel = 0;
	for (int i = 0; i < 4; ++i) {
		float lateral_boost = 1.0f;

		// at speeds below 10 mph, 5% of the current speed in mph is applied as the brake scale for driven wheels
		if (state.gas_input > 0.8f && state.brake_input > 0.5f && UMath::Abs(speedmph) < 10.0f && IsDriveWheel(i)) {
			mTires[i]->SetBrake(UMath::Abs(speedmph) * 0.05f);
		} else {
			mTires[i]->SetBrake(brake_biased[i >> 1]);
		}

		// handbrake only applies to the rear wheels
		if (IsRear(i)) {
			float b = ebrake;
			// increase handbrake multiplier when a hard handbrake turn is detected
			if (ebrake > 0.2f && car_yaw > EBrake180Yaw && speedmph < EBrake180Speed) {
				b += 0.5f;
			}
			mTires[i]->SetEBrake(b);
		} else {
			mTires[i]->SetEBrake(0.0f);
		}

		float friction_boost = 1.0f;
		// rear wheels get extra boost according to the yaw control
		if (IsRear(i)) {
			float grade = state.GetForwardVector().y;
			float boost = YawFrictionBoost(car_yaw, mTires[i]->GetEBrake(), state.speed, suspension_yaw_control_limit, grade) - 1.0f;
			friction_boost = yawcontrol * boost + 1.0f;
		}

		// speedbreaker increases front tire friction relative to the absolute steering input
		if (mGameBreaker > 0.0f && IsFront(i)) {
			float over_boost = mGameBreaker * UMath::Abs(state.steer_input) * 0.75f + 1.0f;
			lateral_boost = over_boost;
			friction_boost *= over_boost;
		}
		mTires[i]->ScaleTractionBoost(friction_boost * fHackFrictionBoost);
		mTires[i]->SetLateralBoost(lateral_boost * fHackLateralBoost);

		//if (tunings) {
		//	UMath::Vector2 circle;
		//	circle.x = tunings->Value[Physics::Tunings::HANDLING] * 0.2f + 1.0f;
		//	circle.y = 1.0f - tunings->Value[Physics::Tunings::HANDLING] * 0.2f;
		//	mTires[i]->SetTractionCircle(circle);
		//}
		// traction is increased by perfect shifts in drag races and also by engaging the nitrous
		mTires[i]->ScaleTractionBoost(state.nos_boost * state.shift_boost);

		// popped tires are permanently braking and have reduced traction
		if ((1 << i) & state.blown_tires) {
			mTires[i]->SetEBrake(0.0f);
			mTires[i]->SetBrake(1.0f);
			mTires[i]->ScaleTractionBoost(0.3f);
		}

		// find the highest slip of all tires for the burnout/fishtailing state
		if (mTires[i]->GetCurrentSlip() > max_slip) {
			max_slip = mTires[i]->GetCurrentSlip();
			max_slip_wheel = i;
		}
	}

	// burnout state only applies when in first gear and the throttle is fully pressed outside of drag events
	if (state.driver_style != STYLE_DRAG && state.gear == G_FIRST && state.gas_input >= 1.0f) {
		mBurnOut.Update(state.time, MPS2MPH(state.local_vel.z), max_slip, max_slip_wheel, state.slipangle);
	} else {
		mBurnOut.Reset();
	}

	// lower traction for all wheels when staging
	if (state.flags & State::IS_STAGING) {
		for (int i = 0; i < 4; ++i) {
			mTires[i]->ScaleTractionBoost(0.25f);
		}
	}

	DoDrifting(state);
}

static const float Tweak_DragYawSpeed = 1.6f;

// Credits: Brawltendo
void SuspensionRacer::DoWheelForces(State &state) {
	const float dT = state.time;

	UMath::Vector3 steerR;
	UMath::Vector3 steerL;
	DoSteering(state, steerR, steerL);
	TuneWheelParams(state);

	unsigned int wheelsOnGround = 0;
	float maxDelta = 0.0f;

	const UMath::Vector3 &vFwd = state.GetForwardVector();
	const UMath::Vector3 &vUp = state.GetUpVector();

	const float mass = state.mass;

	float ride_extra = 0.0f;
	const Physics::Tunings *tunings = GetVehicle()->GetTunings();
	// todo
	//if (tunings) {
	//	ride_extra = tunings->Value[Physics::Tunings::RIDEHEIGHT];
	//}

	float time = GetSimTimeNew();
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
		shock_specs[i] = LBIN2NM(GetMWCarData()->SHOCK_STIFFNESS.At(i).GetValue(UNDERCOVER_SuspensionAtValue));
		shock_ext_specs[i] = LBIN2NM(GetMWCarData()->SHOCK_EXT_STIFFNESS.At(i).GetValue(UNDERCOVER_SuspensionAtValue));
		shock_valving[i] = INCH2METERS(GetMWCarData()->SHOCK_VALVING.At(i));
		shock_digression[i] = 1.0f - GetMWCarData()->SHOCK_DIGRESSION.At(i);
		spring_specs[i] = LBIN2NM(GetMWCarData()->SPRING_STIFFNESS.At(i));
		sway_specs[i] = LBIN2NM(GetMWCarData()->SWAYBAR_STIFFNESS.At(i).GetValue(UNDERCOVER_SuspensionAtValue));
		travel_specs[i] = INCH2METERS(GetMWCarData()->TRAVEL.At(i));
		rideheight_specs[i] = INCH2METERS(GetMWCarData()->RIDE_HEIGHT.At(i) + ride_extra);
		progression[i] = GetMWCarData()->SPRING_PROGRESSION.At(i);
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
			wheel.SetBottomOutTime(time);
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

			if (damp > GetMWCarData()->SHOCK_BLOWOUT * 9.81f * mass) {
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
			max_traction *= fHackMaxTraction;
			lateralForce = (UMath::Vector3)(lateralNormal * UMath::Clamp(traction_force, -max_traction, max_traction));
			lateralForce *= fHackLateralForce;

			UMath::Vector3 force;
			UMath::UnitCross(lateralNormal, groundNormal, driveForce);
			UMath::Scale(driveForce, wheel.GetLongitudeForce(), driveForce);
			UMath::Add(lateralForce, driveForce, force);
			UMath::Add(force, verticalForce, force);

			wheel.SetForce(force);
			resolve = true;
		} else {
			wheel.SetForce({0,0,0});
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
		UMath::Vector3 total_torque = {0,0,0};
		UMath::Vector3 total_force = {0,0,0};

		for (unsigned int i = 0; i < 4; ++i) {
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

		// mw m3 test:
		// yaw speed is 0.26
		// matrix y -0.00363379973 0.9999672771 -0.007230669726
		// total_torque -4137.378418 1823.675293 0.05406260118
		// or maybe 297.5856934 10732.44727 2017.319824
		// dot result is... 1.94?

		// expected for Resolve is:
		// total_force -87.99267578 15717.22852 -149.1251526
		// total_torque -444.7327271 -11.27389526 -925.8071289

		float yaw = UMath::Dot((UMath::Vector3)state.matrix.y, total_torque);
		float counter_yaw = yaw * GetMWCarData()->YAW_SPEED;
		if (state.driver_style == STYLE_DRAG) {
			counter_yaw *= Tweak_DragYawSpeed;
		}
		UMath::ScaleAdd((UMath::Vector3)state.matrix.y, counter_yaw - yaw, total_torque, total_torque);
		mRB->Resolve(&total_force, &total_torque);
	}

	if (maxDelta > 0.0f) {
		for (int i = 0; i < 4; ++i) {
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

SuspensionRacer::State LastChassisState;
void SuspensionRacer::ComputeState(float dT, State &state) {
	IRigidBody *irb = GetOwner()->GetRigidBody();
	state.time = dT;
	state.flags = 0;
	state.collider = mCollisionBody->GetWCollider();
	irb->GetInertiaTensor(&state.inertia);
	irb->GetDimension(&state.dimension);

	state.matrix = *irb->GetTransform();
	state.matrix.p = *irb->GetPosition();
	state.matrix.pw = 1.0f;

	state.local_vel = *irb->GetLinearVelocity();
	state.linear_vel = state.local_vel;
	irb->ConvertWorldToLocal(&state.local_vel, false);
	state.angular_vel = *irb->GetAngularVelocity();
	state.local_angular_vel = state.angular_vel;
	irb->ConvertWorldToLocal(&state.local_angular_vel, false);

	state.speed = UMath::Length(state.linear_vel);
	if (state.local_vel.z < 1.0f) {
		state.slipangle = 0.0f;
	} else {
		state.slipangle = UMath::Atan2a(state.local_vel.x, state.local_vel.z);
	}

	state.gas_input = UMath::Clamp(mInput->GetControlGas(), 0.0f, 1.0f);
	state.brake_input = UMath::Clamp(mInput->GetControlBrake(), 0.0f, 1.0f);
	state.ebrake_input = mInput->GetControlHandBrake();
	state.steer_input = UMath::Clamp(mInput->GetControlSteering(), -1.0f, 1.0f);

	//state.cog = *irb->GetCenterOfGravity();
	state.cog = {0,0,0};
	state.ground_effect = mNumWheelsOnGround * 0.25f;
	state.mass = irb->GetMass();
	state.driver_style = GetVehicle()->GetDriverStyle();
	state.driver_class = GetVehicle()->GetDriverClass();

	if (GetVehicle()->IsStaging()) {
		state.flags |= State::IS_STAGING;
	}

	if (mEngine) {
		state.nos_boost = mEngine->GetNOSBoost();
	} else {
		state.nos_boost = 1.0f;
	}
	if (mTransmission) {
		state.gear = mTransmission->GetGear();
	} else {
		state.gear = G_NEUTRAL;
	}

	state.shift_boost = 1.0f;

	if (mEngineDamage && mEngineDamage->IsBlown() || GetVehicle()->IsDestroyed()) {
		state.brake_input = 1.0f;
		state.gas_input = 0.0f;
		state.ebrake_input = 1.0f;
	}

	UMath::Rotate(state.cog, state.matrix, state.world_cog);

	state.blown_tires = 0;
	if (mSpikeDamage) {
		for (unsigned int i = 0; i < 4; ++i) {
			if (mSpikeDamage->GetTireDamage(i) == TIRE_DAMAGE_BLOWN) {
				state.blown_tires |= (1 << i);
			}
		}
	}

	if (GetVehicle()->IsDestroyed()) {
		state.flags |= State::IS_DESTROYED;
	}

	LastChassisState = state;
}

static const float Tweak_SteerDragReduction = 0.15f;
static const float Tweak_GameBreakerSteerDragReduction = 0.15f;
static const float Tweak_GameBreakerExtraGs = -2.0f;
static const float TweakGameBreakerRampOutPhysicsTime = 1.0f / 3.0f;
static const float Tweak_DragAeroMult = 1.5f;

void SuspensionRacer::OnTaskSimulate(float dT) {
	gCurrentCarInfo = mCarInfo.GetLayout();

	//FUNCTION_LOG("SuspensionRacer::OnTaskSimulate");

	if (!mCollisionBody || !mRB) {
		WriteLog("OnTaskSimulate early exit");
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
	if ((state.flags & State::IS_STAGING) == 0) {
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

void MWWheel::UpdateSurface(const Attrib::Collection* surface) {
	if (mSurfaceStick > 0.0) {
		if (surface == mSurface.mCollection) {
			mSurfaceStick = mSurface.GetLayout()->STICK;
		}
	}
	else {
		Attrib::Instance::Change(&mSurface, surface);
		mSurfaceStick = mSurface.GetLayout()->STICK;
	}
}

bool MWWheel::InitPosition(ICollisionBody* cb, IRigidBody *rb, double maxcompression) {
	//FUNCTION_LOG("Wheel::InitPosition");
	auto mat = *rb->GetTransform();
	mat.p = *rb->GetPosition();
	UMath::Vector3 dim;
	rb->GetDimension(&dim);
	return UpdatePosition(*rb->GetAngularVelocity(), *rb->GetLinearVelocity(), mat, {0,0,0}, 0.0, maxcompression, false, cb->GetWCollider(), dim.y * 2.0);
}

bool MWWheel::UpdatePosition(const UMath::Vector3 &body_av, const UMath::Vector3 &body_lv,
						   const UMath::Matrix4 &body_matrix, const UMath::Vector3 &cog,
						   float dT, float wheel_radius, bool usecache, const WCollider *collider, float vehicle_height) {
	//FUNCTION_LOG("Wheel::UpdatePosition");
	UMath::Rotate(mLocalArm, body_matrix, mWorldArm);
	UMath::Add(mWorldArm, (UMath::Vector3)body_matrix.p, mPosition);

	UMath::Vector3 pVel;
	UMath::Vector3 pos;
	UMath::Sub(mWorldArm, cog, pos);
	UMath::Cross(body_av, pos, pVel);
	UMath::Add(pVel, body_lv, pVel);
	SetVelocity(pVel);

	UMath::Add(mWorldArm, (UMath::Vector3)body_matrix.p, mPosition);

	float tolerance = UMath::Max(-pVel.y * dT, 0.0f) + wheel_radius + UMath::Lengthxz(pVel) * dT;
	float prev = vehicle_height * 0.5f;
	mWorldPos.SetTolerance(UMath::Min(tolerance, prev));

	bool result = WWorldPos::Update(&mWorldPos, &mPosition, &mNormal, IsOnGround() && usecache, collider, true);
	mNormal.w = ((mWorldPos.fPt0.x - mPosition.x) * mNormal.x) + (((mWorldPos.fPt0.z - mPosition.z) * mNormal.z) + ((mWorldPos.fPt0.y - mPosition.y) * mNormal.y));
	UpdateSurface(mWorldPos.pSurface);
	return result;
}

Meters SuspensionRacer::GetRideHeight(unsigned int idx) const {
	return INCH2METERS(GetMWCarData()->RIDE_HEIGHT.At(idx / 2));
}

void SuspensionRacer::MatchSpeed(float speed) {
	for (int i = 0; i < 4; ++i) {
		mTires[i]->MatchSpeed(speed);
	}
	mBurnOut.Reset();
	mDrift.Reset();
}

void SuspensionRacer::dtor(char a2) {
	FUNCTION_LOG("SuspensionRacer::dtor");

	for (int i = 0; i < 4; ++i) {
		WriteLog("delete mTires[i]");
		delete mTires[i];
	}
	if ((a2 & 1) != 0) {
		WriteLog("gFastMem.Free");
		gFastMem.Free(this, sizeof(SuspensionRacer), nullptr);
	}

	WriteLog("SuspensionRacer::dtor finished");
}

float SuspensionRacer::CalculateUndersteerFactor() {
	FUNCTION_LOG("SuspensionRacer::CalculateUndersteerFactor");
	float magnitude = 0.0f;
	float slip_avg = (GetIChassis()->GetWheelSkid(0) + GetIChassis()->GetWheelSkid(1)) / 2.0f;
	float steer = (GetIChassis()->GetWheelSteer(0) + GetIChassis()->GetWheelSteer(1)) / 2.0f;
	float speed = GetOwner()->GetRigidBody()->GetSpeed();
	if ((GetVehicle()->GetSpeed() > 0.0f) && (speed > 1.0f) && (steer * slip_avg < 0.0f)) {
		magnitude = UMath::Abs(slip_avg) / speed;
	}
	return UMath::Min(magnitude, 1.0f);
}

float SuspensionRacer::CalculateOversteerFactor() {
	FUNCTION_LOG("SuspensionRacer::CalculateOversteerFactor");
	float speed = GetOwner()->GetRigidBody()->GetSpeed();
	float magnitude = 0.0f;
	if ((this->GetVehicle()->GetSpeed() > 0.0f) && (speed > 1.0f)) {
		magnitude = UMath::Abs((GetIChassis()->GetWheelSkid(3) + GetIChassis()->GetWheelSkid(2)) * 0.5f) / speed;
	}
	return UMath::Min(magnitude, 1.0f);
}

void SuspensionRacer::Reset() {
	FUNCTION_LOG("SuspensionRacer::Reset");

	ISimable *owner = GetOwner();
	IRigidBody *rigidBody = owner->GetRigidBody();
	UMath::Vector3 vUp = *rigidBody->GetUpVector();

	unsigned int numonground = 0;
	this->mGameBreaker = 0.0f;
	for (int i = 0; i < 4; ++i) {
		SuspensionRacer::Tire &wheel = GetWheel(i);
		wheel.Reset();
		if (wheel.InitPosition(mCollisionBody, rigidBody, wheel.GetRadius())) {
			float upness = UMath::Clamp(UMath::Dot(UMath::Vector4To3(wheel.GetNormal()), vUp), 0.0f, 1.0f);
			float newCompression = wheel.GetNormal().w + GetRideHeight(i) * upness;
			if (newCompression < 0.0f) {
				newCompression = 0.0f;
			}
			wheel.SetCompression(newCompression);
			if (newCompression > 0.0f) {
				numonground++;
			}
		}
	}
	this->mNumWheelsOnGround = numonground;
	mSteering.Reset();
	mBurnOut.Reset();
	mDrift.Reset();
}