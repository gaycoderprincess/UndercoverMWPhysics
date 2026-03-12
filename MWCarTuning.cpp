#ifdef SUSPENSIONRACER_ELISE_TEST
MWCarTuning* GetMWCarData(const SuspensionRacer* pThis) {
	return GetCarTuning(pThis->mVehicle->GetVehicleName());
}
MWCarTuning* GetMWCarData(const SuspensionRacer::Tire* pThis) {
	return GetCarTuning(pThis->mVehicle->GetVehicleName());
}
MWCarTuning* GetMWCarData(const EngineRacer* pThis) {
	return GetCarTuning(pThis->mVehicle->GetVehicleName());
}
#else
Attrib::Gen::car_tuning::_LayoutStruct* GetMWCarData(const SuspensionRacer* pThis) {
	return pThis->mCarInfo.GetLayout();
}
Attrib::Gen::car_tuning::_LayoutStruct* GetMWCarData(const SuspensionRacer::Tire* pThis) {
	return pThis->mSpecs->GetLayout();
}
Attrib::Gen::car_tuning::_LayoutStruct* GetMWCarData(const EngineRacer* pThis) {
	return pThis->mCarInfo.GetLayout();
}
#endif