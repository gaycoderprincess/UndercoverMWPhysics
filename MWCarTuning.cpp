#include "MWCarTuning.h"
#include "decomp/SuspensionRacer.h"

#ifdef SUSPENSIONRACER_ELISE_TEST
MWCarTuning* GetMWCarData(const SuspensionRacer* pThis) {
	return GetCarTuning(pThis->mVehicle->GetVehicleName());
}
MWCarTuning* GetMWCarDataFromTire(const void* pThis) {
	const SuspensionRacer::Tire* castTire = reinterpret_cast<const SuspensionRacer::Tire*>(pThis);
	return GetCarTuning(castTire->mVehicle->GetVehicleName());
}
#else
Attrib::Gen::car_tuning::_LayoutStruct* GetMWCarData(const SuspensionRacer* pThis) {
	return pThis->mCarInfo.GetLayout();
}
Attrib::Gen::car_tuning::_LayoutStruct* GetMWCarData(const SuspensionRacer::Tire* pThis) {
	return pThis->mSpecs->GetLayout();
}
#endif