namespace MWInductable {
	EngineRacer* GetEngineRacer(uintptr_t ptr) {
		ptr -= offsetof(EngineRacer, tmpInductable);
		return (EngineRacer*)ptr;
	}

	enum eUCInductionType {
		INDUCTIONUC_NONE = 0,
		INDUCTIONUC_TURBO_CHARGER = 1,
		INDUCTIONUC_ROOTS_BLOWER = 2,
		INDUCTIONUC_CENTRIFUGAL_BLOWER = 3,
		INDUCTIONUC_TWINSCREW_BLOWER = 4,
	};

	int __thiscall GetInductionType(uintptr_t ptr) {
		IINDUCTABLE_FUNCTION_LOG("GetInductionType");
		auto pThis = GetEngineRacer(ptr);
		return pThis->InductionType();
	}

	float __thiscall GetSpool(uintptr_t ptr) {
		IINDUCTABLE_FUNCTION_LOG("GetSpool");
		auto pThis = GetEngineRacer(ptr);
		return pThis->InductionSpool();
	}

	float __thiscall GetCurrentPSI(uintptr_t ptr) {
		IINDUCTABLE_FUNCTION_LOG("GetCurrentPSI");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetInductionPSI();
	}

	float __thiscall GetMaxPSI(uintptr_t ptr) {
		IINDUCTABLE_FUNCTION_LOG("GetMaxPSI");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetMaxInductionPSI();
	}

	bool __thiscall HasBlowoffValve(uintptr_t ptr) {
		IINDUCTABLE_FUNCTION_LOG("HasBlowoffValve");
		auto pThis = GetEngineRacer(ptr);
		return false;
	}

	bool __thiscall HasBypassValve(uintptr_t ptr) {
		IINDUCTABLE_FUNCTION_LOG("HasBypassValve");
		auto pThis = GetEngineRacer(ptr);
		return false;
	}

	float __thiscall GetRelativeTorqueGain(uintptr_t ptr) {
		IINDUCTABLE_FUNCTION_LOG("GetRelativeTorqueGain");
		auto pThis = GetEngineRacer(ptr);
		//return pThis->GetRelativeTorqueGain();
		return 1.0;
	}

	float __thiscall GetBypassPosition(uintptr_t ptr) {
		IINDUCTABLE_FUNCTION_LOG("GetRelativeTorqueGain");
		auto pThis = GetEngineRacer(ptr);
		return 0.0;
	}

	bool __thiscall IsBlowoffOpened(uintptr_t ptr) {
		IINDUCTABLE_FUNCTION_LOG("IsBlowoffOpened");
		auto pThis = GetEngineRacer(ptr);
		return false;
	}

	bool __thiscall IsWastegateOpened(uintptr_t ptr) {
		IINDUCTABLE_FUNCTION_LOG("IsWastegateOpened");
		auto pThis = GetEngineRacer(ptr);
		return false;
	}

	void* NewVTable[] = {
			(void*)0x7B8560, // generic interface dtor
			(void*)&GetInductionType,
			(void*)&GetSpool,
			(void*)&GetCurrentPSI,
			(void*)&GetMaxPSI,
			(void*)&HasBlowoffValve,
			(void*)&HasBypassValve,
			(void*)&GetRelativeTorqueGain,
			(void*)&GetBypassPosition,
			(void*)&IsBlowoffOpened,
			(void*)&IsWastegateOpened,
	};
}