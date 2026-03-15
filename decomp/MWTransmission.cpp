namespace MWTransmission {
	EngineRacer* GetEngineRacer(uintptr_t ptr) {
		ptr -= offsetof(EngineRacer, tmpTransmission);
		return (EngineRacer*)ptr;
	}

	GearID __thiscall GetGear(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetGear");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetGear();
	}
	GearID __thiscall GetTopGear(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetTopGear");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetTopGear();
	}
	bool __thiscall Shift(uintptr_t ptr, GearID gear, bool forceOnRemote = false) {
		ITRANSMISSION_FUNCTION_LOG("Shift");
		auto pThis = GetEngineRacer(ptr);
		return pThis->Shift(gear);
	}
	bool __thiscall IsClutchEngaged(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("IsClutchEngaged");
		auto pThis = GetEngineRacer(ptr);
		return pThis->mClutch.mState == EngineRacer::Clutch::ENGAGED;
	}
	bool __thiscall IsGearChanging(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("IsGearChanging");
		auto pThis = GetEngineRacer(ptr);
		return pThis->IsGearChanging();
	}
	bool __thiscall IsReversing(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("IsReversing");
		auto pThis = GetEngineRacer(ptr);
		return pThis->IsReversing();
	}
	float __thiscall GetSpeedometer(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetSpeedometer");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetSpeedometer();
	}
	float __thiscall GetMaxSpeedometer(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetMaxSpeedometer");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetMaxSpeedometer();
	}
	float __thiscall GetDriveTorqueAtEngine(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetDriveTorqueAtEngine");
		auto pThis = GetEngineRacer(ptr);
		return pThis->mDriveTorqueAtEngine;
	}
	float __thiscall GetDriveTorque(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetDriveTorque");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetDriveTorque();
	}
	float __thiscall GetEngineBrakingTorque(uintptr_t ptr) { // todo this might not be correct
		ITRANSMISSION_FUNCTION_LOG("GetEngineBrakingTorque");
		auto pThis = GetEngineRacer(ptr);
		auto rpm = RPS2RPM(pThis->mAngularVelocity);
		float engine_torque = pThis->GetEngineTorque(rpm);
		return pThis->GetBrakingTorque(engine_torque, rpm);
	}
	float __thiscall GetShiftPoint(uintptr_t ptr, GearID from_gear, GearID to_gear) {
		ITRANSMISSION_FUNCTION_LOG("GetShiftPoint");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetShiftPoint(from_gear, to_gear);
	}
	ShiftStatus __thiscall GetShiftStatus(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetShiftStatus");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetShiftStatus();
	}
	ShiftPotential __thiscall GetShiftPotential(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetShiftPotential");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetShiftPotential();
	}
	bool __thiscall DoClutchKick(uintptr_t ptr, float) { // todo?
		ITRANSMISSION_FUNCTION_LOG("DoClutchKick");
		auto pThis = GetEngineRacer(ptr);
		return false;
	}
	float __thiscall GetTorqueSplit(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetTorqueSplit");
		auto pThis = GetEngineRacer(ptr);
		return pThis->mMWInfo->TORQUE_SPLIT;
	}
	float __thiscall GetTotalShiftTime(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetTotalShiftTime");
		auto pThis = GetEngineRacer(ptr);
		return pThis->mGearShiftTimer; // todo is this correct
	}
	ClutchState __thiscall GetClutchState(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetClutchState");
		auto pThis = GetEngineRacer(ptr);
		switch (pThis->mClutch.mState) {
			case EngineRacer::Clutch::DISENGAGED:
				return CLUTCH_STATE_DISENGAGED;
			case EngineRacer::Clutch::ENGAGING:
				return pThis->mClutch.mShiftingUp ? CLUTCH_STATE_UPSHIFT : CLUTCH_STATE_DOWNSHIFT;
			case EngineRacer::Clutch::ENGAGED:
			default:
				return CLUTCH_STATE_ENGAGED;
		}
	}
	float __thiscall GetTimeUntilTransition(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetTimeUntilTransition");
		auto pThis = GetEngineRacer(ptr);
		return pThis->mGearShiftTimer;
	}
	float __thiscall GetClutchPosition(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetClutchPosition");
		auto pThis = GetEngineRacer(ptr);
		return 0.0; // todo
	}
	float __thiscall GetClutchGrind(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetClutchGrind");
		auto pThis = GetEngineRacer(ptr);
		return 0.0; // todo
	}
	void __thiscall SetTransmissionOverride(uintptr_t ptr, eTransmissionOverride transmission) {
		ITRANSMISSION_FUNCTION_LOG("SetTransmissionOverride");
		auto pThis = GetEngineRacer(ptr);
		pThis->mTransmissionOverride = transmission;
	}
	eTransmissionOverride __thiscall GetTransmissionOverride(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetTransmissionOverride");
		auto pThis = GetEngineRacer(ptr);
		return pThis->mTransmissionOverride;
	}
	float __thiscall GetPerfectLaunchError(uintptr_t ptr) {
		ITRANSMISSION_FUNCTION_LOG("GetPerfectLaunchError");
		auto pThis = GetEngineRacer(ptr);
		return 1.0; // todo
	}
	
	void* NewVTable[] = {
			(void*)0xDEADBEEF, // dtor
			(void*)&GetGear,
			(void*)&GetTopGear,
			(void*)&Shift,
			(void*)&IsClutchEngaged,
			(void*)&IsGearChanging,
			(void*)&IsReversing,
			(void*)&GetSpeedometer,
			(void*)&GetMaxSpeedometer,
			(void*)&GetDriveTorqueAtEngine,
			(void*)&GetDriveTorque,
			(void*)&GetEngineBrakingTorque,
			(void*)&GetShiftPoint,
			(void*)&GetShiftStatus,
			(void*)&GetShiftPotential,
			(void*)&DoClutchKick,
			(void*)&GetTorqueSplit,
			(void*)&GetTotalShiftTime,
			(void*)&GetClutchState,
			(void*)&GetTimeUntilTransition,
			(void*)&GetClutchPosition,
			(void*)&GetClutchGrind,
			(void*)&SetTransmissionOverride,
			(void*)&GetTransmissionOverride,
			(void*)&GetPerfectLaunchError,
	};
}