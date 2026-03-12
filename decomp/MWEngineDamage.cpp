namespace MWEngineDamage {
	EngineRacer* GetEngineRacer(uintptr_t ptr) {
		ptr -= offsetof(EngineRacer, tmpEngineDamage);
		return (EngineRacer*)ptr;
	}

	bool __thiscall IsBlown(uintptr_t ptr) {
		IENGINEDAMAGE_FUNCTION_LOG("IsBlown");
		auto pThis = GetEngineRacer(ptr);
		return pThis->IsBlown();
	}

	bool __thiscall Blow(uintptr_t ptr) {
		IENGINEDAMAGE_FUNCTION_LOG("Blow");
		auto pThis = GetEngineRacer(ptr);
		return pThis->Blow();
	}

	void __thiscall Sabotage(uintptr_t ptr, float f) {
		IENGINEDAMAGE_FUNCTION_LOG("Sabotage");
		auto pThis = GetEngineRacer(ptr);
		return pThis->Sabotage(f);
	}

	bool __thiscall IsSabotaged(uintptr_t ptr) {
		IENGINEDAMAGE_FUNCTION_LOG("IsSabotaged");
		auto pThis = GetEngineRacer(ptr);
		return pThis->IsSabotaged();
	}

	void __thiscall Repair(uintptr_t ptr) {
		IENGINEDAMAGE_FUNCTION_LOG("Repair");
		auto pThis = GetEngineRacer(ptr);
		return pThis->Repair();
	}

	void* NewVTable[] = {
			(void*)0x7B8560, // generic interface dtor
			(void*)&IsBlown,
			(void*)&Blow,
			(void*)&Sabotage,
			(void*)&IsSabotaged,
			(void*)&Repair,
	};
}