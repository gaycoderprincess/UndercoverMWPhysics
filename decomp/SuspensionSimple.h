class SuspensionSimpleMW : public ChassisMW {
  public:
	class Tire : public WheelMW {
	  public:
		enum LastRotationSign {
			WAS_POSITIVE = 0,
			WAS_ZERO = 1,
			WAS_NEGATIVE = 2,
		};

		Tire(float radius, int index, MWCarTuning* mwSpecs);
		void BeginFrame(float max_slip, float grip_scale, float traction_boost);
		void EndFrame(float dT);
		void UpdateFree(float dT);
		float UpdateLoaded(float lat_vel, float fwd_vel, float body_speed, float load, float dT);
		void CheckForBrakeLock(float ground_force);
		Newtons ComputeLateralForce(float load, float slip_angle);

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
		}

		void Reset() {
			WheelMW::Reset();
			mLoad = 0.0f;
			mBrake = 0.0f;
			mEBrake = 0.0f;
			mAV = 0.0f;
			mLateralForce = 0.0f;
			mLongitudeForce = 0.0f;
			mAppliedTorque = 0.0f;
			mTractionBoost = 1.0f;
			mSlip = 0.0f;
			mLateralSpeed = 0.0f;
			mRoadSpeed = 0.0f;
			mTraction = 1.0f;
			mBrakeLocked = false;
			mAllowSlip = false;
			mLastTorque = 0.0f;
			mAngularAcc = 0.0f;
			mMaxSlip = 0.5f;
		}

		float GetRadius() const {
			return mRadius;
		}

		void ApplyTorque(float torque) {
			if (!mBrakeLocked) {
				mAppliedTorque += torque;
			}
		}

		void ScaleTractionBoost(float scale) {
			mTractionBoost *= scale;
		}

		void AllowSlip(bool b) {
			mAllowSlip = b;
		}

		float mRadius;
		float mBrake;
		float mEBrake;
		float mAV;
		float mLoad;
		float mLateralForce;
		float mLongitudeForce;
		float mAppliedTorque;
		float mTractionBoost;
		float mSlip;
		float mLateralSpeed;
		int mWheelIndex;
		float mRoadSpeed;
		float mSlipAngle;
		int mAxleIndex;
		float mTraction;
		bool mBrakeLocked;
		bool mAllowSlip;
		float mLastTorque;
		float mAngularAcc;
		float mMaxSlip;
		float mGripBoost;
		MWCarTuning *mMWSpecs;
	};

	void Create(const BehaviorParams &bp);
	void Destroy(char a2);
	void DoAerobatics(State &state);
	void DoSteering(State &state, UMath::Vector3 &right, UMath::Vector3 &left);
	void DoWallSteer(State &state);
	void DoDriveForces(State &state);
	void DoWheelForces(ChassisMW::State &state);
	void OnTaskSimulate(float dT);
	float CalculateUndersteerFactor() { return 0.0; }
	float CalculateOversteerFactor() { return 0.0; }

	// ISuspension
	void MatchSpeed(float speed);
	UMath::Vector3* GetWheelCenterPos(UMath::Vector3* result, unsigned int i);

	// UC IChassis
	float GetWheelTorqueRatio(int idx) {
		auto tire = mTires[idx];
		auto v70 = tire->mGripBoost;
		if (v70 <= 1.0) v70 = 1.0;
		float v63 = 1.0; // todo Curve::GetValueLinear((v19->mVehicleInfo->mLayoutPtr + 48 * v19->mAxleIndex + 0x1F0), (v21 * 2.23699)) Mu0[AxleIndex]
		auto v69 = std::sqrt(((tire->mLongitudeForce * tire->mLongitudeForce) + (tire->mLateralForce * tire->mLateralForce)));
		auto v72 = (v69 / (((((tire->mBrake + 1.0) * tire->mTractionBoost) * tire->mLoad) * v70) * v63));
		return UMath::Clamp(v72, -3.0f, 3.0f);
	}

	// Behavior
	void Reset();

	// IListener
	void OnCollision(const Sim::Collision::Info &cinfo);

	Tire &GetWheel(unsigned int i) {
		return *mTires[i];
	}

	IInput *mInput;
	MWCheater *mCheater;
	float mFrictionBoost;
	float mDraft;
	bool mPowerSliding;
	float mWheelSteer[2];
	float mYawControlMultiplier;
	unsigned int mNumWheelsOnGround;
	float mAgainstWall;
	float mMaxSteering;
	float mTimeInAir;
	float mSleepTime;
	bool mDriftPhysics;
	Tire *mTires[4];

	float GetDriftValue() { SUSPENSIONSIMPLE_FUNCTION_LOG("GetDriftValue"); return 0.0; }
	void ApplyVehicleEntryForces(bool enteringVehicle, const UMath::Vector3 *pos, bool calledfromEvent) { SUSPENSIONSIMPLE_FUNCTION_LOG("ApplyVehicleEntryForces");  }
};