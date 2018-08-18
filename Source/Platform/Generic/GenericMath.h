#pragma once

#include "EngineCore/EngineUtils.h"
#include "cmwc4096.h"
#include <cmath>
#include <cstdlib>

/**
 * Various math functions. 
 *
 * This struct is here so we can optimize for other platforms.
 *
 */
struct BaseMath
{
	static CONSTEXPR FORCEINLINE int32 TruncToInt(float Val)
	{
		return (int32)Val;
	}

	static CONSTEXPR FORCEINLINE float TruncToFloat(float Val)
	{
		return (float)TruncToInt(Val);
	}

	static FORCEINLINE int32 Floor(float Val)
	{
		return TruncToInt(Floorf(Val));
	}

	static FORCEINLINE float Floorf(float Val)
	{
		return floorf(Val);
	}

	static FORCEINLINE double Floord(double Val)
	{
		return floor(Val);
	}

	static FORCEINLINE int32 RoundToInt(float Val)
	{
		return CeilToInt(Val - 0.5f);
	}

	static FORCEINLINE float RoundToFloat(float Val)
	{
		return CeilToFloat(Val - 0.5f);
	}

	static FORCEINLINE int32 CeilToInt(float Val)
	{
		return TruncToInt(ceilf(Val));
	}

	static FORCEINLINE float CeilToFloat(float Val)
	{
		return ceilf(Val);
	}

	static FORCEINLINE double CeilToDouble(double Val)
	{
		return ceil(Val);
	}

	static FORCEINLINE float Fractional(float Val)
	{
		return Val - TruncToFloat(Val);
	}

	static FORCEINLINE float Exp(float Val) { return expf(Val); }
	static FORCEINLINE float Ln(float Val) { return logf(Val); }
	static FORCEINLINE float Logx(float base, float Val) { return Ln(Val)/Ln(base); }
	static FORCEINLINE float Log2(float Val) { return Ln(Val) * MATH_R_LN_2; }

	static FORCEINLINE float Fmod(float num, float den) { return fmodf(num, den); }
	static FORCEINLINE float Sin(float Val) { return sinf(Val); }
	static FORCEINLINE float Arcsin(float Val) { return asinf(Val); }
	static FORCEINLINE float Cos(float Val) { return cosf(Val); }
	static FORCEINLINE float Arccos(float Val) { return acosf(Val); }
	static FORCEINLINE float Tan(float Val) { return tanf(Val); }
	static FORCEINLINE float Arctan(float Val) { return atanf(Val); }
	static FORCEINLINE float Arctan(float Val, float Val2) { return atan2f(Val, Val2); }
	static FORCEINLINE float Pow(float Val, float power) { return powf(Val, power); }
	static FORCEINLINE float Sqrt(float Val) { return sqrtf(Val); }

	template<typename T>
	static CONSTEXPR FORCEINLINE T Select(const T& InCmp, const T& ValIfGreaterOrEqualToZero, const T& ValIfLessZero)
	{
		return InCmp >= (T)(0) ? ValIfGreaterOrEqualToZero : ValIfLessZero;
	}
	
	static FORCEINLINE void Sincos(float* OutSin, float* OutCos, float Angle)
	{
		// If angle is outside range of [0, 2*pi], adjust it so it is.
		// Using Fmod gives worse precision than the following code
		if ((Angle < 0.f) || (Angle >= MATH_TWO_PI)) 
		{
			Angle -= Floorf(Angle * MATH_R_TWO_PI) * MATH_TWO_PI;
		}

		// This approximation is only accurate with angles in the range
		// [-pi/2, pi/2]. If angle is outside that range, convert it to that
		// range and find the appropriate sign to adjust the result.
		Angle = MATH_PI - Angle;
		float Sign = -1.f;
		if (fabsf(Angle) >= MATH_HALF_PI ) 
		{
			Angle = Select(Angle, MATH_PI, -MATH_PI) - Angle;
			Sign = 1.f;
		}

		// Sine and cosine are calculated with their respective taylor series
		// (technically Maclaurin series, since no offset is used). The "typical"
		// series is as follows:
		//
		// sin(a) = a/(1!) - a^3/(3!) + a^5/(5!) - a^7/(7!) + ...
		// cos(a) = 1 - a^2/(2!) + a^4/(4!) - a^6/(6!) + a^8/(8!) - ...
		//
		// However, this form is inefficient for computation. Some algebra has 
		// been performed to reduce computational cost.
		//
		// For sine:
		// - The a was factored out
		// sin(a) = a * (1/(1!) - a^2/(3!) + a^4/(5!) - a^6/(7!))
		// - a^2 was factored out
		// sin(a) = a * (1/(1!) + a^2*(-1/(3!) + a^2*(1/(5!) + a^2*(...))))
		// - The factorial constants were precalculated and hardcoded, leaving
		// the final equation used. The equation terminates at the a^9/(9!) term
		// of the original taylor series.
		//
		// For cosine:
		// - a^2 was factored out
		// cos(a) = 1 + a^2*(-1/(2!) + a^2*(1/(4!) + a^2*(-1/(6!) + a^2*(...))))
		// - The factorial constants were precalculated and hardcoded, leaving
		// the final equation used. The equation terminates at the a^10/(10!)
		// term of the original taylor series.
		//
		// Note that in both equations, some of the factorial constants have
		// been modified to generate empirically more accurate results.

		float a2 = Angle * Angle;
		*OutSin = Angle * (((((-2.39e-08f * a2 + 2.7526e-06f) * a2 - 1.98409e-04f) * a2 + 8.3333315e-03f) * a2 - 1.666666664e-01f) * a2 + 1.f);
		*OutCos = Sign * (((((-2.605e-07f * a2 + 2.47609e-05f) * a2 - 1.3888397e-03f) *	a2 + 4.16666418e-02f) * a2 - 4.999999963e-01f) * a2 + 1.f);
	}

	static FORCEINLINE float Reciprocal(float Val)
	{
		return 1.f/Val;
	}

	static FORCEINLINE float rSqrt(float Val)
	{
		return Reciprocal(sqrtf(Val));
	}

	static FORCEINLINE bool IsNaN(float Val)
	{
		union 
		{	
			float f;
			uint32 i;
		} f;
		f.f = Val;
		return (f.i & 0x7FFFFFFF) > 0x7F800000;
	}

	static FORCEINLINE bool IsFinite(float Val)
	{
		union 
		{
			float f;
			uint32 i;
		} f;
		f.f = Val;
		return (f.i & 0x7F800000) != 0x7F800000;
	}

	static FORCEINLINE int32 Rand() { return ::rand(); }
	static FORCEINLINE void RandFromSeed(int32 Seed) { srand((uint32)Seed); }
	static FORCEINLINE float Randf() { return ::rand()/(float)RAND_MAX; }
	static FORCEINLINE float Randf(float Min, float Max) { return Lerp(Min, Max, Randf()); }
	
	static FORCEINLINE uint32 FloorLog2(uint32 Val)
	{
		uint32 pos = 0;
		if (Val >= 1<<16) { Val >>= 16; pos += 16; }
		if (Val >= 1<< 8) { Val >>=  8; pos +=  8; }
		if (Val >= 1<< 4) { Val >>=  4; pos +=  4; }
		if (Val >= 1<< 2) { Val >>=  2; pos +=  2; }
		if (Val >= 1<< 1) {				pos +=  1; }
		return (Val == 0) ? 0 : pos;
	}

	static FORCEINLINE uint32 GetNumLeadingZeroes(uint32 Val)
	{
		if(Val == 0) {
			return 32;
		}
		return 31 - FloorLog2(Val);
	}

	static FORCEINLINE uint32 CeilLog2(uint32 Val)
	{
		if(Val <= 1) {
			return 0;
		}
		return FloorLog2(Val-1)+1;
	}

	static FORCEINLINE uint32 RoundUpToNextPowerOf2(uint32 Val)
	{
		return 1 << CeilLog2(Val);
	}

	template<typename T>
	static CONSTEXPR FORCEINLINE T Abs(const T& Val)
	{
		return Val >= (T)(0) ? Val : -Val;
	}

	template<typename T>
	static CONSTEXPR FORCEINLINE T Min(const T& Val1, const T& Val2)
	{
		return Val1 <= Val2 ? Val1 : Val2;
	}

	template<typename T>
	static CONSTEXPR FORCEINLINE T Min3(const T& Val1, const T& Val2, const T& Val3)
	{
		return Min(Min(Val1, Val2), Val3);
	}

	template<typename T>
	static CONSTEXPR FORCEINLINE T Max(const T& Val1, const T& Val2)
	{
		return Val1 >= Val2 ? Val1 : Val2;
	}

	template<typename T>
	static CONSTEXPR FORCEINLINE T Max3(const T& Val1, const T& Val2, const T& Val3)
	{
		return Max(Max(Val1, Val2), Val3);
	}

	/*
	 *	/r/me_irl
	 **/
	template<typename T>
	static FORCEINLINE T Mad(const T& Val1, const T& Val2, const T& Val3)
	{
		return Val1 * Val2 + Val3;
	}

	template<typename T>
	static FORCEINLINE T ToDeg(const T& Val)
	{
		return Val * MATH_RAD_TO_DEG_CONV;
	}

	template<typename T>
	static FORCEINLINE T ToRad(const T& Val)
	{
		return Val * MATH_DEG_TO_RAD_CONV;
	}

	template<typename T, typename U>
	static FORCEINLINE T Lerp(const T& Val1, const T& Val2, const U& Amount)
	{
		return (T)(Val1 * ((U)(1) - Amount) + Val2 * Amount);
	}

	template<typename T, typename U>
	static FORCEINLINE T CubeLerp(const T& Val1, const T& Val2, const U& Amount)
	{
		// Cubic interp, assuming tangents are 0 at all points.
		return Lerp(Val1, Val2, 3*Amount*Amount - 2*Amount*Amount*Amount);
	}

	template<typename T, typename U>
	static FORCEINLINE T CubeInterp(const T& val0, const T& Val1, const T& Val2, const T& Val3, const U& Amount)
	{
		// Using Catmull-Rom tangents
		U amt2 = Amount*Amount;
		return (
				 (Val3*(U(1)/U(2)) - Val2*(U(3)/U(2)) - val0*(U(1)/U(2)) + Val1*(U(3)/U(2)))*Amount*amt2
				+(val0 - Val1*(U(5)/U(2)) + Val2*U(2) - Val3*(U(1)/U(2)))*amt2
				+(Val2*(U(1)/U(2)) - val0*(U(1)/U(2)))*Amount
				+Val1);
	}

	/*
	 *	@TODO(Formating): I feel like this return statement should be reformated. 
	 **/
	template<typename T, typename U>
	static FORCEINLINE T CubInterpSpecifyTangents(const T& Val1, const T& Tan1, const T& Val2, const T& Tan2, const U& Amount)
	{
		// Using custom tangents
		U amt2 = Amount*Amount;
		return (
				 (Tan2 - Val2*U(2) + Tan1 + Val1*(U(2)))*Amount*amt2
				+(Tan1*U(2) - Val1*U(3) + Val2*U(3) - Tan2*U(2))*amt2
				+Tan1*Amount
				+Val1);
	}

	template<typename T, typename U>
	static FORCEINLINE T Bilerp(const T& Val, const T& Val1, const T& Val3, const T& Val4, const U& AmountX, const U& AmountY)
	{
		return Lerp( Lerp(Val, Val1, AmountX), Lerp(Val3, Val4, AmountX),	AmountY);
	}

	/*
	 *	Clamps X between Min and Max
	 **/
	template<typename T>
	static FORCEINLINE T Clamp(const T& X, const T& Min, const T& Max)
	{
		return X < Min ? Min : X < Max ? X : Max;
	}

	template<typename T>
	static FORCEINLINE T Saturate(const T& Val)
	{
		return Clamp(Val, (T)(0), (T)(1));
	}

	template<typename T>
	static FORCEINLINE T Square(const T& Val)
	{
		return Val * Val;
	}

	template<typename T>
	static FORCEINLINE T Cube(const T& Val)
	{
		return Val * Val * Val;
	}

	template<typename T>
	static FORCEINLINE bool Equals(const T& Val1, const T& Val2, const T& ErrorMargin)
	{
		return Abs(Val1 - Val2) < ErrorMargin;
	}

	// TODO(Devlinw): Add some functions to check for Min/Max of MArray's and MMaps
};

template<>
FORCEINLINE float BaseMath::Abs(const float& Val)
{
	return fabsf(Val);
}


