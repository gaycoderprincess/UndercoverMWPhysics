namespace MWIChassis {
	// this is horrible but vtables don't work with the way i have this set up
	ChassisMW* GetChassis(uintptr_t ptr) {
		ptr -= offsetof(ChassisMW, tmpChassis);
		return (ChassisMW*)ptr;
	}

	bool IsChassisSimple(uintptr_t ptr) {
		return !strcmp(GetChassis(ptr)->mChassisType, "SuspensionSimple");
	}

	SuspensionSimpleMW* GetSuspensionSimple(uintptr_t ptr) {
		if (strcmp(GetChassis(ptr)->mChassisType, "SuspensionSimple")) {
			MessageBoxA(nullptr, std::format("GetSuspensionSimple() wrong type! Called from {:X}", (uintptr_t)__builtin_return_address(0)).c_str(), "nya?!~", MB_ICONERROR);
			__debugbreak();
		}
		static_assert(offsetof(SuspensionSimpleMW, tmpChassis) == offsetof(ChassisMW, tmpChassis));
		ptr -= offsetof(SuspensionSimpleMW, tmpChassis);
		return (SuspensionSimpleMW*)ptr;
	}

	SuspensionRacerMW* GetSuspensionRacer(uintptr_t ptr) {
		if (strcmp(GetChassis(ptr)->mChassisType, "SuspensionRacer")) {
			MessageBoxA(nullptr, std::format("GetSuspensionRacer() wrong type! Called from {:X}", (uintptr_t)__builtin_return_address(0)).c_str(), "nya?!~", MB_ICONERROR);
			__debugbreak();
		}
		static_assert(offsetof(SuspensionRacerMW, tmpChassis) == offsetof(ChassisMW, tmpChassis));
		ptr -= offsetof(SuspensionRacerMW, tmpChassis);
		return (SuspensionRacerMW*)ptr;
	}

	const char *__thiscall GetChassisName(uintptr_t ptr);
	float __thiscall GetWheelTraction(uintptr_t ptr, unsigned int);
	float __thiscall GetWheelSlipRatio(uintptr_t ptr, unsigned int);
	float __thiscall GetDragBoost(uintptr_t ptr);
	unsigned int __thiscall GetNumWheels(uintptr_t ptr);
	const UMath::Vector3 *__thiscall GetWheelPos(uintptr_t ptr, unsigned int);
	const UMath::Vector3 *__thiscall GetWheelLocalPos(uintptr_t ptr, unsigned int);
	UMath::Vector3 *__thiscall GetWheelCenterPos(uintptr_t ptr, UMath::Vector3 *result, unsigned int);
	float __thiscall GetWheelTorque(uintptr_t ptr, unsigned int);
	float __thiscall GetWheelBrakeTorque(uintptr_t ptr, unsigned int);
	float __thiscall GetWheelLoad(uintptr_t ptr, unsigned int);
	float __thiscall GetWheelRoadHeight(uintptr_t ptr, unsigned int);
	bool __thiscall IsWheelOnGround(uintptr_t ptr, unsigned int);
	float __thiscall GetCompression(uintptr_t ptr, unsigned int);
	float __thiscall GetSuspensionMaxTravel(uintptr_t ptr, unsigned int);
	float __thiscall GuessCompression(uintptr_t ptr, unsigned int, float);
	void __thiscall ForceCompression(uintptr_t ptr, unsigned int, float);
	void __thiscall DecompressShocks(uintptr_t ptr);
	float __thiscall GetWheelSlip(uintptr_t ptr, unsigned int);
	float __thiscall GetToleratedSlip(uintptr_t ptr, unsigned int);
	float __thiscall GetWheelSkid(uintptr_t ptr, unsigned int);
	float __thiscall GetWheelSlipAngle(uintptr_t ptr, unsigned int);
	const UMath::Vector3 *__thiscall GetWheelRoadNormal(uintptr_t ptr, unsigned int);
	const SimSurface *__thiscall GetWheelRoadSurface(uintptr_t ptr, unsigned int);
	const UMath::Vector3 *__thiscall GetWheelVelocity(uintptr_t ptr, unsigned int);
	int __thiscall GetNumWheelsOnGround(uintptr_t ptr);
	unsigned int __thiscall GetNumWheelsOnBand(uintptr_t ptr);
	float __thiscall GetWheelAngularVelocity(uintptr_t ptr, int);
	void __thiscall SetWheelAngularVelocity(uintptr_t ptr, int, float);
	void __thiscall SetWheelMaxAngularVelocity(uintptr_t ptr, int, float);
	float __thiscall GetWheelSteer(uintptr_t ptr, unsigned int);
	bool __thiscall CalculateFFBSteeringState(uintptr_t ptr, FFBSteeringState *);
	float __thiscall GetSuspensionDigression(uintptr_t ptr, unsigned int);
	float __thiscall GetWheelLateralForce(uintptr_t ptr, unsigned int);
	float __thiscall GetRideHeight(uintptr_t ptr, unsigned int);
	float __thiscall GetWheelRadius(uintptr_t ptr, unsigned int);
	float __thiscall GetMaxSteering(uintptr_t ptr);
	void __thiscall MatchSpeed(uintptr_t ptr, float, bool);
	float __thiscall GetDriveshaftTorqueEffect(uintptr_t ptr);
	float __thiscall GetRenderMotion(uintptr_t ptr);
	ISteeringWheel::SteeringType __thiscall GetSteeringType(uintptr_t ptr);
	float __thiscall GetWheelTorqueRatio(uintptr_t ptr, unsigned int);
	float __thiscall GetWheelIdealTorque(uintptr_t ptr, unsigned int);
	void __thiscall SetWheelRemoved(uintptr_t ptr, unsigned int, bool);
	void __thiscall SetWheelMaximumTorqueRatio(uintptr_t ptr, unsigned int, float);
	bool __thiscall IsAntiBrakeLockOn(uintptr_t ptr);
	int __thiscall GetAntiBrakeLockLevel(uintptr_t ptr);
	void __thiscall SetAntiBrakeLockLevel(uintptr_t ptr, int);
	bool __thiscall IsStabilityManagementOn(uintptr_t ptr);
	int __thiscall GetStabilityManagementLevel(uintptr_t ptr);
	void __thiscall SetStabilityManagementLevel(uintptr_t ptr, int);
	bool __thiscall IsDriftAsssistOn(uintptr_t ptr);
	int __thiscall GetDriftAssistLevel(uintptr_t ptr);
	void __thiscall SetDriftAssistLevel(uintptr_t ptr, int);
	bool __thiscall IsDriftGlueToRoadOn(uintptr_t ptr);
	int __thiscall GetDriftGlueToRoadLevel(uintptr_t ptr);
	void __thiscall SetDriftGlueToRoadLevel(uintptr_t ptr, int);
	bool __thiscall IsDriftDynamicBrakeOn(uintptr_t ptr);
	int __thiscall GetDriftDynamicBrakeLevel(uintptr_t ptr);
	void __thiscall SetDriftDynamicBrakeLevel(uintptr_t ptr, int);
	bool __thiscall IsDriftSpeedControlOn(uintptr_t ptr);
	int __thiscall GetDriftSpeedControlLevel(uintptr_t ptr);
	void __thiscall SetDriftSpeedControlLevel(uintptr_t ptr, int);
	bool __thiscall IsRacelineAssistOn(uintptr_t ptr);
	int __thiscall GetRacelineAssistLevel(uintptr_t ptr);
	void __thiscall SetRacelineAssistLevel(uintptr_t ptr, int);
	bool __thiscall IsBrakingAssistOn(uintptr_t ptr);
	int __thiscall GetBrakingAssistLevel(uintptr_t ptr);
	void __thiscall SetBrakingAssistLevel(uintptr_t ptr, int);
	float __thiscall GetDragCoefficient(uintptr_t ptr);
	float __thiscall GetDownCoefficient(uintptr_t ptr);
	float __thiscall GetStaticGripForSpeed(uintptr_t ptr, float);
	void __thiscall SetClutchKickExtraTireSpin(uintptr_t ptr, float);
	float __thiscall GetWheelieAngle(uintptr_t ptr);
	bool __thiscall IsStaticResetCondition(uintptr_t ptr);
	void __thiscall SetAICatchOverride(uintptr_t ptr, bool);
	float __thiscall GetSlipToGripImpactTime(uintptr_t ptr);
	float __thiscall GetJumpTime(uintptr_t ptr);
	float __thiscall GetTimeFromLanding(uintptr_t ptr);
	bool __thiscall IsCounterSteering(uintptr_t ptr);

	const char* __thiscall GetChassisName(uintptr_t ptr) { return IsChassisSimple(ptr) ? "ChassisSimple" : "ChassisHuman"; }
	float __thiscall GetWheelTraction(uintptr_t ptr, unsigned int index) {
		ICHASSIS_FUNCTION_LOG("GetWheelTraction");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mTires[index]->mTraction;
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelTraction(index);
		}
	}
	float __thiscall GetWheelSlipRatio(uintptr_t ptr, unsigned int index) { // todo this is weird
		ICHASSIS_FUNCTION_LOG("GetWheelSlipRatio");
		float traction = 1.0;
		if (IsChassisSimple(ptr)) {
			traction = GetSuspensionSimple(ptr)->mTires[index]->mTraction;
		}
		else {
			traction = GetSuspensionRacer(ptr)->GetWheelTraction(index);
		}
		traction *= 1.25; // a bit of leeway so turning doesn't immediately make skidmarks
		return UMath::Clamp(1.0f - traction, 0.0f, 1.0f);
	}
	float __thiscall GetDragBoost(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetDragBoost");
		return 0.0;
	}
	unsigned int __thiscall GetNumWheels(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetNumWheels");
		return 4;
	}
	const UMath::Vector3 *__thiscall GetWheelPos(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelPos");
		if (IsChassisSimple(ptr)) {
			return &GetSuspensionSimple(ptr)->mTires[i]->GetPosition();
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelPos(i);
		}
	}
	const UMath::Vector3 *__thiscall GetWheelLocalPos(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelLocalPos");
		if (IsChassisSimple(ptr)) {
			return &GetSuspensionSimple(ptr)->mTires[i]->GetLocalArm();
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelLocalPos(i);
		}
	}
	UMath::Vector3 *__thiscall GetWheelCenterPos(uintptr_t ptr, UMath::Vector3 *result, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelCenterPos");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->GetWheelCenterPos(result, i);
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelCenterPos(result, i);
		}
	}
	float __thiscall GetWheelTorque(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelTorque");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mTires[i]->mAppliedTorque;
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelTorque(i);
		}
	}
	float __thiscall GetWheelBrakeTorque(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelBrakeTorque");
		if (IsChassisSimple(ptr)) {
			return 0.0;
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelBrakeTorque(i);
		}
	}
	float __thiscall GetWheelLoad(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelLoad");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mTires[i]->mLoad;
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelLoad(i);
		}
	}
	float __thiscall GetWheelRoadHeight(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelRoadHeight");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mTires[i]->mNormal.w;
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelRoadHeight(i);
		}
	}
	bool __thiscall IsWheelOnGround(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("IsWheelOnGround");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mTires[i]->IsOnGround();
		}
		else {
			return GetSuspensionRacer(ptr)->IsWheelOnGround(i);
		}
	}
	float __thiscall GetSuspensionMaxTravel(uintptr_t ptr, unsigned int i) { // todo is this correct
		ICHASSIS_FUNCTION_LOG("GetSuspensionMaxTravel");
		auto pThis = GetChassis(ptr);
		return INCH2METERS(pThis->mMWAttributes->TRAVEL.At(IsRear(i)));
	}
	float __thiscall GetCompression(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetCompression");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mTires[i]->GetCompression();
		}
		else {
			return GetSuspensionRacer(ptr)->GetCompression(i);
		}
	}
	float __thiscall GuessCompression(uintptr_t ptr, unsigned int id, float downforce) {
		ICHASSIS_FUNCTION_LOG("GuessCompression");
		auto pThis = GetChassis(ptr);
		float compression = 0.0f;
		if (downforce < 0.0f) {
			unsigned int axle = id / 2;
			float spring_weight = LBIN2NM(pThis->mMWAttributes->SPRING_STIFFNESS.At(axle));
			downforce *= 0.25f;
			compression = -downforce / spring_weight;
		}
		return compression;
	}
	void __thiscall ForceCompression(uintptr_t ptr, unsigned int id, float f) { // todo does this break stuff due to uc using it
		ICHASSIS_FUNCTION_LOG("ForceCompression");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mTires[id]->SetCompression(f);
		}
		else {
			return GetSuspensionRacer(ptr)->ForceCompression(id, f);
		}
	}
	void __thiscall DecompressShocks(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("DecompressShocks");
	}
	float __thiscall GetWheelSlip(uintptr_t ptr, unsigned int id) {
		ICHASSIS_FUNCTION_LOG("GetWheelSlip");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mTires[id]->mSlip;
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelSlip(id);
		}
	}
	float __thiscall GetToleratedSlip(uintptr_t ptr, unsigned int id) {
		ICHASSIS_FUNCTION_LOG("GetToleratedSlip");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mTires[id]->mMaxSlip;
		}
		else {
			return GetSuspensionRacer(ptr)->GetToleratedSlip(id);
		}
	}
	float __thiscall GetWheelSkid(uintptr_t ptr, unsigned int id) {
		ICHASSIS_FUNCTION_LOG("GetWheelSkid");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mTires[id]->mLateralSpeed;
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelSkid(id);
		}
	}
	float __thiscall GetWheelSlipAngle(uintptr_t ptr, unsigned int id) {
		ICHASSIS_FUNCTION_LOG("GetWheelSlipAngle");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mTires[id]->mSlipAngle;
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelSlipAngle(id);
		}
	}
	const UMath::Vector3* __thiscall GetWheelRoadNormal(uintptr_t ptr, unsigned int id) {
		ICHASSIS_FUNCTION_LOG("GetWheelRoadNormal");
		if (IsChassisSimple(ptr)) {
			return (UMath::Vector3*)&GetSuspensionSimple(ptr)->mTires[id]->mNormal;
		}
		else {
			return (UMath::Vector3*)GetSuspensionRacer(ptr)->GetWheelRoadNormal(id);
		}
	}
	const SimSurface* __thiscall GetWheelRoadSurface(uintptr_t ptr, unsigned int id) {
		ICHASSIS_FUNCTION_LOG("GetWheelRoadSurface");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mTires[id]->GetSurface();
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelRoadSurface(id);
		}
	}
	const UMath::Vector3* __thiscall GetWheelVelocity(uintptr_t ptr, unsigned int id) {
		ICHASSIS_FUNCTION_LOG("GetWheelVelocity");
		if (IsChassisSimple(ptr)) {
			return &GetSuspensionSimple(ptr)->mTires[id]->GetVelocity();
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelVelocity(id);
		}
	}
	int __thiscall GetNumWheelsOnGround(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetNumWheelsOnGround");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mNumWheelsOnGround;
		}
		else {
			return GetSuspensionRacer(ptr)->GetNumWheelsOnGround();
		}
	}
	unsigned int __thiscall GetNumWheelsOnBand(uintptr_t ptr) { // todo what is this?
		ICHASSIS_FUNCTION_LOG("GetNumWheelsOnBand");
		return 0;
	}
	float __thiscall GetWheelAngularVelocity(uintptr_t ptr, int index) {
		ICHASSIS_FUNCTION_LOG("GetWheelAngularVelocity");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mTires[index]->mAV;
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelAngularVelocity(index);
		}
	}
	void __thiscall SetWheelAngularVelocity(uintptr_t ptr, int i, float f) {
		ICHASSIS_FUNCTION_LOG("SetWheelAngularVelocity");
		if (IsChassisSimple(ptr)) {
			GetSuspensionSimple(ptr)->mTires[i]->mAV = f;
		}
		else {
			GetSuspensionRacer(ptr)->SetWheelAngularVelocity(i, f);
		}
	}
	void __thiscall SetWheelMaxAngularVelocity(uintptr_t ptr, int i, float f) { // todo this doesn't exist
		ICHASSIS_FUNCTION_LOG("SetWheelMaxAngularVelocity");
	}
	float __thiscall GetWheelSteer(uintptr_t ptr, unsigned int wheel) {
		ICHASSIS_FUNCTION_LOG("GetWheelSteer");
		if (IsChassisSimple(ptr)) {
			return wheel < 2 ? RAD2ANGLE(GetSuspensionSimple(ptr)->mWheelSteer[wheel]) : 0.0f;
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelSteer(wheel);
		}
	}
	bool __thiscall CalculateFFBSteeringState(uintptr_t ptr, FFBSteeringState*) {
		ICHASSIS_FUNCTION_LOG("CalculateFFBSteeringState");
		return false;
	}
	float __thiscall GetSuspensionDigression(uintptr_t ptr, unsigned int i) { // todo is this correct
		ICHASSIS_FUNCTION_LOG("GetSuspensionDigression");
		auto pThis = GetChassis(ptr);
		return 1.0f - pThis->mMWAttributes->SHOCK_DIGRESSION.At(IsRear(i));
	}
	float __thiscall GetWheelLateralForce(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelLateralForce");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mTires[i]->mLateralForce;
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelLateralForce(i);
		}
	}
	float __thiscall GetRideHeight(uintptr_t ptr, unsigned int idx) {
		ICHASSIS_FUNCTION_LOG("GetRideHeight");
		auto pThis = GetChassis(ptr);
		float ride = pThis->GetRideHeight(idx);
		const Physics::Tunings *tunings = GetVehicleMWTunings(pThis->GetVehicle());
		if (tunings) {
			ride += INCH2METERS(tunings->Value[Physics::Tunings::RIDEHEIGHT]);
		}
		return ride;
	}
	float __thiscall GetWheelRadius(uintptr_t ptr, unsigned int idx) {
		ICHASSIS_FUNCTION_LOG("GetWheelRadius");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->mTires[idx]->mRadius;
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelRadius(idx);
		}
	}
	float __thiscall GetMaxSteering(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetMaxSteering");
		if (IsChassisSimple(ptr)) {
			return 45.0;
		}
		else {
			return GetSuspensionRacer(ptr)->GetMaxSteering();
		}
	}
	void __thiscall MatchSpeed(uintptr_t ptr, float speed, bool for_nis) {
		ICHASSIS_FUNCTION_LOG("MatchSpeed");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->MatchSpeed(speed);
		}
		else {
			return GetSuspensionRacer(ptr)->MatchSpeed(speed);
		}
	}
	float __thiscall GetDriveshaftTorqueEffect(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetDriveshaftTorqueEffect");
		return 0.0;
	}
	float __thiscall GetRenderMotion(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetRenderMotion");
		auto pThis = GetChassis(ptr);
		return pThis->mMWAttributes->RENDER_MOTION;
	}
	ISteeringWheel::SteeringType __thiscall GetSteeringType(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetSteeringType");
		auto pThis = GetChassis(ptr);

		ISteeringWheel::SteeringType steer_type = ISteeringWheel::kGamePad;

		IPlayer *player = pThis->GetOwner()->GetPlayer();
		if (player) {
			ISteeringWheel *device = player->GetSteeringDevice();

			if (device && device->IsConnected()) {
				steer_type = device->GetSteeringType();
			}
		}

		return steer_type;
	}
	float __thiscall GetWheelTorqueRatio(uintptr_t ptr, unsigned int i) { // todo what is this
		ICHASSIS_FUNCTION_LOG("GetWheelTorqueRatio");
		if (IsChassisSimple(ptr)) {
			return GetSuspensionSimple(ptr)->GetWheelTorqueRatio(i);
		}
		else {
			return GetSuspensionRacer(ptr)->GetWheelTorqueRatio(i);
		}
	}
	float __thiscall GetWheelIdealTorque(uintptr_t ptr, unsigned int i) { // todo what is this
		ICHASSIS_FUNCTION_LOG("GetWheelIdealTorque");
		if (IsChassisSimple(ptr)) {
			auto pThis = GetSuspensionSimple(ptr);
			return std::min(pThis->mTires[i]->mAppliedTorque / pThis->mTires[i]->mRadius, 0.0f);
		}
		else {
			auto pThis = GetSuspensionRacer(ptr);
			return std::min(pThis->GetWheelTorque(i) / pThis->GetWheelRadius(i), 0.0f);
		}
	}
	void __thiscall SetWheelRemoved(uintptr_t ptr, unsigned int i, bool b) { // todo
		ICHASSIS_FUNCTION_LOG("SetWheelRemoved");
	}
	void __thiscall SetWheelMaximumTorqueRatio(uintptr_t ptr, unsigned int i, float f) { // todo
		ICHASSIS_FUNCTION_LOG("SetWheelMaximumTorqueRatio");
	}
	bool __thiscall IsAntiBrakeLockOn(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("1"); return false; }
	int __thiscall GetAntiBrakeLockLevel(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("2"); return 0; }
	void __thiscall SetAntiBrakeLockLevel(uintptr_t ptr, int i) { ICHASSIS_FUNCTION_LOG("3"); return; }
	bool __thiscall IsStabilityManagementOn(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("4"); return false; }
	int __thiscall GetStabilityManagementLevel(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("5"); return 0; }
	void __thiscall SetStabilityManagementLevel(uintptr_t ptr, int i) { ICHASSIS_FUNCTION_LOG("6"); return; }
	bool __thiscall IsDriftAsssistOn(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("7"); return false; }
	int __thiscall GetDriftAssistLevel(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("8"); return 0; }
	void __thiscall SetDriftAssistLevel(uintptr_t ptr, int i) { ICHASSIS_FUNCTION_LOG("9"); return; }
	bool __thiscall IsDriftGlueToRoadOn(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("10"); return false; }
	int __thiscall GetDriftGlueToRoadLevel(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("11"); return 0; }
	void __thiscall SetDriftGlueToRoadLevel(uintptr_t ptr, int i) { ICHASSIS_FUNCTION_LOG("12"); return; }
	bool __thiscall IsDriftDynamicBrakeOn(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("13"); return false; }
	int __thiscall GetDriftDynamicBrakeLevel(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("14"); return 0; }
	void __thiscall SetDriftDynamicBrakeLevel(uintptr_t ptr, int i) { ICHASSIS_FUNCTION_LOG("15"); return; }
	bool __thiscall IsDriftSpeedControlOn(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("16"); return false; }
	int __thiscall GetDriftSpeedControlLevel(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("17"); return 0; }
	void __thiscall SetDriftSpeedControlLevel(uintptr_t ptr, int i) { ICHASSIS_FUNCTION_LOG("18"); return; }
	bool __thiscall IsRacelineAssistOn(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("19"); return false; }
	int __thiscall GetRacelineAssistLevel(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("20"); return 0; }
	void __thiscall SetRacelineAssistLevel(uintptr_t ptr, int i) { ICHASSIS_FUNCTION_LOG("21"); return; }
	bool __thiscall IsBrakingAssistOn(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("22"); return false; }
	int __thiscall GetBrakingAssistLevel(uintptr_t ptr) { ICHASSIS_FUNCTION_LOG("23"); return 0; }
	void __thiscall SetBrakingAssistLevel(uintptr_t ptr, int i) { ICHASSIS_FUNCTION_LOG("24"); return; }
	float __thiscall GetDragCoefficient(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetDragCoefficient");
		auto pThis = GetChassis(ptr);
		return pThis->mMWAttributes->DRAG_COEFFICIENT;
	}
	float __thiscall GetDownCoefficient(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetDownCoefficient");
		auto pThis = GetChassis(ptr);
		return pThis->mMWAttributes->AERO_COEFFICIENT;
	}
	float __thiscall GetStaticGripForSpeed(uintptr_t ptr, float f) { // todo
		ICHASSIS_FUNCTION_LOG("GetStaticGripForSpeed");
		return 0.8;
	}
	void __thiscall SetClutchKickExtraTireSpin(uintptr_t ptr, float f) { // todo
		ICHASSIS_FUNCTION_LOG("SetClutchKickExtraTireSpin");
	}
	float __thiscall GetWheelieAngle(uintptr_t ptr) { // todo
		ICHASSIS_FUNCTION_LOG("GetWheelieAngle");
		return 0.0;
	}
	bool __thiscall IsStaticResetCondition(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("IsStaticResetCondition");
		return false;
	}
	void __thiscall SetAICatchOverride(uintptr_t ptr, bool) {
		ICHASSIS_FUNCTION_LOG("SetAICatchOverride");
	}
	float __thiscall GetSlipToGripImpactTime(uintptr_t ptr) { // todo
		ICHASSIS_FUNCTION_LOG("GetSlipToGripImpactTime");
		return 0.0;
	}
	float __thiscall GetJumpTime(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetJumpTime");
		auto pThis = GetChassis(ptr);
		return pThis->mJumpTime;
	}
	float __thiscall GetTimeFromLanding(uintptr_t ptr) { // todo
		ICHASSIS_FUNCTION_LOG("GetTimeFromLanding");
		return 0.0;
	}
	bool __thiscall IsCounterSteering(uintptr_t ptr) { // todo
		ICHASSIS_FUNCTION_LOG("IsCounterSteering");
		return false;
	}

	void* NewVTable[] = {
			(void*)0xDEADBEEF, // dtor
			(void*)&GetChassisName,
			(void*)&GetWheelTraction,
			//(void*)&GetWheelDynamicSlipAngle,
			(void*)&GetWheelSlipRatio,
			(void*)&GetDragBoost,
			(void*)&GetNumWheels,
			(void*)&GetWheelPos,
			(void*)&GetWheelLocalPos,
			(void*)&GetWheelCenterPos,
			(void*)&GetWheelTorque,
			(void*)&GetWheelBrakeTorque,
			(void*)&GetWheelLoad,
			(void*)&GetWheelRoadHeight,
			(void*)&IsWheelOnGround,
			(void*)&GetCompression,
			(void*)&GetSuspensionMaxTravel,
			(void*)&GuessCompression,
			(void*)&ForceCompression,
			(void*)&DecompressShocks,
			(void*)&GetWheelSlip,
			(void*)&GetToleratedSlip,
			(void*)&GetWheelSkid,
			(void*)&GetWheelSlipAngle,
			(void*)&GetWheelRoadNormal,
			(void*)&GetWheelRoadSurface,
			(void*)&GetWheelVelocity,
			(void*)&GetNumWheelsOnGround,
			(void*)&GetNumWheelsOnBand,
			(void*)&GetWheelAngularVelocity,
			(void*)&SetWheelAngularVelocity,
			(void*)&SetWheelMaxAngularVelocity,
			(void*)&GetWheelSteer,
			(void*)&CalculateFFBSteeringState,
			(void*)&GetSuspensionDigression,
			(void*)&GetWheelLateralForce,
			(void*)&GetRideHeight,
			(void*)&GetWheelRadius,
			(void*)&GetMaxSteering,
			(void*)&MatchSpeed,
			(void*)&GetDriveshaftTorqueEffect,
			(void*)&GetRenderMotion,
			(void*)&GetSteeringType,
			(void*)&GetWheelTorqueRatio,
			(void*)&GetWheelIdealTorque,
			(void*)&SetWheelRemoved,
			(void*)&SetWheelMaximumTorqueRatio,
			(void*)&IsAntiBrakeLockOn,
			(void*)&GetAntiBrakeLockLevel,
			(void*)&SetAntiBrakeLockLevel,
			(void*)&IsStabilityManagementOn,
			(void*)&GetStabilityManagementLevel,
			(void*)&SetStabilityManagementLevel,
			(void*)&IsDriftAsssistOn,
			(void*)&GetDriftAssistLevel,
			(void*)&SetDriftAssistLevel,
			(void*)&IsDriftGlueToRoadOn,
			(void*)&GetDriftGlueToRoadLevel,
			(void*)&SetDriftGlueToRoadLevel,
			(void*)&IsDriftDynamicBrakeOn,
			(void*)&GetDriftDynamicBrakeLevel,
			(void*)&SetDriftDynamicBrakeLevel,
			(void*)&IsDriftSpeedControlOn,
			(void*)&GetDriftSpeedControlLevel,
			(void*)&SetDriftSpeedControlLevel,
			(void*)&IsRacelineAssistOn,
			(void*)&GetRacelineAssistLevel,
			(void*)&SetRacelineAssistLevel,
			(void*)&IsBrakingAssistOn,
			(void*)&GetBrakingAssistLevel,
			(void*)&SetBrakingAssistLevel,
			(void*)&GetDragCoefficient,
			(void*)&GetDownCoefficient,
			(void*)&GetStaticGripForSpeed,
			(void*)&SetClutchKickExtraTireSpin,
			(void*)&GetWheelieAngle,
			(void*)&IsStaticResetCondition,
			(void*)&SetAICatchOverride,
			(void*)&GetSlipToGripImpactTime,
			(void*)&GetJumpTime,
			(void*)&GetTimeFromLanding,
			(void*)&IsCounterSteering,
	};
}