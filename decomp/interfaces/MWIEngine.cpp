namespace MWIEngine {
	EngineRacer* GetEngineRacer(uintptr_t ptr) {
		ptr -= offsetof(EngineRacer, tmpEngine);
		return (EngineRacer*)ptr;
	}

	float __thiscall GetRPM(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetRPM");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetRPM();
	}
	float __thiscall GetEngineBogRPM(uintptr_t ptr) { // todo
		IENGINE_FUNCTION_LOG("GetEngineBogRPM");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetRPM();
	}
	float __thiscall GetRedline(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetRedline");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetRedline();
	}
	float __thiscall GetMaxRPM(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetMaxRPM");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetMaxRPM();
	}
	float __thiscall GetMinRPM(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetMinRPM");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetMinRPM();
	}
	float __thiscall GetPeakTorqueRPM(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetPeakTorqueRPM");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetPeakTorqueRPM();
	}
	float __thiscall GetTorqueRatio(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetTorqueRatio");
		auto pThis = GetEngineRacer(ptr);
		return 1.0; // todo
	}
	void __thiscall MatchSpeed(uintptr_t ptr, float f) {
		IENGINE_FUNCTION_LOG("MatchSpeed");
		auto pThis = GetEngineRacer(ptr);
		return pThis->MatchSpeed(f);
	}
	float __thiscall GetNOSCapacity(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetNOSCapacity");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetNOSCapacity();
	}
	void __thiscall SetNOSCapacity(uintptr_t ptr, float f) {
		IENGINE_FUNCTION_LOG("SetNOSCapacity");
		auto pThis = GetEngineRacer(ptr);
		pThis->mNOSCapacity = f;
	}
	bool __thiscall IsNOSEngaged(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("IsNOSEngaged");
		auto pThis = GetEngineRacer(ptr);
		return pThis->IsNOSEngaged();
	}
	float __thiscall GetNOSFlowRate(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetNOSFlowRate");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetNOSFlowRate();
	}
	float __thiscall GetNOSBoost(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetNOSBoost");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetNOSBoost();
	}
	bool __thiscall HasNOS(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("HasNOS");
		auto pThis = GetEngineRacer(ptr);
		return pThis->HasNOS();
	}
	bool __thiscall CanUseNOS(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("CanUseNOS");
		auto pThis = GetEngineRacer(ptr);
		//return pThis->mNOSCapacity > 0.0;
		return true; // is this the delay for nos usage at the start of a race?
	}
	void __thiscall ChargeNOS(uintptr_t ptr, float charge) {
		IENGINE_FUNCTION_LOG("ChargeNOS");
		auto pThis = GetEngineRacer(ptr);
		return pThis->ChargeNOS(charge);
	}
	float __thiscall GetMaxHorsePower(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetMaxHorsePower");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetMaxHorsePower();
	}
	float __thiscall GetMinHorsePower(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetMinHorsePower");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetMinHorsePower();
	}
	float __thiscall GetHorsePower(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetHorsePower");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetHorsePower();
	}
	float __thiscall GetThrottle(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetThrottle");
		auto pThis = GetEngineRacer(ptr);
		return pThis->mThrottle;
	}
	bool __thiscall IsTractionControlOn(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("IsTractionControlOn");
		auto pThis = GetEngineRacer(ptr);
		return false; // todo
	}
	int __thiscall GetTractionControlLevel(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetTractionControlLevel");
		auto pThis = GetEngineRacer(ptr);
		return 0;
	}
	void __thiscall SetTractionControlLevel(uintptr_t ptr, int) {
		IENGINE_FUNCTION_LOG("SetTractionControlLevel");
		auto pThis = GetEngineRacer(ptr);

	}
	float __thiscall GetEngineTemperature(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetEngineTemperature");
		auto pThis = GetEngineRacer(ptr);
		return 50.0; // todo
	}
	float __thiscall GetPeakTorque(uintptr_t ptr) {
		IENGINE_FUNCTION_LOG("GetPeakTorque");
		auto pThis = GetEngineRacer(ptr);
		return pThis->mPeakTorque;
	}

	void* NewVTable[] = {
			(void*)0xDEADBEEF, // dtor
			(void*)&GetRPM,
			(void*)&GetEngineBogRPM,
			(void*)&GetRedline,
			(void*)&GetMaxRPM,
			(void*)&GetMinRPM,
			(void*)&GetPeakTorqueRPM,
			(void*)&GetTorqueRatio,
			(void*)&MatchSpeed,
			(void*)&GetNOSCapacity,
			(void*)&SetNOSCapacity,
			(void*)&IsNOSEngaged,
			(void*)&GetNOSFlowRate,
			(void*)&GetNOSBoost,
			(void*)&HasNOS,
			(void*)&CanUseNOS,
			(void*)&ChargeNOS,
			(void*)&GetMaxHorsePower,
			(void*)&GetMinHorsePower,
			(void*)&GetHorsePower,
			(void*)&GetThrottle,
			(void*)&IsTractionControlOn,
			(void*)&GetTractionControlLevel,
			(void*)&SetTractionControlLevel,
			(void*)&GetEngineTemperature,
			(void*)&GetPeakTorque,
	};
}