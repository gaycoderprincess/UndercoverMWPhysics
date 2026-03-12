namespace MWTiptronic {
	EngineRacer* GetEngineRacer(uintptr_t ptr) {
		ptr -= offsetof(EngineRacer, tmpTiptronic);
		return (EngineRacer*)ptr;
	}

	bool __thiscall SportShift(uintptr_t ptr, GearID gear) {
		ITIPTRONIC_FUNCTION_LOG("SportShift");
		auto pThis = GetEngineRacer(ptr);
		return pThis->SportShift(gear);
	}

	void* NewVTable[] = {
			(void*)0x7B8560, // generic interface dtor
			(void*)&SportShift,
	};
}