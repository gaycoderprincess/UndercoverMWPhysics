namespace MWRaceEngine {
	EngineRacer* GetEngineRacer(uintptr_t ptr) {
		ptr -= offsetof(EngineRacer, tmpRaceEngine);
		return (EngineRacer*)ptr;
	}

	float __thiscall GetPerfectLaunchRange(uintptr_t ptr, float* range) {
		IRACEENGINE_FUNCTION_LOG("GetPerfectLaunchRange");
		auto pThis = GetEngineRacer(ptr);
		return pThis->GetPerfectLaunchRange(*range);
	}

	void* NewVTable[] = {
			(void*)0x7B8560, // generic interface dtor
			(void*)&GetPerfectLaunchRange,
	};
}