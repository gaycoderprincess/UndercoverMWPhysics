float UNDERCOVER_BrakesAtValue = 0.0;
float UNDERCOVER_StaticGripAtValue = 0.0;
float UNDERCOVER_RollCenterAtValue = 0.0;
float UNDERCOVER_AeroCGAtValue = 0.0;
float UNDERCOVER_AeroCoeffAtValue = 0.0;
float UNDERCOVER_SuspensionAtValue = 0.0;
float UNDERCOVER_SteeringAtValue = 0.0;

class MWWheel {
public:
	WWorldPos mWorldPos;
	UMath::Vector4 mNormal;
	UMath::Vector3 mPosition;
	unsigned int mFlags;
	UMath::Vector3 mForce;
	float mAirTime;
	UMath::Vector3 mLocalArm;
	float mCompression;
	UMath::Vector3 mWorldArm;
	int pad;
	UMath::Vector3 mVelocity;
	int pad2;
	SimSurface mSurface;
	float mSurfaceStick;
	UMath::Vector4 mIntegral;

	MWWheel(unsigned int flags) {
		FUNCTION_LOG("Wheel::Wheel");
		memset(this,0,sizeof(*this));
		mFlags = flags;
		Reset();
	}

	void Reset() {
		FUNCTION_LOG("Wheel::Reset");
		mSurfaceStick = 0.0;
		mIntegral.x = 0.0;
		mIntegral.y = 0.0;
		mIntegral.z = 0.0;
		mIntegral.w = 0.0;
		mAirTime = 0.0;
		mVelocity.x = 0.0;
		mVelocity.y = 0.0;
		mVelocity.z = 0.0;
		mCompression = 0.0;
		mNormal.x = 0.0;
		mNormal.y = 0.0;
		mNormal.z = 0.0;
		mNormal.w = 0.0;
		mForce.x = 0.0;
		mForce.y = 0.0;
		mForce.z = 0.0;
		mWorldPos = WWorldPos();
		Attrib::Instance::Change(&mSurface, SimSurface::kNull.mCollection);
	}

	void UpdateSurface(const Attrib::Collection* surface);
	bool InitPosition(ICollisionBody* cb, IRigidBody *rb, double maxcompression);
	bool UpdatePosition(const UMath::Vector3 &body_av, const UMath::Vector3 &body_lv,
							   const UMath::Matrix4 &body_matrix, const UMath::Vector3 &cog,
							   float dT, float wheel_radius, bool usecache, const WCollider *collider, float vehicle_height);

	const UMath::Vector4 &GetNormal() const {
		return mNormal;
	}

	const UMath::Vector3 &GetPosition() const {
		return mPosition;
	}

	void SetPosition(UMath::Vector3 &p) {
		mPosition = p;
	}

	void SetY(float y) {
		mPosition.y = y;
	}

	const UMath::Vector3 &GetForce() const {
		return mForce;
	}

	void SetForce(const UMath::Vector3 &f) {
		mForce = f;
	}

	void SetVelocity(const UMath::Vector3 &v) {
		mVelocity = v;
	}

	void IncAirTime(float dT) {
		mAirTime += dT;
	}

	void SetAirTime(float f) {
		mAirTime = f;
	}

	const UMath::Vector3 &GetLocalArm() const {
		return mLocalArm;
	}

	void SetLocalArm(UMath::Vector3 &arm) {
		mLocalArm = arm;
	}

	const UMath::Vector3 &GetWorldArm() const {
		return mWorldArm;
	}

	float GetCompression() const {
		return mCompression;
	}

	void SetCompression(float c) {
		mCompression = UMath::Max(c, 0.0f);
	}

	const SimSurface *GetSurface() const {
		return &mSurface;
	}

	const UMath::Vector3 &GetVelocity() const {
		return mVelocity;
	}

	bool IsOnGround() const {
		return mCompression > 0.0f;
	}

	void UpdateTime(float dT) {
		if (mSurfaceStick <= 0.0 || dT >= mSurfaceStick) mSurfaceStick = 0.0;
		else mSurfaceStick = mSurfaceStick - dT;
	}
};

class SuspensionRacer : public VehicleBehavior {
public:
	struct Steering {
		Steering() : InputAverage(0.55f, 60.0f), InputSpeedCoeffAverage(0.15f, 60.0f) {
			Reset();
		}

		float Previous;                       // offset 0x0, size 0x4
		float Wheels[2];                      // offset 0x4, size 0x8
		float Maximum;                        // offset 0xC, size 0x4
		float LastMaximum;                    // offset 0x10, size 0x4
		float LastInput;                      // offset 0x14, size 0x4
		AverageWindow InputAverage;           // offset 0x18, size 0x38
		AverageWindow InputSpeedCoeffAverage; // offset 0x50, size 0x38
		float CollisionTimer;                 // offset 0x88, size 0x4
		float WallNoseTurn;                   // offset 0x8C, size 0x4
		float WallSideTurn;                   // offset 0x90, size 0x4
		float YawControl;                     // offset 0x94, size 0x4

		void Reset() {
			Previous = 0.0f;
			Wheels[1] = 0.0f;
			Wheels[0] = 0.0f;
			Maximum = 45.0f;
			LastMaximum = 45.0f;
			LastInput = 0.0f;
			InputAverage.Reset(0.0f);
			InputSpeedCoeffAverage.Reset(0.0f);
			CollisionTimer = 0.0f;
			WallNoseTurn = 0.0f;
			WallSideTurn = 0.0f;
			YawControl = 1.0f;
		}
	};

	struct Drift {
		Drift() : State(D_OUT), Value(0.0f) {}

		enum eState { D_OUT, D_ENTER, D_IN, D_EXIT } State; // offset 0x0, size 0x4
		float Value;                                        // offset 0x4, size 0x4

		void Reset() {
			State = SuspensionRacer::Drift::D_OUT;
			Value = 0.0f;
		}
	};

	struct Burnout {
		Burnout() : mState(0), mBurnOutTime(0.0f), mTraction(1.0f), mBurnOutAllow(0.0f) {}

		void Update(const float dT, const float speedmph, const float max_slip, const int max_slip_wheel, const float yaw);

		int GetState() {
			return mState;
		}

		float GetTraction() {
			return mTraction;
		}

		void Reset() {
			mState = 0;
			mBurnOutTime = 0.0f;
			mTraction = 1.0f;
			mBurnOutAllow = 0.0f;
		}

		void SetState(int s) {
			mState = s;
		}

		void SetBurnOutTime(float t) {
			mBurnOutTime = t;
		}

		void SetTraction(float t) {
			mTraction = t;
		}

		float GetBurnOutTime(float t) {
			return mBurnOutTime;
		}

		void DecBurnOutTime(float t) {
			mBurnOutTime -= t;
		}

		void ClearBurnOutAllow() {
			mBurnOutAllow = 0.0f;
		}

		void IncBurnOutAllow(float t) {
			mBurnOutAllow += t;
		}

	private:
		int mState;          // offset 0x0, size 0x4
		float mBurnOutTime;  // offset 0x4, size 0x4
		float mTraction;     // offset 0x8, size 0x4
		float mBurnOutAllow; // offset 0xC, size 0x4
	};

	class Tire : public MWWheel {
	public:
		Tire(float radius, int index, const Attrib::Gen::car_tuning *specs)
				: MWWheel(1), mRadius(UMath::Max(radius, 0.1f)), mWheelIndex(index), mAxleIndex(index >> 1), mSpecs(specs), mBrake(0.0f),
				  mEBrake(0.0f), mAV(0.0f), mLoad(0.0f), mLateralForce(0.0f), mLongitudeForce(0.0f), mDriveTorque(0.0f), mBrakeTorque(0.0f), mLateralBoost(1.0f),
				  mTractionBoost(1.0f), mSlip(0.0f), mLastTorque(0.0f), mRoadSpeed(0.0f), mAngularAcc(0.0f), mTraction(1.0f), mBottomOutTime(0.0f),
				  mSlipAngle(0.0f), mTractionCircle(UMath::Vector2(1.0f, 1.0f)), mMaxSlip(0.5f), mGripBoost(1.0f), mDriftFriction(1.0f), mLateralSpeed(0.0f),
				  mBrakeLocked(false), mLastSign(SuspensionRacer::Tire::WAS_ZERO), mDragReduction(0.0f) {}
		void BeginFrame(float max_slip, float grip_boost, float traction_boost, float drag_reduction);
		void EndFrame(float dT);
		float ComputeLateralForce(float load, float slip_angle);
		float GetPilotFactor(const float speed);
		void CheckForBrakeLock(float ground_force);
		void CheckSign();
		void UpdateFree(float dT);
		float UpdateLoaded(float lat_vel, float fwd_vel, float body_speed, float load, float dT);

		bool IsSlipping() const {
			return mTraction >= 1.0f;
		}

		void SetBrake(float brake) {
			mBrake = brake;
		}

		void SetEBrake(float ebrake) {
			mEBrake = ebrake;
		}

		float GetEBrake() const {
			return mEBrake;
		}

		float GetTraction() const {
			return mTraction;
		}

		float GetRoadSpeed() const {
			return mRoadSpeed;
		}

		float GetLoad() const {
			return mLoad;
		}

		float GetRadius() const {
			return mRadius;
		}

		float GetAngularVelocity() const {
			return mAV;
		}

		void SetAngularVelocity(float w) {
			mAV = w;
		}

		float GetLateralSpeed() const {
			return mLateralSpeed;
		}

		float GetCurrentSlip() const {
			return mSlip;
		}

		float GetToleratedSlip() const {
			return mMaxSlip;
		}

		void SetLateralBoost(float f) {
			mLateralBoost = f;
		}

		void SetBottomOutTime(float time) {
			mBottomOutTime = time;
		}

		void ScaleTractionBoost(float scale) {
			mTractionBoost *= scale;
		}

		void SetDriftFriction(float scale) {
			mDriftFriction = scale;
		}

		void ApplyDriveTorque(float torque) {
			if (!mBrakeLocked) {
				mDriveTorque += torque;
			}
		}

		void ApplyBrakeTorque(float torque) {
			if (!mBrakeLocked) {
				mBrakeTorque += torque;
			}
		}

		float GetTotalTorque() const {
			return mDriveTorque + mBrakeTorque;
		}

		float GetDriveTorque() const {
			return mDriveTorque;
		}

		float GetLongitudeForce() const {
			return mLongitudeForce;
		}

		bool IsBrakeLocked() const {
			return mBrakeLocked;
		}

		bool IsSteeringWheel() const {
			return mWheelIndex < 2;
		}

		void SetTractionCircle(const UMath::Vector2 &circle) {
			mTractionCircle = circle;
		}

		float GetSlipAngle() const {
			return mSlipAngle;
		}

		void MatchSpeed(float speed) {
			mAV = speed / mRadius;
			mRoadSpeed = speed;
			mTraction = 1.0f;
			mBrakeLocked = false;
			mSlip = 0.0f;
			mAngularAcc = 0.0f;
			mLastTorque = 0.0f;
			mBrake = 0.0f;
			mEBrake = 0.0f;
			mLateralSpeed = 0.0f;
		}

		// total size: 0x140
		const float mRadius;                // offset 0xC4, size 0x4
		float mBrake;                       // offset 0xC8, size 0x4
		float mEBrake;                      // offset 0xCC, size 0x4
		float mAV;                          // offset 0xD0, size 0x4
		float mLoad;                        // offset 0xD4, size 0x4
		float mLateralForce;                // offset 0xD8, size 0x4
		float mLongitudeForce;              // offset 0xDC, size 0x4
		float mDriveTorque;                 // offset 0xE0, size 0x4
		float mBrakeTorque;                 // offset 0xE4, size 0x4
		float mLateralBoost;                // offset 0xE8, size 0x4
		float mTractionBoost;               // offset 0xEC, size 0x4
		float mSlip;                        // offset 0xF0, size 0x4
		float mLastTorque;                  // offset 0xF4, size 0x4
		const int mWheelIndex;              // offset 0xF8, size 0x4
		float mRoadSpeed;                   // offset 0xFC, size 0x4
		const Attrib::Gen::car_tuning *mSpecs;   // offset 0x100, size 0x4
		float mAngularAcc;                  // offset 0x108, size 0x4
		const int mAxleIndex;               // offset 0x10C, size 0x4
		float mTraction;                    // offset 0x110, size 0x4
		float mBottomOutTime;               // offset 0x114, size 0x4
		float mSlipAngle;                   // offset 0x118, size 0x4
		UMath::Vector2 mTractionCircle;     // offset 0x11C, size 0x8
		float mMaxSlip;                     // offset 0x124, size 0x4
		float mGripBoost;                   // offset 0x128, size 0x4
		float mDriftFriction;               // offset 0x12C, size 0x4
		float mLateralSpeed;                // offset 0x130, size 0x4
		bool mBrakeLocked;                  // offset 0x134, size 0x4

		enum LastRotationSign { WAS_POSITIVE, WAS_ZERO, WAS_NEGATIVE } mLastSign; // offset 0x138, size 0x4

		float mDragReduction; // offset 0x13C, size 0x4
	};

	float mJumpTime;
	float mJumpAlititude;
	float mTireHeat;
	Attrib::Gen::car_tuning mCarInfo;
	IEngine *mEngine;
	IEngineDamage *mEngineDamage;
	IInput *mInput;
	IRigidBody *mRB;
	ICollisionBody *mCollisionBody;
	ITransmission *mTransmission;
	IHumanAI *mHumanAI;
	ISpikeable *mSpikeDamage;
	float mGameBreaker;
	unsigned int mNumWheelsOnGround;
	float mLastGroundCollision;
	Drift mDrift;
	Burnout mBurnOut;
	Steering mSteering;
	Tire *mTires[4];
	IChassis tmpChassis;

	struct State {
		UMath::Matrix4 matrix;
		UMath::Vector3 local_vel;
		float gas_input;
		UMath::Vector3 linear_vel;
		float brake_input;
		UMath::Vector3 angular_vel;
		float ground_effect;
		UMath::Vector3 cog;
		float ebrake_input;
		UMath::Vector3 dimension;
		Angle steer_input;
		UMath::Vector3 local_angular_vel;
		Angle slipangle;
		UMath::Vector3 inertia;
		float mass;
		UMath::Vector3 world_cog;
		float speed;
		float time;
		int flags;
		short driver_style;
		short driver_class;
		short gear;
		short blown_tires;
		float nos_boost;
		float shift_boost;
		const WCollider *collider;

		enum Flags { IS_STAGING = 1, IS_DESTROYED };

		const UMath::Vector3 &GetRightVector() const {
			return *(UMath::Vector3*)&matrix.x;
		}
		const UMath::Vector3 &GetUpVector() const {
			return *(UMath::Vector3*)&matrix.y;
		}
		const UMath::Vector3 &GetForwardVector() const {
			return *(UMath::Vector3*)&matrix.z;
		}
		const UMath::Vector3 &GetPosition() const {
			return *(UMath::Vector3*)&matrix.p;
		}
	};

	struct Differential {
		void CalcSplit(bool locked);

		float angular_vel[2];
		int has_traction[2];
		float bias;
		float factor;
		float torque_split[2];
	};

	ISimable* GetOwner() {
		return Behavior::mIOwner;
	}

	IVehicle* GetVehicle() {
		return mVehicle;
	}

	bool RearWheelDrive() {
		return mCarInfo.GetLayout()->TORQUE_SPLIT < 1.0f;
	}

	bool FrontWheelDrive() {
		return mCarInfo.GetLayout()->TORQUE_SPLIT > 0.0f;
	}

	bool IsDriveWheel(unsigned int i) {
		return (IsRear(i) && RearWheelDrive()) || (IsFront(i) && FrontWheelDrive());
	}

	Tire &GetWheel(unsigned int i) {
		return *mTires[i];
	}

	const Tire &GetWheel(unsigned int i) const {
		return *mTires[i];
	}

	// hack around this enough so the compiler doesn't complain about the vtable being missing
	IChassis* GetIChassis() {
		auto addr = (uintptr_t)(this + offsetof(SuspensionRacer, tmpChassis));
		return (IChassis*)addr;
	}

	void OnOwnerAttached(IAttachable* pOther) { FUNCTION_LOG("OnOwnerAttached"); }
	void OnOwnerDetached(IAttachable* pOther) { FUNCTION_LOG("OnOwnerDetached"); }
	void OnPause() { FUNCTION_LOG("OnPause");  }
	void OnUnPause() { FUNCTION_LOG("OnUnPause");  }
	void OnDebugDraw() { FUNCTION_LOG("OnDebugDraw");  }
	int GetPriority() { FUNCTION_LOG("GetPriority"); return mPriority; }
	float GetDownCoefficient(float f) { FUNCTION_LOG("GetDownCoefficient"); return GetIChassis()->GetDownCoefficient(); }
	float GetDynamicRideHeight(unsigned int idx, State*) { FUNCTION_LOG("GetDynamicRideHeight"); return GetRideHeight(idx); }
	float GetDriftValue() { FUNCTION_LOG("GetDriftValue"); return 0.0; } // todo
	void ApplyVehicleEntryForces(bool enteringVehicle, const UMath::Vector3 *pos, bool calledfromEvent) { FUNCTION_LOG("ApplyVehicleEntryForces");  }

	void dtor(char a2);
	void Reset();
	float CalculateUndersteerFactor();
	float CalculateOversteerFactor();
	void MatchSpeed(float speed);
	Meters GetRideHeight(unsigned int idx) const;
	void DoDrifting(const State &state);
	float CalcYawControlLimit(float speed) const;
	void TuneWheelParams(State &state);
	float CalculateMaxSteering(State &state, ISteeringWheel::SteeringType steer_type);
	float CalculateSteeringSpeed(State &state);
	float DoHumanSteering(State &state);
	void ComputeAckerman(const float steering, const State &state, UMath::Vector4 *left, UMath::Vector4 *right) const;
	float DoAISteering(State &state);
	void DoWallSteer(State &state);
	void DoSteering(State &state, UMath::Vector3 &right, UMath::Vector3 &left);
	void DoWheelForces(State &state);
	void ComputeState(float dT, State &state);
	void SetCOG(float extra_bias, float extra_ride);
	Mps ComputeMaxSlip(const State &state) const;
	float ComputeLateralGripScale(const State &state) const;
	float ComputeTractionScale(const State &state) const;
	void DoTireHeat(const State &state);
	void DoAerodynamics(const State &state, float drag_pct, float aero_pct, float aero_front_z, float aero_rear_z, const Physics::Tunings *tunings);

	void CreateTires();
	void OnTaskSimulate(float dT);
	void OnBehaviorChange(const UCrc32 &mechanic);
	void Create(const BehaviorParams& bp);
	void DoJumpStabilizer(const State& bp);
	void DoDriveForces(State &state);
};