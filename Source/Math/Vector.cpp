#include "Vector.h"

bool Vector3D::operator==(const Vector3D& Other) const
{
	return (m_Vector != Other.m_Vector).IsZero3f();
}

bool Vector3D::operator!=(const Vector3D& Other) const
{
	return !(*this == Other);
}

bool Vector3D::Equals(const Vector3D& Other, float ErrorMargin) const
{
	return m_Vector.NotEquals(Other.m_Vector, ErrorMargin).IsZero3f();
}

bool Vector3D::Equals(float Val, float ErrorMargin) const
{
	return Equals(Vector3D(Val), ErrorMargin);
}

float Vector3D::operator[](uint32 Index) const
{
	assertCheck(Index < 3);
	return m_Vector[Index];
}

void Vector3D::Set(float X, float Y, float Z)
{
	m_Vector = Vector::Make(X, Y, Z, 0.f);
}

void Vector3D::Set(uint32 Index, float Val)
{
	m_Vector = m_Vector.Select(Vector::Mask(Index), Vector::Load1f(Val));
}

float Vector3D::Max() const
{
	float _Vals[3];
	m_Vector.Store3f(_Vals);
	return Math::Max3(_Vals[0], _Vals[1], _Vals[2]);
}

float Vector3D::Min() const
{
	float _Vals[3];
	m_Vector.Store3f(_Vals);
	return Math::Min3(_Vals[0], _Vals[1], _Vals[2]);
}

float Vector3D::AbsMax() const
{
	float _Vals[3];
	m_Vector.Abs().Store3f(_Vals);
	return Math::Max3(_Vals[0], _Vals[1], _Vals[2]);
}

float Vector3D::AbsMin() const
{
	float _Vals[3];
	m_Vector.Abs().Store3f(_Vals);
	return Math::Min3(_Vals[0], _Vals[1], _Vals[2]);
}

Vector3D Vector3D::Abs() const
{
	return Vector3D(m_Vector.Abs());
}

Vector3D Vector3D::Min(const Vector3D& Other) const
{
	return Vector3D(m_Vector.Min(Other.m_Vector));
}

Vector3D Vector3D::Max(const Vector3D& Other) const
{
	return Vector3D(m_Vector.Max(Other.m_Vector));
}

Vector3D Vector3D::Normalized() const
{
	return Vector3D(m_Vector.Normalize3());
}

bool Vector3D::GetIsNormalized(float ErrorMargin) const
{
	return Math::Abs(1.f - LengthSquared()) < ErrorMargin;
}

void Vector3D::DirAndLength(Vector3D& Direction, float& Length) const
{
	Vector _Len = m_Vector.Dot3(m_Vector).rSqrt();
	Direction = Vector3D(m_Vector * _Len);
	Length = Math::Reciprocal(_Len[0]);
}

Vector3D Vector3D::Project() const
{
	Vector _ProjectVal = Vector::Load1f(Math::Reciprocal(m_Vector[2]));
	return Vector3D(m_Vector * _ProjectVal);
}

Vector3D Vector3D::Reciprocal() const
{
	return Vector3D(m_Vector.Reciprocal());
}


Vector3D Vector3D::Rotate(const Vector3D& Axis, float Angle) const
{
	float SinAngle;
	float CosAngle;
	
	Math::Sincos(&SinAngle, &CosAngle, -Angle);
	Vector _SinVec = Vector::Load1f(SinAngle);
	Vector _CosVec = Vector::Load1f(CosAngle);
	Vector _OneMinusCosVec = Vector::Load1f(1.f - CosAngle);
	
	Vector _RotatedX = m_Vector.Cross3(Axis.m_Vector * _SinVec);
	Vector _RotatedY = Axis.m_Vector * m_Vector.Dot3(Axis.m_Vector * _OneMinusCosVec);
	Vector _RotatedZ = m_Vector * _CosVec;

	return Vector3D(_RotatedX + _RotatedY + _RotatedZ);
}


Vector3D Vector3D::Reflect(const Vector3D& Normal) const
{
	Vector _DotAmount = VectorConstants::TWO * m_Vector.Dot3(Normal.m_Vector);
	return Vector3D(m_Vector - (Normal.m_Vector * _DotAmount));
}

Vector3D Vector3D::Refract(const Vector3D& Normal, float IndexOfRefraction) const
{
	float _CosNormalAngle = m_Vector.Dot3(Normal.m_Vector)[0];
	float _RefractanceSquared =  1.f - IndexOfRefraction * IndexOfRefraction * (1.f - _CosNormalAngle * _CosNormalAngle);
	
	if (_RefractanceSquared < 0.f) 
	{
		return Vector3D(VectorConstants::ZERO);
	}

	float _NormalScale = IndexOfRefraction * _CosNormalAngle + Math::Sqrt(_RefractanceSquared);
	Vector _NormalScaleVec(Vector::Load1f(_NormalScale));
	Vector _IindexOfRefractionVec(Vector::Load1f(IndexOfRefraction));
	
	return Vector3D(m_Vector * _IindexOfRefractionVec - _NormalScaleVec * Normal.m_Vector);
}

Vector3D Vector3D::ToDeg() const
{
	return Vector3D(m_Vector * Vector::Load1f(MATH_RAD_TO_DEG_CONV));
}

Vector3D Vector3D::ToRad() const
{
	return Vector3D(m_Vector * Vector::Load1f(MATH_DEG_TO_RAD_CONV));
}

Vector Vector3D::ToVector(float In) const
{
	return m_Vector.Select(VectorConstants::MASK_W, Vector::Load1f(In));
}

bool Vector2D::operator==(const Vector2D& Other) const
{
	return m_Vector[0] == Other.m_Vector[0] && m_Vector[1] == Other.m_Vector[1];
}

bool Vector2D::operator!=(const Vector2D& Other) const
{
	return m_Vector[0] != Other.m_Vector[0] || m_Vector[1] != Other.m_Vector[1];
}

bool Vector2D::Equals(const Vector2D& Other, float ErrorMargin) const
{
	return (m_Vector[0] - Other.m_Vector[0]) < ErrorMargin && (m_Vector[1] - Other.m_Vector[1]) < ErrorMargin;
}

bool Vector2D::Equals(float Val, float ErrorMargin) const
{
	return (m_Vector[0] - Val) < ErrorMargin && (m_Vector[1] - Val) < ErrorMargin;
}

float Vector2D::operator[](uint32 Index) const
{
	assertCheck(Index < 2);
	return m_Vector[Index];
}

void Vector2D::Set(float X, float Y)
{
	m_Vector[0] = X;
	m_Vector[1] = Y;
}

void Vector2D::Set(uint32 Index, float Val)
{
	assertCheck(Index < 2);
	m_Vector[Index] = Val;
}

float Vector2D::Max() const
{
	return Math::Max(m_Vector[0], m_Vector[1]);
}

float Vector2D::Min() const
{
	return Math::Min(m_Vector[0], m_Vector[1]);
}

float Vector2D::AbsMax() const
{
	return Math::Max(Math::Abs(m_Vector[0]), Math::Abs(m_Vector[1]));
}

float Vector2D::AbsMin() const
{
	return Math::Min(Math::Abs(m_Vector[0]), Math::Abs(m_Vector[1]));
}

Vector2D Vector2D::Abs() const
{
	return Vector2D(Math::Abs(m_Vector[0]), Math::Abs(m_Vector[1]));
}

Vector2D Vector2D::Min(const Vector2D& Other) const
{
	return Vector2D(Math::Min(m_Vector[0], Other.m_Vector[0]), Math::Min(m_Vector[1], Other.m_Vector[1]));
}

Vector2D Vector2D::Max(const Vector2D& Other) const
{
	return Vector2D(Math::Max(m_Vector[0], Other.m_Vector[0]), Math::Max(m_Vector[1], Other.m_Vector[1]));
}

Vector2D Vector2D::Normalized(float ErrorMargin) const
{
	float _LengthSq = LengthSquared();
	if (_LengthSq < ErrorMargin) 
	{
		return Vector2D(0.f, 0.f);
	}

	return (*this) * Math::rSqrt(_LengthSq);
}

bool Vector2D::GetIsNormalized(float ErrorMargin) const
{
	return Math::Abs(1.f - LengthSquared()) < ErrorMargin;
}

void Vector2D::DirectionLength(Vector2D& Direction, float& Length, float ErrorMargin) const
{
	float _LengthSq = LengthSquared();
	if (_LengthSq < ErrorMargin) 
	{
		Direction = Vector2D(0.f, 0.f);
		Length = 0;
		return;
	}

	float _rLen = Math::rSqrt(_LengthSq);
	Direction = (*this) * _rLen;
	Length = Math::Reciprocal(_rLen);
}

Vector2D Vector2D::Reciprocal() const
{
	return Vector2D(Math::Reciprocal(m_Vector[0]), Math::Reciprocal(m_Vector[1]));
}

Vector2D Vector2D::Rotate(float Angle) const
{
	float _Sin, _Cos;
	Math::Sincos(&_Sin, &_Cos, Angle);
	return Vector2D(_Cos * m_Vector[0] - _Sin * m_Vector[1], _Sin * m_Vector[0] + _Cos * m_Vector[1]);
}

Vector2D Vector2D::Reflect(const Vector2D& Normal) const
{
	Vector2D _DotAmount = Vector2D(2.f * Dot(Normal));
	return (*this) - (Normal * _DotAmount);
}

Vector2D Vector2D::Refract(const Vector2D& Normal, float IndexOfRefraction) const
{
	float _CosNormalAngle = Dot(Normal);
	float _RefractanceSquared = 1.f - IndexOfRefraction * IndexOfRefraction * (1.f - _CosNormalAngle * _CosNormalAngle);
	
	if (_RefractanceSquared < 0.f) 
	{
		return Vector2D(0.f);
	}

	float _NormalScale = IndexOfRefraction * _CosNormalAngle + Math::Sqrt(_RefractanceSquared);
	Vector2D _NormalScaleVec(_NormalScale);
	Vector2D _IndexOfRefractionVec(IndexOfRefraction);
	
	return (*this) * _IndexOfRefractionVec - _NormalScaleVec * Normal;
}

Vector2D Vector2D::ToDeg() const
{
	return Vector2D(MATH_RAD_TO_DEG_CONV * m_Vector[0], MATH_RAD_TO_DEG_CONV * m_Vector[1]);
}

Vector2D Vector2D::ToRad() const
{
	return Vector2D(MATH_DEG_TO_RAD_CONV * m_Vector[0], MATH_DEG_TO_RAD_CONV * m_Vector[1]);
}

Vector Vector2D::ToVector2D() const
{
	return ToVector2D(0.f, 0.f);
}

Vector Vector2D::ToVector2D(float Z, float In) const
{
	return ToVector2D(Vector2D(Z, In));
}

Vector Vector2D::ToVector2D(Vector2D Other) const
{
	return Vector::Make(m_Vector[0], m_Vector[1], Other.m_Vector[0], Other.m_Vector[1]);
}
