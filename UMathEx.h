#ifndef UMATHEX_H
#define UMATHEX_H

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning(disable: 4244) // conversion from 'const double' to 'T', possible loss of data
#endif

#include "framework.h"
#include "types/UMath.h"
#include "decomp/ConversionUtil.hpp"

#include <numbers>

#define FLOAT_EPSILON 0.000001f

namespace UMath {
	inline Vector4 Vector4Make(const Vector3& c, float w) {
		Vector4 res;
		res.x = c.x;
		res.y = c.y;
		res.z = c.z;
		res.w = w;
		return res;
	}

	inline Vector3 Vector4To3(const Vector4& c) {
		return { c.x,c.y,c.z };
	}

	inline float Abs(float f) {
		return std::abs(f);
	}
	inline float Min(float a, float b) {
		return std::min(a, b);
	}
	inline float Max(float a, float b) {
		return std::max(a, b);
	}
	inline float Lerp(float a, float b, float c) {
		return std::lerp(a, b, c);
	}
	inline float Sina(float a) {
		return std::sin(a * (std::numbers::pi * 2));
	}
	inline float Sqrt(float a) {
		return std::sqrt(a);
	}
	inline float Pow(float a, float b) {
		return std::pow(a, b);
	}
	inline float Pow(int a, int b) {
		return std::pow(a, b);
	}
	inline float Atan2a(float a, float b) {
		return std::atan2(a, b) / (std::numbers::pi * 2);
	}

	inline void Cross(Vector3 a, Vector3 b, Vector3& r) {
		r.x = a.y * b.z - a.z * b.y;
		r.y = a.z * b.x - a.x * b.z;
		r.z = a.x * b.y - a.y * b.x;
	}

	inline float Atan2d(float o, float a) {
		return ANGLE2DEG(Atan2a(o, a));
	}

	inline void RotateTranslate(Vector3 v, Matrix4 m, Vector3& result) {
		result.x = ((m.x.x * v.x) + ((m.z.x * v.z) + (m.y.x * v.y))) + m.p.x;
		result.y = ((m.x.y * v.x) + ((m.z.y * v.z) + (m.y.y * v.y))) + m.p.y;
		result.z = ((m.x.z * v.x) + ((m.z.z * v.z) + (m.y.z * v.y))) + m.p.z;
	}

	inline void Unit(Vector3 a, Vector3& r) {
		auto len = a.length();
		if (len != 0.0) {
			r.x = a.x / len;
			r.y = a.y / len;
			r.z = a.z / len;
		}
		else {
			r = { 0,0,0 };
		}
	}

	// todo is this correct
	inline void UnitCross(Vector3 a, Vector3 b, Vector3& r) {
		r.x = a.y * b.z - a.z * b.y;
		r.y = a.z * b.x - a.x * b.z;
		r.z = a.x * b.y - a.y * b.x;
		Unit(r, r);
	}

	inline int Clamp(const int a, const int amin, const int amax) {
		return a < amin ? amin : (a > amax ? amax : a);
	}

	inline float Clamp(const float a, const float amin, const float amax) {
		return a < amin ? amin : (a > amax ? amax : a);
	}

	inline float LengthSquare(const Vector3& a) {
		return a.x * a.x + a.y * a.y + a.z * a.z;
	}

	inline float Length(const Vector3& a) {
		return std::sqrt(LengthSquare(a));
	}

	inline float Lengthxz(const Vector3& a) {
		auto tmp = a;
		tmp.y = 0;
		return tmp.length();
	}

	inline void Scale(const Vector3& a, const Vector3& b, Vector3& r) {
		r.x = a.x * b.x;
		r.y = a.y * b.y;
		r.z = a.z * b.z;
	}

	inline void Scale(const Vector3& a, const float s, Vector3& r) {
		r.x = a.x * s;
		r.y = a.y * s;
		r.z = a.z * s;
	}

	inline float Dot(const Vector3& a, const Vector3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	inline void Rotate(Vector3 a, Matrix4 m, Vector3& r) {
		Vector3 temp = a;

		r.x = m.x.x * temp.x + m.y.x * temp.y + m.z.x * temp.z;
		r.y = m.x.y * temp.x + m.y.y * temp.y + m.z.y * temp.z;
		r.z = m.x.z * temp.x + m.y.z * temp.y + m.z.z * temp.z;
	}

	inline void Add(const Vector3& a, const Vector3& b, Vector3& r) {
		r.x = a.x + b.x;
		r.y = a.y + b.y;
		r.z = a.z + b.z;
	}

	inline void Sub(const Vector3& a, const Vector3& b, Vector3& r) {
		r.x = a.x - b.x;
		r.y = a.y - b.y;
		r.z = a.z - b.z;
	}

	//UMath::ScaleAdd((UMath::Vector3)state.matrix.y, counter_yaw - yaw, total_torque, total_torque);
	//total_torque.x = (counter_yaw - yaw) * state.matrix.y.x + total_torque.x;
	inline void ScaleAdd(const Vector3& a, const float s, const Vector3& b, Vector3& r) {
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
		}
		else {
			if (a > 0.f) {
				retval = Min(a, l);

			}
			else {
				retval = Max(a, l);
			}
		}
		return retval;
	}
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif


#endif