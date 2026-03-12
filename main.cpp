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

#define FUNCTION_LOG(name) WriteLog(std::format("{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)));
//#define ICHASSIS_FUNCTION_LOG(name) WriteLog(std::format("IChassis::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)))
#define ICHASSIS_FUNCTION_LOG(name) {}
#define ITIPTRONIC_FUNCTION_LOG(name) WriteLog(std::format("ITiptronic::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)))
#define IRACEENGINE_FUNCTION_LOG(name) WriteLog(std::format("IRaceEngine::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)))
#define IENGINEDAMAGE_FUNCTION_LOG(name) WriteLog(std::format("IEngineDamage::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)))
#define IINDUCTABLE_FUNCTION_LOG(name) WriteLog(std::format("IInductable::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)))
#define ITRANSMISSION_FUNCTION_LOG(name) WriteLog(std::format("ITransmission::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)))
#define IENGINE_FUNCTION_LOG(name) WriteLog(std::format("IEngine::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)))

#include "decomp/ConversionUtil.hpp"

auto cartuning_LookupKey = (uint32_t(__thiscall*)(Attrib::Gen::car_tuning*, const ISimable*, int))0x721E20;
auto ctor_cartuning = (void(__thiscall*)(Attrib::Gen::car_tuning*, uint32_t))0x721CB0;

#define FLOAT_EPSILON 0.000001f

namespace VehicleSystem {
	float ENABLE_ROLL_STOPS_THRESHOLD = 0.2f;
};

inline bool IsFront(unsigned int i) {
	return i < 2;
}

inline bool IsRear(unsigned int i) {
	return i > 1;
}

inline int bClamp(int a, int MINIMUM, int MAXIMUM) {
	return std::min(std::max(a, MINIMUM), MAXIMUM);
}

inline float bClamp(float a, float MINIMUM, float MAXIMUM) {
	return std::min(MAXIMUM, std::max(a, MINIMUM));
}

namespace UMath {
	inline Vector4 Vector4Make(const Vector3 &c, float w) {
		Vector4 res;
		res.x = c.x;
		res.y = c.y;
		res.z = c.z;
		res.w = w;
		return res;
	}

	inline Vector3 Vector4To3(const Vector4 &c) {
		return {c.x,c.y,c.z};
	}

	float Abs(float f) { return std::abs(f); }
	float Min(float a, float b) { return std::min(a, b); }
	float Max(float a, float b) { return std::max(a, b); }
	float Lerp(float a, float b, float c) { return std::lerp(a, b, c); }
	float Sina(float a) { return std::sin(a * (std::numbers::pi*2)); }
	float Sqrt(float a) { return std::sqrt(a); }
	float Pow(float a, float b) { return std::pow(a, b); }
	float Pow(int a, int b) { return std::pow(a, b); }
	float Atan2a(float a, float b) { return std::atan2(a, b) / (std::numbers::pi*2); }

	inline void Cross(Vector3 a, Vector3 b, Vector3 &r) {
		r.x = a.y * b.z - a.z * b.y;
		r.y = a.z * b.x - a.x * b.z;
		r.z = a.x * b.y - a.y * b.x;
	}

	inline float Atan2d(float o, float a) {
		return ANGLE2DEG(Atan2a(o, a));
	}

	inline void RotateTranslate(Vector3 v, Matrix4 m, Vector3 &result) {
		result.x = ((m.x.x * v.x) + ((m.z.x * v.z) + (m.y.x * v.y))) + m.p.x;
		result.y = ((m.x.y * v.x) + ((m.z.y * v.z) + (m.y.y * v.y))) + m.p.y;
		result.z = ((m.x.z * v.x) + ((m.z.z * v.z) + (m.y.z * v.y))) + m.p.z;
	}

	inline void Unit(Vector3 a, Vector3 &r) {
		auto len = a.length();
		if (len != 0.0) {
			r.x = a.x / len;
			r.y = a.y / len;
			r.z = a.z / len;
		}
		else {
			r = {0,0,0};
		}
	}

	// todo is this correct
	void UnitCross(Vector3 a, Vector3 b, Vector3 &r) {
		r.x = a.y * b.z - a.z * b.y;
		r.y = a.z * b.x - a.x * b.z;
		r.z = a.x * b.y - a.y * b.x;
		Unit(r, r);
	}

	int Clamp(const int a, const int amin, const int amax) {
		return a < amin ? amin : (a > amax ? amax : a);
	}

	float Clamp(const float a, const float amin, const float amax) {
		return a < amin ? amin : (a > amax ? amax : a);
	}

	inline float LengthSquare(const Vector3 &a) {
		return a.x * a.x + a.y * a.y + a.z * a.z;
	}

	inline float Length(const Vector3 &a) {
		return std::sqrt(LengthSquare(a));
	}

	inline float Lengthxz(const Vector3 &a) {
		auto tmp = a;
		tmp.y = 0;
		return tmp.length();
	}

	inline void Scale(const Vector3 &a, const Vector3 &b, Vector3 &r) {
		r.x = a.x * b.x;
		r.y = a.y * b.y;
		r.z = a.z * b.z;
	}

	inline void Scale(const Vector3 &a, const float s, Vector3 &r) {
		r.x = a.x * s;
		r.y = a.y * s;
		r.z = a.z * s;
	}

	inline float Dot(const Vector3 &a, const Vector3 &b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	inline void Rotate(Vector3 a, Matrix4 m, Vector3 &r) {
		Vector3 temp = a;

		r.x = m.x.x * temp.x + m.y.x * temp.y + m.z.x * temp.z;
		r.y = m.x.y * temp.x + m.y.y * temp.y + m.z.y * temp.z;
		r.z = m.x.z * temp.x + m.y.z * temp.y + m.z.z * temp.z;
	}

	inline void Add(const Vector3 &a, const Vector3 &b, Vector3 &r) {
		r.x = a.x + b.x;
		r.y = a.y + b.y;
		r.z = a.z + b.z;
	}

	inline void Sub(const Vector3 &a, const Vector3 &b, Vector3 &r) {
		r.x = a.x - b.x;
		r.y = a.y - b.y;
		r.z = a.z - b.z;
	}

	//UMath::ScaleAdd((UMath::Vector3)state.matrix.y, counter_yaw - yaw, total_torque, total_torque);
	//total_torque.x = (counter_yaw - yaw) * state.matrix.y.x + total_torque.x;
	inline void ScaleAdd(const Vector3 &a, const float s, const Vector3 &b, Vector3 &r) {
		r.x = s * a.x + b.x;
		r.y = s * a.y + b.y;
		r.z = s * a.z + b.z;
	}

	inline float Ramp(const float a, const float amin, const float amax) {
		//auto v3 = amax - amin;
		//if ( v3 <= 0.000001 )
		//	return 0.0;
		//auto v5 = (a - amin) / v3;
		//if ( v5 >= 1.0 )
		//	return 1.0;
		//auto result = v5;
		//if ( v5 < 0.0 )
		//	return 0.0;
		//return result;

		//auto v2 = 1.0;
		//if ( ((a - amin) / (amax - amin)) < 1.0 )
		//	v2 = ((a - amin) / (amax - amin));
		//auto v3 = v2;
		//if ( v2 < 0.0 )
		//	v3 = 0.0;
		//return v3;

		float arange = amax - amin;
		return arange > FLOAT_EPSILON ? std::max(0.0f, std::min((a - amin) / arange, 1.0f)) : 0.0f;
	}

	// Credits: Brawltendo
	inline float Limit(const float a, const float l) {
		float retval;
		if (!(a * l > 0.f)) {
			retval = a;
		} else {
			if (a > 0.f) {
				retval = Min(a, l);

			} else {
				retval = Max(a, l);
			}
		}
		return retval;
	}
}

// Credits: Brawltendo
float Table::GetValue(float input) {
	const int entries = NumEntries;
	const float normarg = IndexMultiplier * (input - MinArg);
	const int index = (int)normarg;

	if (index < 0 || normarg < 0.0f)
		return pTable[0];
	if (index >= (entries - 1))
		return pTable[entries - 1];

	float ind = index;
	if (ind > normarg)
		ind -= 1.0f;

	float delta = normarg - ind;
	return (1.0f - delta) * pTable[index] + delta * pTable[index + 1];
}

std::vector<float> UNDERCOVER_YawControl = { 0.1, 0.2, 0.65, 1 };

#define GET_FAKE_INTERFACE(base, type, var) { auto ptr = (uintptr_t)this; ptr += offsetof(base, var); return (type*)ptr; }

#include "MWCarTuning.h"
#include "decomp/AverageWindow.h"
#include "decomp/EngineRacer.h"
#include "decomp/SuspensionRacer.h"
#include "MWCarTuning.cpp"
#include "decomp/MWChassis.cpp"
#include "decomp/MWRaceEngine.cpp"
#include "decomp/MWTiptronic.cpp"
#include "decomp/MWEngineDamage.cpp"
#include "decomp/MWInductable.cpp"
#include "decomp/MWTransmission.cpp"
#include "decomp/MWEngine.cpp"
#include "decomp/SuspensionRacer.cpp"
#include "decomp/EngineRacer.cpp"

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

SuspensionRacer* pSuspension = nullptr;
EngineRacer* pEngine = nullptr;
void DebugMenu() {
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
		}
	}
	else {
		DrawMenuOption("woof?");
	}

	ChloeMenuLib::EndMenu();
}

auto oldctorbase = (void*(__thiscall*)(void*, BehaviorParams*, int))0x6DB670;
SuspensionRacer* ChassisHumanConstructHooked(BehaviorParams* bp) {
	auto data = pSuspension = (SuspensionRacer*)gFastMem.Alloc(sizeof(SuspensionRacer), nullptr);
	memset(data,0,sizeof(SuspensionRacer));
	oldctorbase(data, bp, 0);
	data->Create(*bp);
	return data;
}

EngineRacer* EngineRacerConstructHooked(BehaviorParams* bp) {
	auto data = pEngine = (EngineRacer*)gFastMem.Alloc(sizeof(EngineRacer), nullptr);
	memset(data,0,sizeof(EngineRacer));
	oldctorbase(data, bp, 0);
	data->Create(*bp);
	return data;
}

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

void __thiscall GetAttribHooked(Attrib::Instance* pThis, Attrib::Collection* collection, uint32_t msgPort) {
	auto tmp = (uintptr_t)collection;
	if (tmp && tmp < 0x1000) {
		WriteLog(std::format("collection {:X} from {:X}", tmp, (uintptr_t)__builtin_return_address(0)));
		collection = nullptr;
	}

	pThis->mCollection = collection;
	pThis->mMsgPort = msgPort;
	pThis->mLayoutPtr = nullptr;
	pThis->mFlags = 0;
	if (collection) {
		if (!collection->mSource) {
			pThis->mFlags = 1;
		}
		pThis->mLayoutPtr = collection->mLayout;
		++collection->mTable.mRefCount;
	}
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

			ChloeMenuLib::RegisterMenu("Debug Menu", &DebugMenu);

			LoadCarTuningFromFile("orig/bmwm3gtre46_top");

			//NyaHookLib::PatchRelative(NyaHookLib::CALL, 0x73F88D, 0x6DB670);
			NyaHookLib::PatchRelative(NyaHookLib::JMP, 0x73F830, &ChassisHumanConstructHooked);
			NyaHookLib::PatchRelative(NyaHookLib::JMP, 0x73EC60, &EngineRacerConstructHooked);
			NyaHookLib::PatchRelative(NyaHookLib::JMP, 0x462C80, &GetAttribHooked);

			// AIVehicle::GetOverSteerCorrection, disable road surface getter during race cutscenes
			NyaHookLib::PatchRelative(NyaHookLib::JMP, 0x40AAC8, 0x40AB89);
			NyaHookLib::Patch<uint16_t>(0x40AAC2, 0xE8D9); // fld1

			WriteLog("Mod initialized");
		} break;
		default:
			break;
	}
	return TRUE;
}