namespace MWChassis {
	SuspensionRacer* GetSuspensionRacer(uintptr_t ptr) {
		ptr -= offsetof(SuspensionRacer, tmpChassis);
		return (SuspensionRacer*)ptr;
	}

	// calls in one frame:
	// IChassis::IsStaticResetCondition called from 43416C - AIVehicleHuman::OnTaskSimulate, resets the car if needed
	// IChassis::GetNumWheels called from 72D97A - don't run EngineRacer code if wheel count isn't 4
	// IChassis::GetWheelTraction called from 719550 - EngineRacer::DoThrottle
	// IChassis::GetWheelTraction called from 719563 - EngineRacer::DoThrottle
	// IChassis::GetWheelTraction called from 71957A - EngineRacer::DoThrottle
	// IChassis::GetWheelTraction called from 719591 - EngineRacer::DoThrottle
	// IChassis::GetNumWheelsOnGround called from 72DA0C
	// IChassis::GetWheelAngularVelocity called from 700270
	// IChassis::GetWheelAngularVelocity called from 700283
	// IChassis::GetWheelAngularVelocity called from 700316
	// IChassis::GetWheelAngularVelocity called from 700329
	// IChassis::GetWheelIdealTorque called from 70176F
	// IChassis::GetWheelIdealTorque called from 701785
	// IChassis::GetWheelBrakeTorque called from 7017A6
	// IChassis::GetWheelBrakeTorque called from 7017C1
	// IChassis::GetWheelIdealTorque called from 7017F6
	// IChassis::GetWheelIdealTorque called from 70180C
	// IChassis::GetWheelBrakeTorque called from 70182B
	// IChassis::GetWheelBrakeTorque called from 701846
	// IChassis::SetWheelMaximumTorqueRatio called from 72DD70
	// IChassis::SetWheelMaximumTorqueRatio called from 72DD89
	// IChassis::SetWheelMaximumTorqueRatio called from 72DDA3
	// IChassis::SetWheelMaximumTorqueRatio called from 72DDBD
	// IChassis::IsCounterSteering called from 72DEC3
	// IChassis::GetWheelAngularVelocity called from 700270
	// IChassis::GetWheelAngularVelocity called from 700283
	// IChassis::GetWheelAngularVelocity called from 700316
	// IChassis::GetWheelAngularVelocity called from 700329
	// IChassis::GetWheelIdealTorque called from 70176F
	// IChassis::GetWheelIdealTorque called from 701785
	// IChassis::GetWheelBrakeTorque called from 7017A6
	// IChassis::GetWheelBrakeTorque called from 7017C1
	// IChassis::GetWheelIdealTorque called from 7017F6
	// IChassis::GetWheelIdealTorque called from 70180C
	// IChassis::GetWheelBrakeTorque called from 70182B
	// IChassis::GetWheelBrakeTorque called from 701846
	// IChassis::GetNumWheelsOnGround called from 7011CD
	// IChassis::GetNumWheelsOnGround called from 7013AE
	// IChassis::GetNumWheelsOnGround called from 72D6CE
	// IChassis::GetWheelSlip called from 700425
	// IChassis::GetWheelSlip called from 700438
	// IChassis::GetWheelSlip called from 700471
	// IChassis::GetWheelSlip called from 700484
	// IChassis::GetWheelAngularVelocity called from 700270
	// IChassis::GetWheelAngularVelocity called from 700283
	// IChassis::GetWheelAngularVelocity called from 700316
	// IChassis::GetWheelAngularVelocity called from 700329
	// IChassis::IsWheelOnGround called from 70052A
	// IChassis::GetWheelAngularVelocity called from 700555
	// IChassis::GetWheelAngularVelocity called from 700568
	// IChassis::SetWheelAngularVelocity called from 7005C7
	// IChassis::SetWheelAngularVelocity called from 7005F0
	// IChassis::IsWheelOnGround called from 700632
	// IChassis::GetWheelAngularVelocity called from 70065D
	// IChassis::GetWheelAngularVelocity called from 700670
	// IChassis::SetWheelAngularVelocity called from 7006CF
	// IChassis::SetWheelAngularVelocity called from 7006F8
	// IChassis::GetNumWheels called from 700823
	// IChassis::IsWheelOnGround called from 70083E
	// IChassis::IsWheelOnGround called from 70083E
	// IChassis::IsWheelOnGround called from 70083E
	// IChassis::IsWheelOnGround called from 70083E
	// IChassis::GetNumWheels called from 700902
	// IChassis::SetWheelMaxAngularVelocity called from 70093D
	// IChassis::SetWheelMaxAngularVelocity called from 70093D
	// IChassis::SetWheelMaxAngularVelocity called from 70093D
	// IChassis::SetWheelMaxAngularVelocity called from 70093D
	// IChassis::GetNumWheelsOnGround called from 720AFF
	// IChassis::GetWheelSlipRatio called from 730F13
	// IChassis::GetWheelSlipRatio called from 730F22
	// IChassis::GetWheelSlipRatio called from 730F35
	// IChassis::GetWheelSlipRatio called from 730F48
	// IChassis::GetWheelAngularVelocity called from 730F61
	// IChassis::GetWheelAngularVelocity called from 730F70
	// IChassis::GetWheelAngularVelocity called from 730F83
	// IChassis::GetWheelAngularVelocity called from 730F96
	// IChassis::GetNumWheelsOnGround called from 70AAD1
	// IChassis::GetNumWheels called from 70AADC
	// IChassis::GetNumWheels called from 7BA907
	// IChassis::IsWheelOnGround called from 7BA940
	// IChassis::GetWheelRoadSurface called from 7BA977
	// IChassis::GetWheelSlip called from 7BA9AF
	// IChassis::GetWheelSkid called from 7BA9BB
	// IChassis::GetNumWheels called from 7BAA0F
	// IChassis::IsWheelOnGround called from 7BA940
	// IChassis::GetWheelRoadSurface called from 7BA977
	// IChassis::GetWheelSlip called from 7BA9AF
	// IChassis::GetWheelSkid called from 7BA9BB
	// IChassis::GetNumWheels called from 7BAA0F
	// IChassis::IsWheelOnGround called from 7BA940
	// IChassis::GetWheelRoadSurface called from 7BA977
	// IChassis::GetWheelSlip called from 7BA9AF
	// IChassis::GetWheelSkid called from 7BA9BB
	// IChassis::GetNumWheels called from 7BAA0F
	// IChassis::IsWheelOnGround called from 7BA940
	// IChassis::GetWheelRoadSurface called from 7BA977
	// IChassis::GetWheelSlip called from 7BA9AF
	// IChassis::GetWheelSkid called from 7BA9BB
	// IChassis::GetNumWheels called from 7BAA0F
	// IChassis::GetWheelieAngle called from 61944A
	// IChassis::GetWheelLoad called from 619469
	// IChassis::GetWheelLoad called from 61947C
	// IChassis::GetWheelLoad called from 61948F
	// IChassis::GetWheelLoad called from 6194A2
	// IChassis::GetWheelSlipRatio called from 6194B5
	// IChassis::GetWheelSlipRatio called from 6194C4
	// IChassis::GetWheelSlipRatio called from 6194D3
	// IChassis::GetWheelSlipRatio called from 6194E2
	// IChassis::GetWheelSlipAngle called from 6194F1
	// IChassis::GetWheelSlipAngle called from 619500
	// IChassis::GetWheelSlipAngle called from 61950F
	// IChassis::GetWheelSlipAngle called from 61951E
	// IChassis::GetWheelSlip called from 61952D
	// IChassis::GetWheelSlip called from 61953C
	// IChassis::GetWheelSlip called from 61954B
	// IChassis::GetWheelSlip called from 61955A
	// IChassis::GetCompression called from 619569
	// IChassis::GetCompression called from 619578
	// IChassis::GetCompression called from 619587
	// IChassis::GetCompression called from 619599
	// IChassis::GetSuspensionDigression called from 6195AE
	// IChassis::GetSuspensionDigression called from 6195C3
	// IChassis::GetSuspensionDigression called from 6195D8
	// IChassis::GetSuspensionDigression called from 6195ED
	// IChassis::IsWheelOnGround called from 428EF9
	// IChassis::IsWheelOnGround called from 428F0C
	// IChassis::IsWheelOnGround called from 428F1F
	// IChassis::IsWheelOnGround called from 428F32
	// IChassis::GetWheelRoadSurface called from 428F45
	// IChassis::GetWheelRoadSurface called from 428F66
	// IChassis::GetWheelRoadSurface called from 428F84
	// IChassis::GetWheelRoadSurface called from 428FA2
	// IChassis::GetNumWheelsOnGround called from 6F0F0E


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

	const char* __thiscall GetChassisName(uintptr_t ptr) { return "ChassisHuman"; }
	float __thiscall GetWheelTraction(uintptr_t ptr, unsigned int index) {
		ICHASSIS_FUNCTION_LOG("GetWheelTraction");
		auto pThis = GetSuspensionRacer(ptr);
		//return pThis->mTires[index]->GetTraction();
		return 1.0;
	}
	float __thiscall GetWheelSlipRatio(uintptr_t ptr, unsigned int index) { // todo this is weird
		ICHASSIS_FUNCTION_LOG("GetWheelSlipRatio");
		auto pThis = GetSuspensionRacer(ptr);
		if (pThis->mTires[index]->mRoadSpeed < 1.0f) return 0.0;
		return pThis->mTires[index]->GetSlipAngle() / pThis->mTires[index]->GetToleratedSlip();
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
		auto pThis = GetSuspensionRacer(ptr);
		return &pThis->mTires[i]->mPosition;
	}
	const UMath::Vector3 *__thiscall GetWheelLocalPos(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelLocalPos");
		auto pThis = GetSuspensionRacer(ptr);
		return &pThis->mTires[i]->mLocalArm;
	}
	UMath::Vector3 *__thiscall GetWheelCenterPos(uintptr_t ptr, UMath::Vector3 *result, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelCenterPos");
		auto pThis = GetSuspensionRacer(ptr);
		*result = pThis->mTires[i]->GetPosition();
		if (!pThis->mRB) {
			return result;
		} else {
			UMath::ScaleAdd(*pThis->mRB->GetUpVector(), pThis->mTires[i]->GetRadius(), *result, *result);
			return result;
		}
	}
	float __thiscall GetWheelTorque(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelTorque");
		auto pThis = GetSuspensionRacer(ptr);
		return pThis->mTires[i]->GetTotalTorque();
	}
	float __thiscall GetWheelBrakeTorque(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelBrakeTorque");
		auto pThis = GetSuspensionRacer(ptr);
		return pThis->mTires[i]->mBrakeTorque;
	}
	float __thiscall GetWheelLoad(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelLoad");
		auto pThis = GetSuspensionRacer(ptr);
		return pThis->mTires[i]->GetLoad();
	}
	float __thiscall GetWheelRoadHeight(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelRoadHeight");
		auto pThis = GetSuspensionRacer(ptr);
		return pThis->mTires[i]->GetNormal().w;
	}
	bool __thiscall IsWheelOnGround(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("IsWheelOnGround");
		auto pThis = GetSuspensionRacer(ptr);
		return pThis->mTires[i]->IsOnGround();
	}
	float __thiscall GetSuspensionMaxTravel(uintptr_t ptr, unsigned int i) { // todo is this correct
		ICHASSIS_FUNCTION_LOG("GetSuspensionMaxTravel");
		auto pThis = GetSuspensionRacer(ptr);
		return INCH2METERS(GetMWCarData(pThis)->TRAVEL.At(IsRear(i)));
	}
	float __thiscall GetCompression(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetCompression");
		auto pThis = GetSuspensionRacer(ptr);
		return pThis->mTires[i]->GetCompression();
	}
	float __thiscall GuessCompression(uintptr_t ptr, unsigned int id, float downforce) {
		ICHASSIS_FUNCTION_LOG("GuessCompression");
		auto pThis = GetSuspensionRacer(ptr);
		float compression = 0.0f;
		if (downforce < 0.0f) {
			unsigned int axle = id / 2;
			float spring_weight = LBIN2NM(GetMWCarData(pThis)->SPRING_STIFFNESS.At(axle));
			downforce *= 0.25f;
			compression = -downforce / spring_weight;
		}
		return compression;
	}
	void __thiscall ForceCompression(uintptr_t ptr, unsigned int id, float f) { // todo does this break stuff due to uc using it
		ICHASSIS_FUNCTION_LOG("ForceCompression");
		auto pThis = GetSuspensionRacer(ptr);
		pThis->mTires[id]->SetCompression(f);
	}
	void __thiscall DecompressShocks(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("DecompressShocks");
		auto pThis = GetSuspensionRacer(ptr);
	}
	float __thiscall GetWheelSlip(uintptr_t ptr, unsigned int id) {
		ICHASSIS_FUNCTION_LOG("GetWheelSlip");
		auto pThis = GetSuspensionRacer(ptr);
		return pThis->mTires[id]->GetCurrentSlip();
	}
	float __thiscall GetToleratedSlip(uintptr_t ptr, unsigned int id) {
		ICHASSIS_FUNCTION_LOG("GetToleratedSlip");
		auto pThis = GetSuspensionRacer(ptr);
		return pThis->mTires[id]->GetToleratedSlip();
	}
	float __thiscall GetWheelSkid(uintptr_t ptr, unsigned int id) {
		ICHASSIS_FUNCTION_LOG("GetWheelSkid");
		auto pThis = GetSuspensionRacer(ptr);
		return pThis->mTires[id]->GetLateralSpeed();
	}
	float __thiscall GetWheelSlipAngle(uintptr_t ptr, unsigned int id) {
		ICHASSIS_FUNCTION_LOG("GetWheelSlipAngle");
		auto pThis = GetSuspensionRacer(ptr);
		return pThis->mTires[id]->GetSlipAngle();
	}
	const UMath::Vector3* __thiscall GetWheelRoadNormal(uintptr_t ptr, unsigned int id) {
		ICHASSIS_FUNCTION_LOG("GetWheelRoadNormal");
		auto pThis = GetSuspensionRacer(ptr);
		return (UMath::Vector3*)&pThis->mTires[id]->mNormal;
	}
	const SimSurface* __thiscall GetWheelRoadSurface(uintptr_t ptr, unsigned int id) {
		ICHASSIS_FUNCTION_LOG("GetWheelRoadSurface");
		auto pThis = GetSuspensionRacer(ptr);
		return &pThis->mTires[id]->mSurface;
	}
	const UMath::Vector3* __thiscall GetWheelVelocity(uintptr_t ptr, unsigned int id) {
		ICHASSIS_FUNCTION_LOG("GetWheelVelocity");
		auto pThis = GetSuspensionRacer(ptr);
		return (UMath::Vector3*)&pThis->mTires[id]->mVelocity;
	}
	int __thiscall GetNumWheelsOnGround(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetNumWheelsOnGround");
		auto pThis = GetSuspensionRacer(ptr);
		return pThis->mNumWheelsOnGround;
	}
	unsigned int __thiscall GetNumWheelsOnBand(uintptr_t ptr) { // todo what is this?
		ICHASSIS_FUNCTION_LOG("GetNumWheelsOnBand");
		auto pThis = GetSuspensionRacer(ptr);
		//return pThis->mNumWheelsOnGround;
		return 0;
	}
	float __thiscall GetWheelAngularVelocity(uintptr_t ptr, int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelAngularVelocity");
		auto pThis = GetSuspensionRacer(ptr);
		return pThis->mTires[i]->GetAngularVelocity();
	}
	void __thiscall SetWheelAngularVelocity(uintptr_t ptr, int i, float f) {
		ICHASSIS_FUNCTION_LOG("SetWheelAngularVelocity");
		auto pThis = GetSuspensionRacer(ptr);
		pThis->mTires[i]->SetAngularVelocity(f);
	}
	void __thiscall SetWheelMaxAngularVelocity(uintptr_t ptr, int i, float f) { // todo this doesn't exist
		ICHASSIS_FUNCTION_LOG("SetWheelMaxAngularVelocity");
		auto pThis = GetSuspensionRacer(ptr);
	}
	float __thiscall GetWheelSteer(uintptr_t ptr, unsigned int wheel) {
		ICHASSIS_FUNCTION_LOG("GetWheelSteer");
		auto pThis = GetSuspensionRacer(ptr);
		return wheel < 2 ? RAD2ANGLE(pThis->mSteering.Wheels[wheel]) : 0.0f;
	}
	bool __thiscall CalculateFFBSteeringState(uintptr_t ptr, FFBSteeringState*) {
		ICHASSIS_FUNCTION_LOG("CalculateFFBSteeringState");
		auto pThis = GetSuspensionRacer(ptr);
		return false;
	}
	float __thiscall GetSuspensionDigression(uintptr_t ptr, unsigned int i) { // todo is this correct
		ICHASSIS_FUNCTION_LOG("GetSuspensionDigression");
		auto pThis = GetSuspensionRacer(ptr);
		return 1.0f - GetMWCarData(pThis)->SHOCK_DIGRESSION.At(IsRear(i));
	}
	float __thiscall GetWheelLateralForce(uintptr_t ptr, unsigned int i) {
		ICHASSIS_FUNCTION_LOG("GetWheelLateralForce");
		auto pThis = GetSuspensionRacer(ptr);
		return pThis->mTires[i]->mLateralForce;
	}
	float __thiscall GetRideHeight(uintptr_t ptr, unsigned int idx) {
		ICHASSIS_FUNCTION_LOG("GetRideHeight");
		auto pThis = GetSuspensionRacer(ptr);
		float ride = pThis->GetRideHeight(idx);
		//const Physics::Tunings *tunings = GetVehicle()->GetTunings();
		//if (tunings) {
		//	ride += INCH2METERS(tunings->Value[Physics::Tunings::RIDEHEIGHT]);
		//}
		return ride;
	}
	float __thiscall GetWheelRadius(uintptr_t ptr, unsigned int idx) {
		ICHASSIS_FUNCTION_LOG("GetWheelRadius");
		auto pThis = GetSuspensionRacer(ptr);
		return pThis->mTires[idx]->GetRadius();
	}
	float __thiscall GetMaxSteering(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetMaxSteering");
		auto pThis = GetSuspensionRacer(ptr);
		return DEG2ANGLE(pThis->mSteering.Maximum);
	}
	void __thiscall MatchSpeed(uintptr_t ptr, float speed, bool for_nis) {
		ICHASSIS_FUNCTION_LOG("MatchSpeed");
		auto pThis = GetSuspensionRacer(ptr);
		pThis->MatchSpeed(speed);
	}
	float __thiscall GetDriveshaftTorqueEffect(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetDriveshaftTorqueEffect");
		auto pThis = GetSuspensionRacer(ptr);
		return 0.0;
	}
	float __thiscall GetRenderMotion(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetRenderMotion");
		auto pThis = GetSuspensionRacer(ptr);
		return GetMWCarData(pThis)->RENDER_MOTION;
	}
	ISteeringWheel::SteeringType __thiscall GetSteeringType(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetSteeringType");
		auto pThis = GetSuspensionRacer(ptr);

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
		auto pThis = GetSuspensionRacer(ptr);
		auto tire = pThis->mTires[i];
		auto v70 = tire->mGripBoost;
		if (v70 <= 1.0) v70 = 1.0;
		float v63 = 1.0; // todo Curve::GetValueLinear((v19->mVehicleInfo->mLayoutPtr + 48 * v19->mAxleIndex + 0x1F0), (v21 * 2.23699)) Mu0[AxleIndex]
		auto v69 = std::sqrt(((tire->mLongitudeForce * tire->mLongitudeForce) + (tire->mLateralForce * tire->mLateralForce)));
		auto v72 = (v69 / (((((tire->mBrake + 1.0) * tire->mTractionBoost) * tire->mLoad) * v70) * v63));
		return UMath::Clamp(v72, -3.0f, 3.0f);
	}
	float __thiscall GetWheelIdealTorque(uintptr_t ptr, unsigned int i) { // todo what is this
		ICHASSIS_FUNCTION_LOG("GetWheelIdealTorque");
		auto pThis = GetSuspensionRacer(ptr);
		auto tire = pThis->mTires[i];
		return std::min(tire->GetTotalTorque() / tire->mRadius, 0.0f);
	}
	void __thiscall SetWheelRemoved(uintptr_t ptr, unsigned int i, bool b) { // todo
		ICHASSIS_FUNCTION_LOG("SetWheelRemoved");
		auto pThis = GetSuspensionRacer(ptr);
	}
	void __thiscall SetWheelMaximumTorqueRatio(uintptr_t ptr, unsigned int i, float f) { // todo
		ICHASSIS_FUNCTION_LOG("SetWheelMaximumTorqueRatio");
		auto pThis = GetSuspensionRacer(ptr);
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
		auto pThis = GetSuspensionRacer(ptr);
		return GetMWCarData(pThis)->DRAG_COEFFICIENT;
	}
	float __thiscall GetDownCoefficient(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetDownCoefficient");
		auto pThis = GetSuspensionRacer(ptr);
		return GetMWCarData(pThis)->AERO_COEFFICIENT.GetValue(UNDERCOVER_AeroCoeffAtValue);
	}
	float __thiscall GetStaticGripForSpeed(uintptr_t ptr, float f) { // todo
		ICHASSIS_FUNCTION_LOG("GetStaticGripForSpeed");
		auto pThis = GetSuspensionRacer(ptr);
		return 0.8;
	}
	void __thiscall SetClutchKickExtraTireSpin(uintptr_t ptr, float f) { // todo
		ICHASSIS_FUNCTION_LOG("SetClutchKickExtraTireSpin");
		auto pThis = GetSuspensionRacer(ptr);
	}
	float __thiscall GetWheelieAngle(uintptr_t ptr) { // todo
		ICHASSIS_FUNCTION_LOG("GetWheelieAngle");
		auto pThis = GetSuspensionRacer(ptr);
		return 0.0;
	}
	bool __thiscall IsStaticResetCondition(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("IsStaticResetCondition");
		auto pThis = GetSuspensionRacer(ptr);
		return false;
	}
	void __thiscall SetAICatchOverride(uintptr_t ptr, bool) {
		ICHASSIS_FUNCTION_LOG("SetAICatchOverride");
		auto pThis = GetSuspensionRacer(ptr);
	}
	float __thiscall GetSlipToGripImpactTime(uintptr_t ptr) { // todo
		ICHASSIS_FUNCTION_LOG("GetSlipToGripImpactTime");
		auto pThis = GetSuspensionRacer(ptr);
		return 0.0;
	}
	float __thiscall GetJumpTime(uintptr_t ptr) {
		ICHASSIS_FUNCTION_LOG("GetJumpTime");
		auto pThis = GetSuspensionRacer(ptr);
		return pThis->mJumpTime;
	}
	float __thiscall GetTimeFromLanding(uintptr_t ptr) { // todo
		ICHASSIS_FUNCTION_LOG("GetTimeFromLanding");
		auto pThis = GetSuspensionRacer(ptr);
		return 0.0;
	}
	bool __thiscall IsCounterSteering(uintptr_t ptr) { // todo
		ICHASSIS_FUNCTION_LOG("IsCounterSteering");
		auto pThis = GetSuspensionRacer(ptr);
		return false;
	}
}

void tmpdtor() {

}

void* NewChassisVTable[] = {
		(void*)0x7B8560, // generic interface dtor
		(void*)&MWChassis::GetChassisName,
		(void*)&MWChassis::GetWheelTraction,
		//(void*)&MWChassis::GetWheelDynamicSlipAngle,
		(void*)&MWChassis::GetWheelSlipRatio,
		(void*)&MWChassis::GetDragBoost,
		(void*)&MWChassis::GetNumWheels,
		(void*)&MWChassis::GetWheelPos,
		(void*)&MWChassis::GetWheelLocalPos,
		(void*)&MWChassis::GetWheelCenterPos,
		(void*)&MWChassis::GetWheelTorque,
		(void*)&MWChassis::GetWheelBrakeTorque,
		(void*)&MWChassis::GetWheelLoad,
		(void*)&MWChassis::GetWheelRoadHeight,
		(void*)&MWChassis::IsWheelOnGround,
		(void*)&MWChassis::GetCompression,
		(void*)&MWChassis::GetSuspensionMaxTravel,
		(void*)&MWChassis::GuessCompression,
		(void*)&MWChassis::ForceCompression,
		(void*)&MWChassis::DecompressShocks,
		(void*)&MWChassis::GetWheelSlip,
		(void*)&MWChassis::GetToleratedSlip,
		(void*)&MWChassis::GetWheelSkid,
		(void*)&MWChassis::GetWheelSlipAngle,
		(void*)&MWChassis::GetWheelRoadNormal,
		(void*)&MWChassis::GetWheelRoadSurface,
		(void*)&MWChassis::GetWheelVelocity,
		(void*)&MWChassis::GetNumWheelsOnGround,
		(void*)&MWChassis::GetNumWheelsOnBand,
		(void*)&MWChassis::GetWheelAngularVelocity,
		(void*)&MWChassis::SetWheelAngularVelocity,
		(void*)&MWChassis::SetWheelMaxAngularVelocity,
		(void*)&MWChassis::GetWheelSteer,
		(void*)&MWChassis::CalculateFFBSteeringState,
		(void*)&MWChassis::GetSuspensionDigression,
		(void*)&MWChassis::GetWheelLateralForce,
		(void*)&MWChassis::GetRideHeight,
		(void*)&MWChassis::GetWheelRadius,
		(void*)&MWChassis::GetMaxSteering,
		(void*)&MWChassis::MatchSpeed,
		(void*)&MWChassis::GetDriveshaftTorqueEffect,
		(void*)&MWChassis::GetRenderMotion,
		(void*)&MWChassis::GetSteeringType,
		(void*)&MWChassis::GetWheelTorqueRatio,
		(void*)&MWChassis::GetWheelIdealTorque,
		(void*)&MWChassis::SetWheelRemoved,
		(void*)&MWChassis::SetWheelMaximumTorqueRatio,
		(void*)&MWChassis::IsAntiBrakeLockOn,
		(void*)&MWChassis::GetAntiBrakeLockLevel,
		(void*)&MWChassis::SetAntiBrakeLockLevel,
		(void*)&MWChassis::IsStabilityManagementOn,
		(void*)&MWChassis::GetStabilityManagementLevel,
		(void*)&MWChassis::SetStabilityManagementLevel,
		(void*)&MWChassis::IsDriftAsssistOn,
		(void*)&MWChassis::GetDriftAssistLevel,
		(void*)&MWChassis::SetDriftAssistLevel,
		(void*)&MWChassis::IsDriftGlueToRoadOn,
		(void*)&MWChassis::GetDriftGlueToRoadLevel,
		(void*)&MWChassis::SetDriftGlueToRoadLevel,
		(void*)&MWChassis::IsDriftDynamicBrakeOn,
		(void*)&MWChassis::GetDriftDynamicBrakeLevel,
		(void*)&MWChassis::SetDriftDynamicBrakeLevel,
		(void*)&MWChassis::IsDriftSpeedControlOn,
		(void*)&MWChassis::GetDriftSpeedControlLevel,
		(void*)&MWChassis::SetDriftSpeedControlLevel,
		(void*)&MWChassis::IsRacelineAssistOn,
		(void*)&MWChassis::GetRacelineAssistLevel,
		(void*)&MWChassis::SetRacelineAssistLevel,
		(void*)&MWChassis::IsBrakingAssistOn,
		(void*)&MWChassis::GetBrakingAssistLevel,
		(void*)&MWChassis::SetBrakingAssistLevel,
		(void*)&MWChassis::GetDragCoefficient,
		(void*)&MWChassis::GetDownCoefficient,
		(void*)&MWChassis::GetStaticGripForSpeed,
		(void*)&MWChassis::SetClutchKickExtraTireSpin,
		(void*)&MWChassis::GetWheelieAngle,
		(void*)&MWChassis::IsStaticResetCondition,
		(void*)&MWChassis::SetAICatchOverride,
		(void*)&MWChassis::GetSlipToGripImpactTime,
		(void*)&MWChassis::GetJumpTime,
		(void*)&MWChassis::GetTimeFromLanding,
		(void*)&MWChassis::IsCounterSteering,
};