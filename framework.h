#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#define _USE_MATH_DEFINES
// Windows Header Files
#include <windows.h>
#include <cstdint>
#include <cstddef>
#include <string>
#include <fstream>
#include <format>

#include "nya_commonhooklib.h"
#include "nya_commonmath.h"
#include "nfsuc.h"

#ifdef _MSC_VER
#define __builtin_return_address(x) (_ReturnAddress())
#endif

inline void WriteLog(const std::string& str) {
	static auto file = std::ofstream("NFSUCMWPhysics_gcp.log");

	file << str;
	file << "\n";
	file.flush();
}

#define FUNCTION_LOG(name) WriteLog(std::format("{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)));
//#define ICHASSIS_FUNCTION_LOG(name) WriteLog(std::format("IChassis::{} called from {:X}", name, (uintptr_t)__builtin_return_address(0)))
#define ICHASSIS_FUNCTION_LOG(name) {}

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

inline wchar_t gDLLDir[MAX_PATH];
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


BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID);
