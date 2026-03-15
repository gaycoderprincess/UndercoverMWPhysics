class ChassisMW : public VehicleBehavior {
public:
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

	enum SleepState {
		SS_LATERAL = 2,
		SS_ALL = 1,
		SS_NONE = 0,
	};

	void Create(const BehaviorParams &bp);
	void Destroy(char a2);

	Mps ComputeMaxSlip(const State &state);
	void DoTireHeat(const State &state);
	float ComputeLateralGripScale(const State &state);
	float ComputeTractionScale(const State &state);
	SleepState DoSleep(const State &state);
	void ComputeAckerman(const float steering, const State &state, UMath::Vector4 *left, UMath::Vector4 *right);
	void SetCOG(float extra_bias, float extra_ride);
	void ComputeState(float dT, State &state);
	void DoAerodynamics(const State &state, float drag_pct, float aero_pct, float aero_front_z, float aero_rear_z,
						const Physics::Tunings *tunings);
	void DoJumpStabilizer(const State &state);

	Meters GuessCompression(unsigned int id, float downforce);
	void OnBehaviorChange(const UCrc32 &mechanic);
	float GetRenderMotion();
	Meters GetRideHeight(unsigned int idx);
	float CalculateUndersteerFactor();
	float CalculateOversteerFactor();
	void OnTaskSimulate(float dT);

	ICollisionBody *mRBComplex;
	IRigidBody *mRB;
	IInput *mInput;
	IEngine *mEngine;
	ITransmission *mTransmission;
	//IDragTransmission *mDragTransmission;
	IEngineDamage *mEngineDamage;
	ISpikeable *mSpikeDamage;
	Attrib::Gen::car_tuning mAttributes;
	MWCarTuning* mMWAttributes;
	float mJumpTime;
	float mJumpAlititude;
	float mTireHeat;

	IChassis tmpChassis;

	// hack around this enough so the compiler doesn't complain about the vtable being missing
	IChassis* GetIChassis() { GET_FAKE_INTERFACE(ChassisMW, IChassis, tmpChassis) }

	ISimable* GetOwner() const {
		return Behavior::mIOwner;
	}

	IVehicle* GetVehicle() const {
		return mVehicle;
	}

	void OnOwnerAttached(IAttachable* pOther) { CHASSIS_FUNCTION_LOG("OnOwnerAttached"); }
	void OnOwnerDetached(IAttachable* pOther) { CHASSIS_FUNCTION_LOG("OnOwnerDetached"); }
	void OnPause() { CHASSIS_FUNCTION_LOG("OnPause"); }
	void OnUnPause() { CHASSIS_FUNCTION_LOG("OnUnPause"); }
	void OnDebugDraw() { CHASSIS_FUNCTION_LOG("OnDebugDraw"); }
	int GetPriority() { CHASSIS_FUNCTION_LOG("GetPriority"); return mPriority; }
	float GetDownCoefficient(float f) { CHASSIS_FUNCTION_LOG("GetDownCoefficient"); return GetIChassis()->GetDownCoefficient(); }
	int OnService(HSIMSERVICE__* hCon, void* pkt) { return 0; }

	// IChassis
	//virtual float GetWheelTraction(uint32_t);
	//virtual unsigned int GetNumWheels();
	//virtual const UMath::Vector3* GetWheelPos(uint32_t);
	//virtual const UMath::Vector3* GetWheelLocalPos(uint32_t);
	//virtual UMath::Vector3* GetWheelCenterPos(UMath::Vector3*, uint32_t);
	//virtual float GetWheelLoad(uint32_t);
	//virtual void ApplyVehicleEntryForces(bool, const UMath::Vector3*, bool);
	//virtual float GetWheelRoadHeight(uint32_t);
	//virtual bool IsWheelOnGround(uint32_t);
	//virtual float GetCompression(uint32_t);
	//virtual Meters GuessCompression(uint32_t, float);
	//virtual float GetWheelSlip(uint32_t);
	//virtual float GetToleratedSlip(uint32_t);
	//virtual float GetWheelSkid(uint32_t);
	//virtual float GetWheelSlipAngle(uint32_t);
	//virtual const UMath::Vector4* GetWheelRoadNormal(uint32_t);
	//virtual const SimSurface* GetWheelRoadSurface(uint32_t);
	//virtual const UMath::Vector3* GetWheelVelocity(uint32_t);
	//virtual int GetNumWheelsOnGround();
	//virtual float GetWheelAngularVelocity(uint32_t);
	//virtual void SetWheelAngularVelocity(int, float);
	//virtual float GetWheelSteer(uint32_t);
	//virtual float CalculateUndersteerFactor();
	//virtual float CalculateOversteerFactor();
	//virtual Meters GetRideHeight(uint32_t idx);
	//virtual float GetWheelRadius(uint32_t);
	//virtual float GetMaxSteering();
	//virtual void MatchSpeed(float);
	//virtual float GetRenderMotion();

	// UC stuff
	//virtual float GetWheelTorque(int);
	//virtual float GetWheelTorqueRatio(int);
	//virtual float GetWheelBrakeTorque(int);
	//virtual float GetWheelLateralForce(int);
	//virtual void ForceCompression(int, float);

	const char* mChassisType = "Chassis";
};