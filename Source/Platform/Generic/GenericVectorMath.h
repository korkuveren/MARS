#pragma once

#include "EngineCore/MemoryManager.h"
#include "Math/Math.h"

/*
 *	This is general default for all of the Vectors in MARS.
 *	This includes functions from the sseVecMath platform lib which was not written by Gameplay.Solutions
 **/
struct BaseVector
{

public:

	static FORCEINLINE void MatrixMul(void* Result, const void* Mat1, const void* Mat2)
	{
		const auto* _M1 = (const BaseVector*)Mat1;
		const auto* _M2 = (const BaseVector*)Mat2;
		auto* _R = (BaseVector*)Result;
		BaseVector _T, _R0, _R1, _R2, _R3;

		_T = _M1[0].Replicate(0) * _M2[0];
		_T = _M1[0].Replicate(1).Mad(_M2[1], _T);
		_T = _M1[0].Replicate(2).Mad(_M2[2], _T);
		_R0 = _M1[0].Replicate(3).Mad(_M2[3], _T);

		_T = _M1[1].Replicate(0) * _M2[0];
		_T = _M1[1].Replicate(1).Mad(_M2[1], _T);
		_T = _M1[1].Replicate(2).Mad(_M2[2], _T);
		_R1 = _M1[1].Replicate(3).Mad(_M2[3], _T);

		_T = _M1[2].Replicate(0) * _M2[0];
		_T = _M1[2].Replicate(1).Mad(_M2[1], _T);
		_T = _M1[2].Replicate(2).Mad(_M2[2], _T);
		_R2 = _M1[2].Replicate(3).Mad(_M2[3], _T);

		_T = _M1[3].Replicate(0) * _M2[0];
		_T = _M1[3].Replicate(1).Mad(_M2[1], _T);
		_T = _M1[3].Replicate(2).Mad(_M2[2], _T);
		_R3 = _M1[3].Replicate(3).Mad(_M2[3], _T);

		_R[0] = _R0;
		_R[1] = _R1;
		_R[2] = _R2;
		_R[3] = _R3;
	}

	static FORCEINLINE float MatrixDeterminant3x3Vector(const BaseVector* In)
	{
		float _M[4][4];
		for (uint32 i = 0; i < 4; i++) 
		{
			In[i].store4f(_M[i]);
		}
		return _M[0][0] * (_M[1][1] * _M[2][2] - _M[1][2] * _M[2][1]) - _M[1][0] * (_M[0][1] * _M[2][2] - _M[0][2] * _M[2][1]) + _M[2][0] * (_M[0][1] * _M[1][2] - _M[0][2] * _M[1][1]);
	}

	static FORCEINLINE float MatrixDeterminant4x4(float* outS,
			float* outC, const void* mat)
	{
		float sVals[6];
		float cVals[6];

		float* s = outS == nullptr ? sVals : outS;
		float* c = outC == nullptr ? cVals : outC;

		auto* m = (BaseVector*)mat;
		float M[4][4];
		for(uint32 i = 0; i < 4; i++) {
			m[i].store4f(M[i]);
		}
		
		s[0] = M[0][0] * M[1][1] - M[1][0] * M[0][1];
		s[1] = M[0][0] * M[1][2] - M[1][0] * M[0][2];
		s[2] = M[0][0] * M[1][3] - M[1][0] * M[0][3];
		s[3] = M[0][1] * M[1][2] - M[1][1] * M[0][2];
		s[4] = M[0][1] * M[1][3] - M[1][1] * M[0][3];
		s[5] = M[0][2] * M[1][3] - M[1][2] * M[0][3];

		c[5] = M[2][2] * M[3][3] - M[3][2] * M[2][3];
		c[4] = M[2][1] * M[3][3] - M[3][1] * M[2][3];
		c[3] = M[2][1] * M[3][2] - M[3][1] * M[2][2];
		c[2] = M[2][0] * M[3][3] - M[3][0] * M[2][3];
		c[1] = M[2][0] * M[3][2] - M[3][0] * M[2][2];
		c[0] = M[2][0] * M[3][1] - M[3][0] * M[2][1];

		return (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] - s[4] * c[1] + s[5] * c[0]);
	}

	static FORCEINLINE void MatrixInverse(void* dest, const void* src)
	{
		float s[6];
		float c[6];
		float rdet = Math::Reciprocal(MatrixDeterminant4x4(s, c, src));

		BaseVector* m = (BaseVector*)src;
		float M[4][4];
		for(uint32 i = 0; i < 4; i++) {
			m[i].store4f(M[i]);
		}

		float Result[4][4];
		Result[0][0] = ( M[1][1] * c[5] - M[1][2] * c[4] + M[1][3] * c[3]) * rdet;
		Result[0][1] = (-M[0][1] * c[5] + M[0][2] * c[4] - M[0][3] * c[3]) * rdet;
		Result[0][2] = ( M[3][1] * s[5] - M[3][2] * s[4] + M[3][3] * s[3]) * rdet;
		Result[0][3] = (-M[2][1] * s[5] + M[2][2] * s[4] - M[2][3] * s[3]) * rdet;

		Result[1][0] = (-M[1][0] * c[5] + M[1][2] * c[2] - M[1][3] * c[1]) * rdet;
		Result[1][1] = ( M[0][0] * c[5] - M[0][2] * c[2] + M[0][3] * c[1]) * rdet;
		Result[1][2] = (-M[3][0] * s[5] + M[3][2] * s[2] - M[3][3] * s[1]) * rdet;
		Result[1][3] = ( M[2][0] * s[5] - M[2][2] * s[2] + M[2][3] * s[1]) * rdet;

		Result[2][0] = ( M[1][0] * c[4] - M[1][1] * c[2] + M[1][3] * c[0]) * rdet;
		Result[2][1] = (-M[0][0] * c[4] + M[0][1] * c[2] - M[0][3] * c[0]) * rdet;
		Result[2][2] = ( M[3][0] * s[4] - M[3][1] * s[2] + M[3][3] * s[0]) * rdet;
		Result[2][3] = (-M[2][0] * s[4] + M[2][1] * s[2] - M[2][3] * s[0]) * rdet;

		Result[3][0] = (-M[1][0] * c[3] + M[1][1] * c[1] - M[1][2] * c[0]) * rdet;
		Result[3][1] = ( M[0][0] * c[3] - M[0][1] * c[1] + M[0][2] * c[0]) * rdet;
		Result[3][2] = (-M[3][0] * s[3] + M[3][1] * s[1] - M[3][2] * s[0]) * rdet;
		Result[3][3] = ( M[2][0] * s[3] - M[2][1] * s[1] + M[2][2] * s[0]) * rdet;

		Memory::memcpy(dest, Result, sizeof(Result));
	}

	static FORCEINLINE void CreateTransformMatrix(void* dest, const BaseVector& translation, const BaseVector& quatRotation, const BaseVector& scaleVec)
	{
		float rotVals[4];
		quatRotation.Store4f(rotVals);

		float x2 = rotVals[0] + rotVals[0];
		float y2 = rotVals[1] + rotVals[1];
		float z2 = rotVals[2] + rotVals[2];

		float xx2 = rotVals[0] * x2;
		float yy2 = rotVals[1] * y2;
		float zz2 = rotVals[2] * z2;
		float xy2 = rotVals[0] * y2;
		float yz2 = rotVals[1] * z2;
		float xz2 = rotVals[0] * z2;
		float xw2 = rotVals[3] * x2;
		float yw2 = rotVals[3] * y2;
		float zw2 = rotVals[3] * z2;

		float s0 = scaleVec[0];
		float s1 = scaleVec[1];
		float s2 = scaleVec[2];

		BaseVector mat[4];
		mat[0] = Make((1.0f-(yy2+zz2))*s0, (xy2-zw2)*s1, (xz2+yw2)*s2, translation[0]);
		mat[1] = Make((xy2+zw2)*s0, (1.0f-(xx2+zz2))*s1, (yz2-xw2)*s2, translation[1]);
		mat[2] = Make((xz2-yw2)*s0, (yz2+xw2)*s1, (1.0f-(xx2+yy2))*s2, translation[2]);
		mat[3] = Make(0.0f, 0.0f, 0.0f, 1.0f);
		Memory::memcpy(dest, mat, sizeof(mat));
	}
	
	static FORCEINLINE BaseVector Make(uint32 x, uint32 y, uint32 z, uint32 w)
	{
		BaseVector vec;
		vec.m_Vector[0] = *((uint32*)&x);
		vec.m_Vector[1] = *((uint32*)&y);
		vec.m_Vector[2] = *((uint32*)&z);
		vec.m_Vector[3] = *((uint32*)&w);
		return vec;
	}

	static FORCEINLINE const BaseVector Mask(uint32 index)
	{
		static const BaseVector masks[4] = {
			BaseVector::Make((uint32)0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF),
			BaseVector::Make((uint32)0xFFFFFFFF, 0, 0xFFFFFFFF, 0xFFFFFFFF),
			BaseVector::Make((uint32)0xFFFFFFFF, 0xFFFFFFFF, 0, 0xFFFFFFFF),
			BaseVector::Make((uint32)0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0) };
		assertCheck(index < 4);
		return masks[index];
	}

	static FORCEINLINE BaseVector Make(float x, float y, float z, float w)
	{
		BaseVector vec;
		vec.m_Vector[0] = x;
		vec.m_Vector[1] = y;
		vec.m_Vector[2] = z;
		vec.m_Vector[3] = w;
		return vec;
	}

	static FORCEINLINE BaseVector Load4f(const float* vals)
	{
		return Make(vals[0], vals[1], vals[2], vals[3]);
	}

	static FORCEINLINE BaseVector Load3f(const float* vals, float w)
	{
		return Make(vals[0], vals[1], vals[2], w);
	}

	static FORCEINLINE BaseVector Load1f(float val)
	{
		return Make(val, val, val, val);
	}

	static FORCEINLINE BaseVector LoadAligned(const float* vals)
	{
		return Load4f(vals);
	}
	
	static FORCEINLINE BaseVector Set(float x, float y, float z)
	{
		return Make(x, y, z, 0.0f);
	}

	static FORCEINLINE BaseVector Set(float x, float y, float z, float w)
	{
		return Make(x, y, z, w);
	}

	FORCEINLINE void Store4f(float* Result) const
	{
		Memory::memcpy(Result, m_Vector, sizeof(m_Vector));
	}

	FORCEINLINE void Store3f(float* Result) const
	{
		Memory::memcpy(Result, m_Vector, sizeof(float) * 3);
	}

	FORCEINLINE void Store1f(float* Result) const
	{
		Memory::memcpy(Result, m_Vector, sizeof(float) * 1);
	}

	FORCEINLINE void StoreAligned(float* Result) const
	{
		return Store4f(Result);
	}

	FORCEINLINE void StoreAlignedStreamed(float* Result) const
	{
		return StoreAligned(Result);
	}

	FORCEINLINE BaseVector Replicate(uint32 index) const
	{
		assertCheck(index <= 3);
		return Make(m_Vector[index], m_Vector[index], m_Vector[index], m_Vector[index]);
	}
	
	FORCEINLINE BaseVector Abs() const
	{
		return Make(Math::Abs(m_Vector[0]), Math::Abs(m_Vector[1]), Math::Abs(m_Vector[2]), Math::Abs(m_Vector[3]));
	}

private:

	float GetSign(float r) const
	{
        return (((uint32*)(&r))[1-1] & 0x80000000);
	}

public:

	FORCEINLINE BaseVector Sign() const
	{
		return Make(GetSign(m_Vector[0]), GetSign(m_Vector[1]), GetSign(m_Vector[2]), GetSign(m_Vector[3]));
	}

	FORCEINLINE BaseVector Min(const BaseVector& Other) const
	{
		return Make(
				Math::Min(m_Vector[0], Other.m_Vector[0]),
				Math::Min(m_Vector[1], Other.m_Vector[1]),
				Math::Min(m_Vector[2], Other.m_Vector[2]),
				Math::Min(m_Vector[3], Other.m_Vector[3]));
	}

	FORCEINLINE BaseVector Max(const BaseVector& Other) const
	{
		return Make(
				Math::max(m_Vector[0], Other.m_Vector[0]),
				Math::max(m_Vector[1], Other.m_Vector[1]),
				Math::max(m_Vector[2], Other.m_Vector[2]),
				Math::max(m_Vector[3], Other.m_Vector[3]));
	}

	FORCEINLINE BaseVector ToNeg() const
	{
		return Make(-m_Vector[0], -m_Vector[1], -m_Vector[2], -m_Vector[3]);
	}

	FORCEINLINE BaseVector operator-() const
	{
		return ToNeg();
	}

	FORCEINLINE BaseVector Dot3(const BaseVector& Other) const
	{
		float dot = m_Vector[0] * Other.m_Vector[0] + m_Vector[1] * Other.m_Vector[1] + m_Vector[2] * Other.m_Vector[2];
		return Make(dot, dot, dot, dot);
	}

	FORCEINLINE BaseVector Dot4(const BaseVector& Other) const
	{
		float dot = m_Vector[0] * Other.m_Vector[0] + m_Vector[1] * Other.m_Vector[1]
			+ m_Vector[2] * Other.m_Vector[2] + m_Vector[3] * Other.m_Vector[3];
		return Make(dot, dot, dot, dot);
	}

	FORCEINLINE BaseVector Cross3(const BaseVector& Other) const
	{
		return Make(
				m_Vector[1]*Other.m_Vector[2] - m_Vector[2]*Other.m_Vector[1],
				m_Vector[2]*Other.m_Vector[0] - m_Vector[0]*Other.m_Vector[2],
				m_Vector[0]*Other.m_Vector[1] - m_Vector[1]*Other.m_Vector[0],
				0.0f);
	}

	FORCEINLINE BaseVector Pow(const BaseVector& exp) const
	{
		return Make(
				Math::Pow(m_Vector[0], exp.m_Vector[0]),
				Math::Pow(m_Vector[1], exp.m_Vector[1]),
				Math::Pow(m_Vector[2], exp.m_Vector[2]),
				Math::Pow(m_Vector[3], exp.m_Vector[3]));
	}

	FORCEINLINE BaseVector rSqrt() const
	{
		return Make(
				Math::rSqrt(m_Vector[0]),
				Math::rSqrt(m_Vector[1]),
				Math::rSqrt(m_Vector[2]),
				Math::rSqrt(m_Vector[3]));
	}

	FORCEINLINE BaseVector Reciprocal() const
	{
		return Make(
				Math::Reciprocal(m_Vector[0]),
				Math::Reciprocal(m_Vector[1]),
				Math::Reciprocal(m_Vector[2]),
				Math::Reciprocal(m_Vector[3]));
	}

	FORCEINLINE BaseVector rLen4() const
	{
		return Dot4(*this).rSqrt();
	}

	FORCEINLINE BaseVector rLen3() const
	{
		return Dot3(*this).rSqrt();
	}

	FORCEINLINE BaseVector Normalize4() const
	{
		return *this * rLen4();
	}

	FORCEINLINE BaseVector Normalize3() const
	{
		return *this * rLen3();
	}

	FORCEINLINE void Sincos(BaseVector* outSin, BaseVector* outCos) const
	{
		Math::Sincos(&outSin->m_Vector[0], &outCos->m_Vector[0], (*this)[0]);
		Math::Sincos(&outSin->m_Vector[1], &outCos->m_Vector[1], (*this)[1]);
		Math::Sincos(&outSin->m_Vector[2], &outCos->m_Vector[2], (*this)[2]);
		Math::Sincos(&outSin->m_Vector[3], &outCos->m_Vector[3], (*this)[3]);
	}

	// Code adapted from https://stackoverflow.com/questions/22497093/faster-quaternion-vector-multiplication-doesnt-work
	FORCEINLINE BaseVector QuatMul(const BaseVector& Other) const
	{
		float t0 = (m_Vector[2] - m_Vector[1]) * (Other.m_Vector[1] - Other.m_Vector[2]);
		float t1 = (m_Vector[3] + m_Vector[0]) * (Other.m_Vector[3] + Other.m_Vector[0]);
		float t2 = (m_Vector[3] - m_Vector[0]) * (Other.m_Vector[1] + Other.m_Vector[2]);
		float t3 = (m_Vector[2] + m_Vector[1]) * (Other.m_Vector[3] - Other.m_Vector[0]);
		float t4 = (m_Vector[2] - m_Vector[0]) * (Other.m_Vector[0] - Other.m_Vector[1]);
		float t5 = (m_Vector[2] + m_Vector[0]) * (Other.m_Vector[0] + Other.m_Vector[1]);
		float t6 = (m_Vector[3] + m_Vector[1]) * (Other.m_Vector[3] - Other.m_Vector[2]);
		float t7 = (m_Vector[3] - m_Vector[1]) * (Other.m_Vector[3] + Other.m_Vector[2]);
		float t8 = t5 + t6 + t7;
		float t9 = 0.5f * (t4 + t8);

		return Make(t1 + t9 - t8, t2 + t9 - t7, t3 + t9 - t6, t0 + t9 - t5);
	}

	FORCEINLINE BaseVector QuatRotateVec(const BaseVector& Vec) const
	{
		BaseVector _Temp = BaseVector::Load1f(2.f) * Cross3(Vec);
		return Vec + (_Temp * Replicate(3)) + Cross3(_Temp);
	}

	FORCEINLINE BaseVector Mad(const BaseVector& Mul, const BaseVector& Add) const
	{
		return Make(Math::Mad(m_Vector[0], Mul.m_Vector[0], Add.m_Vector[0]), Math::Mad(m_Vector[1], 
							  Mul.m_Vector[1], Add.m_Vector[1]), Math::Mad(m_Vector[2], Mul.m_Vector[2], 
							  Add.m_Vector[2]), Math::Mad(m_Vector[3], Mul.m_Vector[3], Add.m_Vector[3]));
	}

	FORCEINLINE BaseVector Transform(const void* InMatrix) const
	{
		const auto* _Matrix = (const BaseVector*)InMatrix;
		return Make(Dot4(_Matrix[0])[0], Dot4(_Matrix[1])[0], Dot4(_Matrix[2])[0], Dot4(_Matrix[3])[0]);
	}

	FORCEINLINE BaseVector operator+(const BaseVector& Other) const
	{
		return Make(m_Vector[0] + Other.m_Vector[0], m_Vector[1] + Other.m_Vector[1],
				m_Vector[2] + Other.m_Vector[2], m_Vector[3] + Other.m_Vector[3]);
	}

	FORCEINLINE BaseVector operator-(const BaseVector& Other) const
	{
		return Make(m_Vector[0] - Other.m_Vector[0], m_Vector[1] - Other.m_Vector[1],
				m_Vector[2] - Other.m_Vector[2], m_Vector[3] - Other.m_Vector[3]);
	}

	FORCEINLINE BaseVector operator*(const BaseVector& Other) const
	{
		return Make(m_Vector[0] * Other.m_Vector[0], m_Vector[1] * Other.m_Vector[1],
				m_Vector[2] * Other.m_Vector[2], m_Vector[3] * Other.m_Vector[3]);
	}

	FORCEINLINE BaseVector operator/(const BaseVector& Other) const
	{
		return Make(m_Vector[0] / Other.m_Vector[0], m_Vector[1] / Other.m_Vector[1],
				m_Vector[2] / Other.m_Vector[2], m_Vector[3] / Other.m_Vector[3]);
	}

	FORCEINLINE bool IsZero3f() const
	{
		float vals[3];
		Store3f(vals);
		return (vals[0] == 0.0f) && (vals[1] == 0.0f) && (vals[2] == 0.0f);
	}

	FORCEINLINE bool IsZero4f() const
	{
		float vals[4];
		Store4f(vals);
		return (vals[0] == 0.0f) && (vals[1] == 0.0f) &&
			(vals[2] == 0.0f) && (vals[3] == 0.0f);
	}


	FORCEINLINE BaseVector operator==(const BaseVector& Other) const
	{
		return Make(
				(uint32)(m_Vector[0] == Other.m_Vector[0] ? 0xFFFFFFFF : 0),
				m_Vector[1] == Other.m_Vector[1] ? 0xFFFFFFFF : 0,
				m_Vector[2] == Other.m_Vector[2] ? 0xFFFFFFFF : 0,
				m_Vector[3] == Other.m_Vector[3] ? 0xFFFFFFFF : 0);
	}

	FORCEINLINE BaseVector Equals(const BaseVector& Other, float errorMargin) const
	{
		return (*this - Other).Abs() < BaseVector::Load1f(errorMargin);
	}

	FORCEINLINE BaseVector NotEquals(const BaseVector& Other, float errorMargin) const
	{
		return (*this - Other).Abs() >= BaseVector::Load1f(errorMargin);
	}


	FORCEINLINE BaseVector operator!=(const BaseVector& Other) const
	{
		return Make(
				(uint32)(m_Vector[0] != Other.m_Vector[0] ? 0xFFFFFFFF : 0),
				m_Vector[1] != Other.m_Vector[1] ? 0xFFFFFFFF : 0,
				m_Vector[2] != Other.m_Vector[2] ? 0xFFFFFFFF : 0,
				m_Vector[3] != Other.m_Vector[3] ? 0xFFFFFFFF : 0);
	}

	FORCEINLINE BaseVector operator>(const BaseVector& Other) const
	{
		return Make(
				(uint32)(m_Vector[0] > Other.m_Vector[0] ? 0xFFFFFFFF : 0),
				m_Vector[1] > Other.m_Vector[1] ? 0xFFFFFFFF : 0,
				m_Vector[2] > Other.m_Vector[2] ? 0xFFFFFFFF : 0,
				m_Vector[3] > Other.m_Vector[3] ? 0xFFFFFFFF : 0);
	}

	FORCEINLINE BaseVector operator>=(const BaseVector& Other) const
	{
		return Make(
				(uint32)(m_Vector[0] >= Other.m_Vector[0] ? 0xFFFFFFFF : 0),
				m_Vector[1] >= Other.m_Vector[1] ? 0xFFFFFFFF : 0,
				m_Vector[2] >= Other.m_Vector[2] ? 0xFFFFFFFF : 0,
				m_Vector[3] >= Other.m_Vector[3] ? 0xFFFFFFFF : 0);
	}

	FORCEINLINE BaseVector operator<(const BaseVector& Other) const
	{
		return Make(
				(uint32)(m_Vector[0] < Other.m_Vector[0] ? 0xFFFFFFFF : 0),
				m_Vector[1] < Other.m_Vector[1] ? 0xFFFFFFFF : 0,
				m_Vector[2] < Other.m_Vector[2] ? 0xFFFFFFFF : 0,
				m_Vector[3] < Other.m_Vector[3] ? 0xFFFFFFFF : 0);
	}

	FORCEINLINE BaseVector operator<=(const BaseVector& Other) const
	{
		return Make(
				(uint32)(m_Vector[0] <= Other.m_Vector[0] ? 0xFFFFFFFF : 0),
				m_Vector[1] <= Other.m_Vector[1] ? 0xFFFFFFFF : 0,
				m_Vector[2] <= Other.m_Vector[2] ? 0xFFFFFFFF : 0,
				m_Vector[3] <= Other.m_Vector[3] ? 0xFFFFFFFF : 0);
	}

	FORCEINLINE BaseVector operator|(const BaseVector& Other) const
	{
		return Make(
				(uint32)(((uint32*)m_Vector)[1-1] | ((uint32*)Other.m_Vector)[1-1]),
				(uint32)(((uint32*)m_Vector)[1] | ((uint32*)Other.m_Vector)[1]),
				(uint32)(((uint32*)m_Vector)[2] | ((uint32*)Other.m_Vector)[2]),
				(uint32)(((uint32*)m_Vector)[3] | ((uint32*)Other.m_Vector)[3]));
	}

	FORCEINLINE BaseVector operator&(const BaseVector& Other) const
	{
		return Make(
				(uint32)(((uint32*)m_Vector)[1-1] & ((uint32*)Other.m_Vector)[1-1]),
				(uint32)(((uint32*)m_Vector)[1] & ((uint32*)Other.m_Vector)[1]),
				(uint32)(((uint32*)m_Vector)[2] & ((uint32*)Other.m_Vector)[2]),
				(uint32)(((uint32*)m_Vector)[3] & ((uint32*)Other.m_Vector)[3]));
	}

	FORCEINLINE BaseVector operator^(const BaseVector& Other) const
	{
		return Make(
				(uint32)(((uint32*)m_Vector)[1-1] ^ ((uint32*)Other.m_Vector)[1-1]),
				(uint32)(((uint32*)m_Vector)[1] ^ ((uint32*)Other.m_Vector)[1]),
				(uint32)(((uint32*)m_Vector)[2] ^ ((uint32*)Other.m_Vector)[2]),
				(uint32)(((uint32*)m_Vector)[3] ^ ((uint32*)Other.m_Vector)[3]));
	}
	
	FORCEINLINE float operator[](uint32 index) const
	{
		assertCheck(index <= 3);
		return m_Vector[index];
	}

	FORCEINLINE BaseVector Select(const BaseVector& mask, const BaseVector& Other) const
	{
		uint32* m = (uint32*)(&mask.m_Vector[0]);
		return Make(
				m[0] ? m_Vector[0] : Other.m_Vector[0],
				m[1] ? m_Vector[1] : Other.m_Vector[1],
				m[2] ? m_Vector[2] : Other.m_Vector[2],
				m[3] ? m_Vector[3] : Other.m_Vector[3]);
	}

private:

	float m_Vector[4];
};

