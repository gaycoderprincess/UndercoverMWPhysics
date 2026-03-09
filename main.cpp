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

#define FUNCTION_LOG(name) WriteLog(std::format("{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)));
#define ICHASSIS_FUNCTION_LOG(name) {}

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
	float Atan2a(float a, float b) { return std::atan2(a, b); }

	inline void Cross(const Vector3 &a, const Vector3 &b, Vector3 &r) {
		r.x = a.y * b.z - a.z * b.y;
		r.y = a.z * b.x - a.x * b.z;
		r.z = a.x * b.y - a.y * b.x;
	}

	inline float Atan2d(float o, float a) {
		return ANGLE2DEG(std::atan2(o, a));
	}

	inline void RotateTranslate(const Vector3 &v, const Matrix4 &m, Vector3 &result) {
		result.x = ((m.x.x * v.x) + ((m.z.x * v.z) + (m.y.x * v.y))) + m.p.x;
		result.y = ((m.x.y * v.x) + ((m.z.y * v.z) + (m.y.y * v.y))) + m.p.y;
		result.z = ((m.x.z * v.x) + ((m.z.z * v.z) + (m.y.z * v.y))) + m.p.z;
	}

	inline void RotateTranslate(const Vector4 &v, const Matrix4 &m, Vector4 &result) {
		auto v4 = v.y;
		auto v5 = v.z;
		auto v6 = v.w;
		auto v3 = v.x;
		result.x = (m.x.x * v.x) + ((m.y.x * v4) + ((m.p.x * v6) + (m.z.x * v5)));
		result.y = (m.x.y * v3) + ((m.y.y * v4) + ((m.p.y * v6) + (m.z.y * v5)));
		result.z = (m.x.z * v3) + ((m.y.z * v4) + ((m.p.z * v6) + (m.z.z * v5)));
		result.w = (m.xw * v3) + ((m.yw * v4) + ((m.pw * v6) + (m.zw * v5)));
	}

	inline void Unit(const Vector3 &a, Vector3 &r) {
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
	void UnitCross(const Vector3 &a, const Vector3 &b, Vector3 &r) {
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

	inline void Rotate(const Vector3 &a, const Matrix4 &m, Vector3 &r) {
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

	inline void ScaleAdd(const Vector3 &a, const float s, const Vector3 &b, Vector3 &r) {
		r.x = a.x + b.x * s;
		r.y = a.y + b.y * s;
		r.z = a.z + b.z * s;
	}

	inline float Ramp(const float a, const float amin, const float amax) {
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

#include "decomp/AverageWindow.h"
#include "decomp/SuspensionRacer.h"
#include "decomp/MWChassis.cpp"
#include "decomp/SuspensionRacer.cpp"

SuspensionRacer* pSuspension = nullptr;
void DebugMenu() {
	ChloeMenuLib::BeginMenu();

	if (pSuspension) {
		DrawMenuOption(std::format("Wheels - {:.2f} {:.2f}", pSuspension->mSteering.Wheels[0], pSuspension->mSteering.Wheels[1]));

		auto tire = pSuspension->mTires[0];
		DrawMenuOption(std::format("fHeight[0] - {:.2f}", tire->mWorldPos.fHeight));
		DrawMenuOption(std::format("fNormal - {:.2f} {:.2f} {:.2f} {:.2f}", tire->mNormal.x, tire->mNormal.y, tire->mNormal.z, tire->mNormal.w));
		DrawMenuOption(std::format("mCompression - {:.2f}", tire->mCompression));
		DrawMenuOption(std::format("mLateralSpeed - {:.2f}", tire->mLateralSpeed));
		DrawMenuOption(std::format("mForce - {:.2f} {:.2f} {:.2f}", tire->mForce.x, tire->mForce.y, tire->mForce.z));
	}
	else {
		DrawMenuOption("woof?");
	}

	ChloeMenuLib::EndMenu();
}

auto oldctor = (void*(__thiscall*)(void*, BehaviorParams*, SuspensionParams*))0x73CEA0;
auto oldctorbase = (void*(__thiscall*)(void*, BehaviorParams*, int))0x6DB670;
SuspensionRacer* __thiscall ChassisHumanConstructHooked(BehaviorParams* bp) {
	auto data = pSuspension = (SuspensionRacer*)gFastMem.Alloc(sizeof(SuspensionRacer), nullptr);
	memset(data,0,sizeof(SuspensionRacer));
	oldctorbase(data, bp, 0);
	data->Create(*bp);
	return data;
}


void __attribute__((naked)) ChassisHumanConstructASM() {
	__asm__ (
		//"push EAX\n\t"
		"push ECX\n\t"
		"mov ecx, [esp+8]\n\t"
		"push EDX\n\t"
		"push EBX\n\t"
		"push EBP\n\t"
		"push ESI\n\t"
		"push EDI\n\t"
		"call %0\n\t"
		"pop EDI\n\t"
		"pop ESI\n\t"
		"pop EBP\n\t"
		"pop EBX\n\t"
		"pop EDX\n\t"
		"pop ECX\n\t"
		//"pop EAX\n\t"
		"ret\n\t"
			:
			:  "i" (ChassisHumanConstructHooked)
	);
}

// first crash:
// GetPriority called from 6E6E54

BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID) {
	switch( fdwReason ) {
		case DLL_PROCESS_ATTACH: {
			if (NyaHookLib::GetEntryPoint() != 0x4AEC55) {
				MessageBoxA(nullptr, "Unsupported game version! Make sure you're using v1.0.0.1 (.exe size of 10584064 or 10589456 bytes)", "nya?!~", MB_ICONERROR);
				return TRUE;
			}

			ChloeMenuLib::RegisterMenu("Debug Menu", &DebugMenu);

			//NyaHookLib::PatchRelative(NyaHookLib::CALL, 0x73F88D, 0x6DB670);
			NyaHookLib::PatchRelative(NyaHookLib::JMP, 0x73F830, &ChassisHumanConstructASM);

			WriteLog("Mod initialized");
		} break;
		default:
			break;
	}
	return TRUE;
}