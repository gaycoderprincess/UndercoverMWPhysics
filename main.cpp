#include <windows.h>
#include <format>
#include <cmath>
#include <numbers>
#include <toml++/toml.hpp>

#include "nya_commonhooklib.h"
#include "nya_commonmath.h"
#include "nfsuc.h"

#include "include/chloemenulib.h"

void WriteLog(const std::string& str) {
	static auto file = std::ofstream("NFSUCMWPhysics_gcp.log");

	file << str;
	file << "\n";
	file.flush();
}

wchar_t gDLLDir[MAX_PATH];
class DLLDirSetter {
public:
	wchar_t backup[MAX_PATH];

	DLLDirSetter() {
		GetCurrentDirectoryW(MAX_PATH, backup);
		SetCurrentDirectoryW(gDLLDir);
	}
	~DLLDirSetter() {
		SetCurrentDirectoryW(backup);
	}
};

//#define FUNCTION_LOG(name) WriteLog(std::format("{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)));
#define WHEEL_FUNCTION_LOG(name) WriteLog(std::format("Wheel::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)));
#define CHASSIS_FUNCTION_LOG(name) WriteLog(std::format("Chassis::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)));
#define SUSPENSIONSIMPLE_FUNCTION_LOG(name) WriteLog(std::format("SuspensionSimple::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)));
#define SUSPENSIONRACER_FUNCTION_LOG(name) WriteLog(std::format("SuspensionRacer::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)));
#define ENGINERACER_FUNCTION_LOG(name) WriteLog(std::format("EngineRacer::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)));
//#define ICHASSIS_FUNCTION_LOG(name) WriteLog(std::format("IChassis::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)))
//#define ITIPTRONIC_FUNCTION_LOG(name) WriteLog(std::format("ITiptronic::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)))
//#define IRACEENGINE_FUNCTION_LOG(name) WriteLog(std::format("IRaceEngine::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)))
//#define IENGINEDAMAGE_FUNCTION_LOG(name) WriteLog(std::format("IEngineDamage::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)))
//#define IINDUCTABLE_FUNCTION_LOG(name) WriteLog(std::format("IInductable::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)))
//#define ITRANSMISSION_FUNCTION_LOG(name) WriteLog(std::format("ITransmission::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)))
//#define IENGINE_FUNCTION_LOG(name) WriteLog(std::format("IEngine::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)))
#define ICHASSIS_FUNCTION_LOG(name) {}
#define ITIPTRONIC_FUNCTION_LOG(name) {}
#define IRACEENGINE_FUNCTION_LOG(name) {}
#define IENGINEDAMAGE_FUNCTION_LOG(name) {}
#define IINDUCTABLE_FUNCTION_LOG(name) {}
#define ITRANSMISSION_FUNCTION_LOG(name) {}
#define IENGINE_FUNCTION_LOG(name) {}

bool bAffectOpponents = false;
bool bAffectCops = false;
bool bRevLimiter = true;

auto cartuning_LookupKey = (uint32_t(__thiscall*)(Attrib::Gen::car_tuning*, const ISimable*, int))0x721E20;
auto ctor_cartuning = (void(__thiscall*)(Attrib::Gen::car_tuning*, uint32_t))0x721CB0;
auto dtor_simobject = (void(__thiscall*)(void*))0x7BC8A0;

#define GET_FAKE_INTERFACE(base, type, var) { auto ptr = (uintptr_t)this; ptr += offsetof(base, var); return (type*)ptr; }

#include "decomp/ConversionUtil.h"
#include "decomp/UMathExtras.h"
#include "MWCarTuning.h"
#include "decomp/AverageWindow.h"
#include "decomp/MWICheaterDummy.h"
#include "decomp/behaviors/MWWheel.h"
#include "decomp/behaviors/MWChassisBase.h"
#include "decomp/behaviors/SuspensionRacer.h"
#include "decomp/behaviors/SuspensionSimple.h"
#include "decomp/behaviors/EngineRacer.h"
#include "decomp/interfaces/MWIChassis.cpp"
#include "decomp/interfaces/MWIRaceEngine.cpp"
#include "decomp/interfaces/MWITiptronic.cpp"
#include "decomp/interfaces/MWIEngineDamage.cpp"
#include "decomp/interfaces/MWIInductable.cpp"
#include "decomp/interfaces/MWITransmission.cpp"
#include "decomp/interfaces/MWIEngine.cpp"
#include "decomp/behaviors/MWWheel.cpp"
#include "decomp/behaviors/MWChassisBase.cpp"
#include "decomp/behaviors/SuspensionRacer.cpp"
#include "decomp/behaviors/SuspensionSimple.cpp"
#include "decomp/behaviors/EngineRacer.cpp"

void ValueEditorMenu(float& value) {
	ChloeMenuLib::BeginMenu();

	static char inputString[1024] = {};
	ChloeMenuLib::AddTextInputToString(inputString, 1024, true);
	ChloeMenuLib::SetEnterHint("Apply");

	if (DrawMenuOption(inputString + (std::string)"...", "", false, false) && inputString[0]) {
		value = std::stof(inputString);
		memset(inputString,0,sizeof(inputString));
		ChloeMenuLib::BackOut();
	}

	ChloeMenuLib::EndMenu();
}

void QuickValueEditor(const char* name, float& value) {
	if (DrawMenuOption(std::format("{} - {}", name, value))) { ValueEditorMenu(value); }
}

// whatever highway is in uc:
// LATERAL_GRIP 1
// DRIVE_GRIP 0.1
// ROLLING_RESISTANCE 0.13

// asphalt_no_leaves uc:
// LATERAL_GRIP 1
// DRIVE_GRIP 1
// ROLLING_RESISTANCE 0

// asphalt_no_leaves mw:
// LATERAL_GRIP 1
// DRIVE_GRIP 1
// ROLLING_RESISTANCE 1

// DoDriveForces is almost entirely responsible for acceleration in MW, without it the car just rolls

// rx7 mw:
// mass 1280
// tensor scale 1.0 3.5 1.0

// rx7 uc:
// mass 1270
// tensor scale 1.2 1.6 1.2

void DebugMenu() {
	ChloeMenuLib::BeginMenu();

	if (DrawMenuOption("EngineRacer")) {
		ChloeMenuLib::BeginMenu();

		if (pEngine) {
			auto ply = VEHICLE_LIST::GetList(VEHICLE_PLAYERS)[0];

			// engine -1.0 1.0 tune 2, -1 torque 1 horsepower
			// suspension -1.0 1.0 tune 3, -1 soft 1 stiff
			// drivetrain -1.0 1.0 tune 1, -1 accel 1 top speed
			// tires -1.0 1.0 tune 4, -1 loose 1 grip
			// nitrous -1.0 1.0 tune 0, -1 strength 1 duration

			for (int i = 0; i < 32; i++) {
				DrawMenuOption(std::format("tune {} {:.2f}", i, ply->GetCustomizations()->PhysicsTuning[i]));
			}

			DrawMenuOption(std::format("CARSLOTID_BRAKE_PACKAGE {}", ply->GetCustomizations()->InstalledParts[CARSLOTID_BRAKE_PACKAGE].kit_num));
			DrawMenuOption(std::format("CARSLOTID_DRIVETRAIN_PACKAGE {}", ply->GetCustomizations()->InstalledParts[CARSLOTID_DRIVETRAIN_PACKAGE].kit_num));
			DrawMenuOption(std::format("CARSLOTID_ENGINE_PACKAGE {}", ply->GetCustomizations()->InstalledParts[CARSLOTID_ENGINE_PACKAGE].kit_num));
			DrawMenuOption(std::format("CARSLOTID_FORCED_INDUCTION_PACKAGE {}", ply->GetCustomizations()->InstalledParts[CARSLOTID_FORCED_INDUCTION_PACKAGE].kit_num));
			DrawMenuOption(std::format("CARSLOTID_NITROUS_PACKAGE {}", ply->GetCustomizations()->InstalledParts[CARSLOTID_NITROUS_PACKAGE].kit_num));
			DrawMenuOption(std::format("CARSLOTID_SUSPENSION_PACKAGE {}", ply->GetCustomizations()->InstalledParts[CARSLOTID_SUSPENSION_PACKAGE].kit_num));
			DrawMenuOption(std::format("CARSLOTID_TIRE_PACKAGE {}", ply->GetCustomizations()->InstalledParts[CARSLOTID_TIRE_PACKAGE].kit_num));

			DrawMenuOption(std::format("pEngine {:X}", (uintptr_t)pEngine));
			DrawMenuOption(std::format("ITransmission {:X}", (uintptr_t)pEngine->GetITransmission()));
			DrawMenuOption(std::format("mGear {}", pEngine->mGear));
			DrawMenuOption(std::format("GetTopGear {}", (int)pEngine->GetTopGear()));
			DrawMenuOption(std::format("mTransmissionVelocity {:.2f}", pEngine->mTransmissionVelocity));
			DrawMenuOption(std::format("GetDriveTorque {:.2f}", pEngine->GetDriveTorque()));
			DrawMenuOption(std::format("GetSpeedometer {:.2f}", pEngine->GetSpeedometer()));
			DrawMenuOption(std::format("CalcSpeedometer {:.2f}", pEngine->CalcSpeedometer(RPS2RPM(pEngine->mTransmissionVelocity), pEngine->mGear)));
			DrawMenuOption(std::format("GetMaxSpeedometer {:.2f}", pEngine->GetMaxSpeedometer()));
			DrawMenuOption(std::format("IsGearChanging {}", pEngine->IsGearChanging()));
			DrawMenuOption(std::format("mEngineBraking {}", pEngine->mEngineBraking));
			DrawMenuOption(std::format("IDLE {:.2f}", pEngine->mMWInfo->IDLE));
			DrawMenuOption(std::format("TORQUE.size() {}", pEngine->mMWInfo->TORQUE.size()));
			DrawMenuOption(std::format("mSuspension {:X}", (uintptr_t)pEngine->mSuspension));
			DrawMenuOption(std::format("mIInput {:X}", (uintptr_t)pEngine->mIInput));
			DrawMenuOption(std::format("actual mSuspension {:X}", (uintptr_t)pEngine->mVehicle->mCOMObject->Find<IChassis>()));
			DrawMenuOption(std::format("actual mIInput {:X}", (uintptr_t)pEngine->mVehicle->mCOMObject->Find<IInput>()));
			DrawMenuOption(std::format("pSuspension {:X}", (uintptr_t)pSuspension));
			DrawMenuOption(std::format("GetControlGas {}", pEngine->mIInput->GetControlGas()));
		}
		else {
			DrawMenuOption("woof?");
		}

		ChloeMenuLib::EndMenu();
	}
	if (DrawMenuOption("SuspensionRacer")) {
		ChloeMenuLib::BeginMenu();

	if (pSuspension) {
		ISteeringWheel::SteeringType steer_type = ISteeringWheel::kGamePad;

		if (IPlayer *player = PLAYER_LIST::GetList(PLAYER_LOCAL)[0]) {
			ISteeringWheel *device = player->GetSteeringDevice();

			if (device && device->IsConnected()) {
				steer_type = device->GetSteeringType();
			}
		}
		DrawMenuOption(std::format("steer_type {}", (int)steer_type));

		if (auto veh = VEHICLE_LIST::GetList(VEHICLE_PLAYERS)[0]) {
			DrawMenuOption(std::format("state.speed {:.2f}", LastChassisState.speed));
			DrawMenuOption(std::format("GetSpeedometer() {:.2f}", veh->mCOMObject->Find<ITransmission>()->GetSpeedometer()));
			DrawMenuOption(std::format("GetMaxSpeedometer() {:.2f}", veh->mCOMObject->Find<ITransmission>()->GetMaxSpeedometer()));
			DrawMenuOption(std::format("GetPerfectLaunch() {:.2f}", veh->GetPerfectLaunch()));
		}

		//QuickValueEditor("BrakesAtValue", UNDERCOVER_BrakesAtValue);
		//QuickValueEditor("StaticGripAtValue", UNDERCOVER_StaticGripAtValue);
		//QuickValueEditor("RollCenterAtValue", UNDERCOVER_RollCenterAtValue);
		//QuickValueEditor("AeroCGAtValue", UNDERCOVER_AeroCGAtValue);
		//QuickValueEditor("AeroCoeffAtValue", UNDERCOVER_AeroCoeffAtValue);
		//QuickValueEditor("SuspensionAtValue", UNDERCOVER_SuspensionAtValue);
		//QuickValueEditor("SteeringAtValue", UNDERCOVER_SteeringAtValue);

		DrawMenuOption(std::format("state.inertia {:.2f} {:.2f} {:.2f}", LastChassisState.inertia.x, LastChassisState.inertia.y, LastChassisState.inertia.z));
		DrawMenuOption(std::format("state.dimension {:.2f} {:.2f} {:.2f}", LastChassisState.dimension.x, LastChassisState.dimension.y, LastChassisState.dimension.z));
		DrawMenuOption(std::format("state.cog {:.2f} {:.2f} {:.2f}", LastChassisState.cog.x, LastChassisState.cog.y, LastChassisState.cog.z));
		DrawMenuOption(std::format("state.speed {:.2f}", LastChassisState.speed));
		DrawMenuOption(std::format("state.slipangle {:.2f}", LastChassisState.slipangle));
		DrawMenuOption(std::format("state.ground_effect {:.2f}", LastChassisState.ground_effect));
		DrawMenuOption(std::format("state.time {:.2f}", LastChassisState.time));

		//DrawMenuOption(std::format("state.flags - {}", LastChassisState.flags));
		//DrawMenuOption(std::format("state.time - {}", LastChassisState.time));
		//DrawMenuOption(std::format("state.mass - {}", LastChassisState.mass));
		//DrawMenuOption(std::format("state.local_vel - {:.2f} {:.2f} {:.2f}", LastChassisState.local_vel.x, LastChassisState.local_vel.y, LastChassisState.local_vel.z));
		//DrawMenuOption(std::format("state.linear_vel - {:.2f} {:.2f} {:.2f}", LastChassisState.linear_vel.x, LastChassisState.linear_vel.y, LastChassisState.linear_vel.z));
		//DrawMenuOption(std::format("state.speed - {:.2f}", LastChassisState.speed));
		DrawMenuOption(std::format("state.steer_input - {:.2f}", LastChassisState.steer_input));
		DrawMenuOption(std::format("state.nos_boost - {:.2f}", LastChassisState.nos_boost));
		DrawMenuOption(std::format("state.shift_boost - {:.2f}", LastChassisState.shift_boost));
		DrawMenuOption(std::format("mDrift.State - {}", (int)pSuspension->mDrift.State));
		DrawMenuOption(std::format("mDrift.Value - {:.2f}", pSuspension->mDrift.Value));
		DrawMenuOption(std::format("mBurnOut.Traction - {:.2f}", pSuspension->mBurnOut.GetTraction()));
		DrawMenuOption(std::format("mBurnOut.State - {}", pSuspension->mBurnOut.GetState()));
		//DrawMenuOption(std::format("MaxSlip - {:.2f}", pSuspension->ComputeMaxSlip(LastChassisState)));
		//DrawMenuOption(std::format("MaxSteering - {:.2f}", pSuspension->CalculateMaxSteering(LastChassisState, ISteeringWheel::kGamePad)));
		DrawMenuOption(std::format("LateralGripScale - {:.2f}", pSuspension->ComputeLateralGripScale(LastChassisState)));
		//DrawMenuOption(std::format("TractionScale - {:.2f}", pSuspension->ComputeTractionScale(LastChassisState)));
		DrawMenuOption(std::format("Wheels - {:.2f} {:.2f}", pSuspension->mSteering.Wheels[0], pSuspension->mSteering.Wheels[1]));
		DrawMenuOption(std::format("LastMaximum - {:.2f}", pSuspension->mSteering.LastMaximum));
		//DrawMenuOption(std::format("mGameBreaker - {:.2f}", pSuspension->mGameBreaker));

		for (int i = 0; i < 4; i++) {
			auto tire = pSuspension->mTires[i];
			DrawMenuOption(std::format("Tire {}", i+1));
			DrawMenuOption(std::format("fNormal - {:.2f} {:.2f} {:.2f} {:.2f}", tire->mNormal.x, tire->mNormal.y, tire->mNormal.z, tire->mNormal.w));
			DrawMenuOption(std::format("mCompression - {:.2f}", tire->mCompression));
			DrawMenuOption(std::format("mLateralSpeed - {:.2f}", tire->mLateralSpeed));
			DrawMenuOption(std::format("mForce - {:.2f} {:.2f} {:.2f}", tire->mForce.x, tire->mForce.y, tire->mForce.z));
			DrawMenuOption(std::format("mLongitudeForce - {:.2f}", tire->mLongitudeForce));
			DrawMenuOption(std::format("mTractionBoost - {:.2f}", tire->mTractionBoost));
			DrawMenuOption(std::format("mLateralBoost - {:.2f}", tire->mLateralBoost));
			DrawMenuOption(std::format("mDriftFriction - {:.2f}", tire->mDriftFriction));
			DrawMenuOption(std::format("mGripBoost - {:.2f}", tire->mGripBoost));
			DrawMenuOption(std::format("mTraction - {:.2f}", tire->mTraction));
			DrawMenuOption(std::format("mSlip - {:.2f}", tire->mSlip));
			DrawMenuOption(std::format("mRadius - {:.2f}", tire->mRadius));
			DrawMenuOption(std::format("mRoadSpeed - {:.2f}", tire->mRoadSpeed));
			DrawMenuOption(std::format("mAV - {:.2f}", tire->mAV));
		}
	}
	else {
		DrawMenuOption("woof?");
	}

		ChloeMenuLib::EndMenu();
	}

	ChloeMenuLib::EndMenu();
}

auto oldctorbase = (void*(__thiscall*)(void*, BehaviorParams*, int))0x6DB670;
SuspensionSimpleMW* SuspensionSimpleConstruct(BehaviorParams* bp) {
	auto data = (SuspensionSimpleMW*)gFastMem.Alloc(sizeof(SuspensionSimpleMW), nullptr);
	memset(data,0,sizeof(SuspensionSimpleMW));
	oldctorbase(data, bp, 0);
	data->Create(*bp);
	return data;
}

SuspensionRacerMW* SuspensionRacerConstruct(BehaviorParams* bp) {
	auto data = pSuspension = (SuspensionRacerMW*)gFastMem.Alloc(sizeof(SuspensionRacerMW), nullptr);
	memset(data,0,sizeof(SuspensionRacerMW));
	oldctorbase(data, bp, 0);
	data->Create(*bp);
	return data;
}

EngineRacer* EngineRacerConstruct(BehaviorParams* bp) {
	auto data = pEngine = (EngineRacer*)gFastMem.Alloc(sizeof(EngineRacer), nullptr);
	memset(data,0,sizeof(EngineRacer));
	oldctorbase(data, bp, 0);
	data->Create(*bp);
	return data;
}

class FactoryEntry {
public:
	UCrc32 mSignature;
	void* mConstructor;
	FactoryEntry *mTail;

	static inline auto& mHead = *(FactoryEntry**)0xDE7130;

	FactoryEntry(const char* name, void* function) {
		mSignature.mCRC = Attrib::StringHash32(name);
		mConstructor = function;
		mTail = FactoryEntry::mHead;
		FactoryEntry::mHead = this;
	}
};
FactoryEntry __EngineRacerMW("EngineRacerMW", (void*)&EngineRacerConstruct);
FactoryEntry __SuspensionRacerMW("SuspensionRacerMW", (void*)&SuspensionRacerConstruct);
FactoryEntry __SuspensionSimpleMW("SuspensionSimpleMW", (void*)&SuspensionSimpleConstruct);

void AssistLoop() {
	auto list = VEHICLE_LIST::GetList(VEHICLE_PLAYERS);
	if (list.empty()) return;

	auto ply = list[0];
	for (int i = 0; i < DRIVER_AID_NUMBER; i++) {
		if (ply->GetDriverAidLevel((DriverAidType)i) == 0) continue;
		ply->SetDriverAidLevel((DriverAidType)i, 0);
	}
}

std::vector<Attrib::Collection*> FindCollectionAndAllChildren(const char* className, const char* name) {
	std::vector<Attrib::Collection*> out;

	auto parent = Attrib::FindCollection(Attrib::StringHash32(className), Attrib::StringHash32(name));
	if (!parent) return out;
	out.push_back(parent);

	auto classId = Attrib::ClassTable::Find(&Attrib::Database::sThis->mPrivates->mClasses, Attrib::StringHash32(className));
	auto pClass = Attrib::Database::sThis->mPrivates->mClasses.mTable[classId].mPtr;
	auto collHash = pClass->GetFirstCollection();
	while (collHash) {
		auto childCollection = Attrib::FindCollection(Attrib::StringHash32(className), collHash);
		if (childCollection->mParent == parent) {
			out.push_back(childCollection);
		}
		collHash = pClass->GetNextCollection(collHash);
	}

	return out;
}

UCrc32* __thiscall LookupBehaviorSignatureHooked(PVehicle* pThis, UCrc32* result, const Attrib::StringKey* mechanic) {
	pThis->LookupBehaviorSignature(result, mechanic);
	bool isCorrectDriverClass = pThis->mDriverClass == DRIVER_HUMAN;
	if (bAffectOpponents && pThis->mDriverClass == DRIVER_RACER) isCorrectDriverClass = true;
	if (bAffectCops && pThis->mDriverClass == DRIVER_COP) isCorrectDriverClass = true;
	if (isCorrectDriverClass && pThis->mClass.mCRC != VehicleClass::CHOPPER.mCRC) {
		if (mechanic == &BEHAVIOR_MECHANIC_ENGINE) {
			*result = __EngineRacerMW.mSignature;
			return result;
		}
		if (mechanic == &BEHAVIOR_MECHANIC_SUSPENSION) {
			// only enable simple physics for the player, ai steering is broken
			*result = (result->mCRC == Attrib::StringHash32("ChassisSimple") && pThis->mDriverClass == DRIVER_HUMAN) ? __SuspensionSimpleMW.mSignature : __SuspensionRacerMW.mSignature;
			return result;
		}
	}
	return result;
}

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {
	switch( fdwReason ) {
		case DLL_PROCESS_ATTACH: {
			if (NyaHookLib::GetEntryPoint() != 0x4AEC55) {
				MessageBoxA(nullptr, "Unsupported game version! Make sure you're using v1.0.0.1 (.exe size of 10584064 or 10589456 bytes)", "nya?!~", MB_ICONERROR);
				return TRUE;
			}

			GetCurrentDirectoryW(MAX_PATH, gDLLDir);

			NyaHooks::LateInitHook::Init();
			NyaHooks::LateInitHook::aFunctions.push_back([](){
				DLLDirSetter _setdir;

				for (const auto& entry : std::filesystem::directory_iterator("CarDataDump")) {
					if (entry.is_directory()) continue;

					auto filename = entry.path().filename().string();
					if (!filename.ends_with(".conf")) continue;

					auto tuning = LoadCarTuningFromFile(filename);
					if (!tuning) {
						WriteLog(std::format("Failed to load {}", filename));
						continue;
					}

					auto collections = FindCollectionAndAllChildren("car_tuning", tuning->carName.c_str());
					for (auto collection : collections) {
						auto f = (float*)Attrib::Collection::GetData(collection, Attrib::StringHash32("TENSOR_SCALE"), 0);
						f[0] = tuning->TENSOR_SCALE[0];
						f[1] = tuning->TENSOR_SCALE[1];
						f[2] = tuning->TENSOR_SCALE[2];
					}
				}
			});
			NyaHooks::WorldServiceHook::Init();
			NyaHooks::WorldServiceHook::aPreFunctions.push_back(AssistLoop);

			ChloeMenuLib::RegisterMenu("MW Physics Debug Menu", &DebugMenu);

			NyaHookLib::PatchRelative(NyaHookLib::CALL, 0x6F077A, &LookupBehaviorSignatureHooked);
			NyaHookLib::PatchRelative(NyaHookLib::CALL, 0x6F07D2, &LookupBehaviorSignatureHooked);

			// AIVehicle::GetOverSteerCorrection, disable road surface getter during race cutscenes
			NyaHookLib::PatchRelative(NyaHookLib::JMP, 0x40AAC8, 0x40AB89);
			NyaHookLib::Patch<uint16_t>(0x40AAC2, 0xE8D9); // fld1

			if (std::filesystem::exists("NFSUCMWPhysics_gcp.toml")) {
				auto config = toml::parse_file("NFSUCMWPhysics_gcp.toml");
				bAffectOpponents = config["mw_physics_opponents"].value_or(bAffectOpponents);
				bAffectCops = config["mw_physics_cops"].value_or(bAffectCops);
				bRevLimiter = config["rev_limiter"].value_or(bRevLimiter);
			}

			WriteLog("Mod initialized");
		} break;
		default:
			break;
	}
	return TRUE;
}