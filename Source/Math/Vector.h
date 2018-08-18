#pragma once

#include "EngineCore/EngineUtils.h"
#include "VecMath.h"

/*
 *
 *	Contains everything for a 3D Vector
 *	Default is X, Y, Z
 **/
class Vector3D
{

public:

	FORCEINLINE Vector3D();
	FORCEINLINE Vector3D(float Val);
	FORCEINLINE Vector3D(float X, float Y, float Z);
	FORCEINLINE Vector3D(const Vector& In);

	FORCEINLINE float Dot(const Vector3D& Other) const;
	FORCEINLINE Vector3D DotToVector(const Vector3D& Other) const;
	FORCEINLINE Vector3D Cross(const Vector3D& Other) const;
	FORCEINLINE float Dist(const Vector3D& Other) const;
	FORCEINLINE float DistSquared(const Vector3D& Other) const;

	FORCEINLINE Vector3D operator+(const Vector3D& Other) const;
	FORCEINLINE Vector3D operator-(const Vector3D& Other) const;
	FORCEINLINE Vector3D operator*(const Vector3D& Other) const;
	FORCEINLINE Vector3D operator/(const Vector3D& Other) const;

	FORCEINLINE Vector3D operator+(float Amount) const;
	FORCEINLINE Vector3D operator-(float Amount) const;
	FORCEINLINE Vector3D operator*(float Amount) const;
	FORCEINLINE Vector3D operator/(float Amount) const;

	bool operator==(const Vector3D& Other) const;
	bool operator!=(const Vector3D& Other) const;
	bool Equals(const Vector3D& Other, float ErrorMargin=1.e-4f) const;
	bool Equals(float Val, float ErrorMargin=1.e-4f) const;

	FORCEINLINE Vector3D operator-() const;
	FORCEINLINE Vector3D operator+=(const Vector3D& Other);
	FORCEINLINE Vector3D operator-=(const Vector3D& Other);
	FORCEINLINE Vector3D operator*=(const Vector3D& Other);
	FORCEINLINE Vector3D operator/=(const Vector3D& Other);

	FORCEINLINE Vector3D operator+=(float Val);
	FORCEINLINE Vector3D operator-=(float Val);
	FORCEINLINE Vector3D operator*=(float Val);
	FORCEINLINE Vector3D operator/=(float Val);

	float operator[](uint32 Index) const;

	void Set(float X, float Y, float Z);
	void Set(uint32 Index, float Val);
	float Max() const;
	float Min() const;
	float AbsMax() const;
	float AbsMin() const;

	Vector3D Abs() const;
	Vector3D Min(const Vector3D& Other) const;
	Vector3D Max(const Vector3D& Other) const;

	FORCEINLINE float Length() const;
	FORCEINLINE float LengthSquared() const;

	/*
	 * @TODO(Anyone): Add a version that takes in an ErrorMargin as an optional param
	 **/
	Vector3D Normalized() const;

	bool GetIsNormalized(float ErrorMargin = 1.e-4f) const;

	void DirAndLength(Vector3D& Direction, float& Length) const;
	Vector3D Project() const;
	Vector3D Reciprocal() const;

	Vector3D Rotate(const Vector3D& axis, float Angle) const;

	Vector3D Reflect(const Vector3D& Normal) const;
	Vector3D Refract(const Vector3D& Normal, float IndexOfRefraction) const;

	Vector3D ToDeg() const;
	Vector3D ToRad() const;

	FORCEINLINE Vector ToVector() const;
	Vector ToVector(float In) const;

private:

	Vector m_Vector;
};

FORCEINLINE Vector3D::Vector3D(const Vector& In) :
	m_Vector(In) {}

FORCEINLINE Vector3D::Vector3D() :
	m_Vector(VectorConstants::ZERO) {}

FORCEINLINE Vector3D::Vector3D(float Val) :
	m_Vector(Vector::Make(Val, Val, Val, 0.0f)) {}

FORCEINLINE Vector3D::Vector3D(float X, float Y, float Z) :
	m_Vector(Vector::Make(X, Y, Z, 0.0f)) {}

FORCEINLINE float Vector3D::Dot(const Vector3D& Other) const
{
	return m_Vector.Dot3(Other.m_Vector)[0];
}

FORCEINLINE Vector3D Vector3D::DotToVector(const Vector3D& Other) const
{
	return Vector3D(m_Vector.Dot3(Other.m_Vector));
}

FORCEINLINE Vector3D Vector3D::Cross(const Vector3D& Other) const
{
	return Vector3D(m_Vector.Cross3(Other.m_Vector));
}

FORCEINLINE float Vector3D::Dist(const Vector3D& Other) const
{
	return Math::Sqrt(DistSquared(Other));
}

FORCEINLINE float Vector3D::DistSquared(const Vector3D& Other) const
{
	Vector3D _Temp = Other - *this;
	return _Temp.m_Vector.Dot3(_Temp.m_Vector)[0];
}


FORCEINLINE Vector3D Vector3D::operator+(const Vector3D& Other) const
{
	return Vector3D(m_Vector + Other.m_Vector);
}

FORCEINLINE Vector3D Vector3D::operator-(const Vector3D& Other) const
{
	return Vector3D(m_Vector - Other.m_Vector);
}

FORCEINLINE Vector3D Vector3D::operator*(const Vector3D& Other) const
{
	return Vector3D(m_Vector * Other.m_Vector);
}

FORCEINLINE Vector3D Vector3D::operator/(const Vector3D& Other) const
{
	return Vector3D(m_Vector / Other.m_Vector);
}


FORCEINLINE Vector3D Vector3D::operator+(float Amount) const
{
	return Vector3D(m_Vector + Vector::Load1f(Amount));
}

FORCEINLINE Vector3D Vector3D::operator-(float Amount) const
{
	return Vector3D(m_Vector - Vector::Load1f(Amount));
}

FORCEINLINE Vector3D Vector3D::operator*(float Amount) const
{
	return Vector3D(m_Vector * Vector::Load1f(Amount));
}

FORCEINLINE Vector3D Vector3D::operator/(float Amount) const
{
	return Vector3D(m_Vector * Vector::Load1f(Math::Reciprocal(Amount)));
}


FORCEINLINE Vector3D Vector3D::operator-() const
{
	return Vector3D(-m_Vector);
}

FORCEINLINE Vector3D Vector3D::operator+=(const Vector3D& Other)
{
	m_Vector = m_Vector + Other.m_Vector;
	return *this;
}

FORCEINLINE Vector3D Vector3D::operator-=(const Vector3D& Other)
{
	m_Vector = m_Vector - Other.m_Vector;
	return *this;
}

FORCEINLINE Vector3D Vector3D::operator*=(const Vector3D& Other)
{
	m_Vector = m_Vector * Other.m_Vector;
	return *this;
}

FORCEINLINE Vector3D Vector3D::operator/=(const Vector3D& Other)
{
	m_Vector = m_Vector / Other.m_Vector;
	return *this;
}


FORCEINLINE float Vector3D::Length() const
{
	return Math::Sqrt(LengthSquared());
}

FORCEINLINE float Vector3D::LengthSquared() const
{
	return m_Vector.Dot3(m_Vector)[0];
}


FORCEINLINE Vector3D Vector3D::operator+=(float Val)
{
	m_Vector = m_Vector + Vector::Load1f(Val);
	return *this;
}

FORCEINLINE Vector3D Vector3D::operator-=(float Val)
{
	m_Vector = m_Vector - Vector::Load1f(Val);
	return *this;
}

FORCEINLINE Vector3D Vector3D::operator*=(float Val)
{
	m_Vector = m_Vector * Vector::Load1f(Val);
	return *this;
}

FORCEINLINE Vector3D Vector3D::operator/=(float Val)
{
	m_Vector = m_Vector * Vector::Load1f(Math::Reciprocal(Val));
	return *this;
}

FORCEINLINE Vector Vector3D::ToVector() const
{
	return m_Vector;
}


/*
 *	Everything needed to construct a 2D Vector
 *	Default is X, Y
 **/
class Vector2D
{

public:

	FORCEINLINE Vector2D();
	FORCEINLINE Vector2D(float Val);
	FORCEINLINE Vector2D(float X, float Y);

	FORCEINLINE float Dot(const Vector2D& Other) const;
	FORCEINLINE Vector2D DotToVector(const Vector2D& Other) const;
	FORCEINLINE float Cross(const Vector2D& Other) const;
	FORCEINLINE float Distance(const Vector2D& Other) const;
	FORCEINLINE float DistanceSquared(const Vector2D& Other) const;

	FORCEINLINE Vector2D operator+(const Vector2D& Other) const;
	FORCEINLINE Vector2D operator-(const Vector2D& Other) const;
	FORCEINLINE Vector2D operator*(const Vector2D& Other) const;
	FORCEINLINE Vector2D operator/(const Vector2D& Other) const;

	FORCEINLINE Vector2D operator+(float Amount) const;
	FORCEINLINE Vector2D operator-(float Amount) const;
	FORCEINLINE Vector2D operator*(float Amount) const;
	FORCEINLINE Vector2D operator/(float Amount) const;

	bool operator==(const Vector2D& Other) const;
	bool operator!=(const Vector2D& Other) const;
	bool Equals(const Vector2D& Other, float ErrorMargin=1.e-4f) const;
	bool Equals(float Val, float ErrorMargin=1.e-4f) const;

	FORCEINLINE Vector2D operator-() const;
	FORCEINLINE Vector2D operator+=(const Vector2D& Other);
	FORCEINLINE Vector2D operator-=(const Vector2D& Other);
	FORCEINLINE Vector2D operator*=(const Vector2D& Other);
	FORCEINLINE Vector2D operator/=(const Vector2D& Other);

	FORCEINLINE Vector2D operator+=(float Val);
	FORCEINLINE Vector2D operator-=(float Val);
	FORCEINLINE Vector2D operator*=(float Val);
	FORCEINLINE Vector2D operator/=(float Val);

	float operator[](uint32 Index) const;

	void Set(float X, float Y);
	void Set(uint32 Index, float Val);
	float Max() const;
	float Min() const;
	float AbsMax() const;
	float AbsMin() const;

	Vector2D Abs() const;
	Vector2D Min(const Vector2D& Other) const;
	Vector2D Max(const Vector2D& Other) const;

	FORCEINLINE float Length() const;
	FORCEINLINE float LengthSquared() const;

	Vector2D Normalized(float ErrorMargin=1.e-8f) const;
	bool GetIsNormalized(float ErrorMargin=1.e-4f) const;

	void DirectionLength(Vector2D& Direction, float& Length, float ErrorMargin=1.e-8f) const;
	Vector2D Reciprocal() const;

	Vector2D Rotate(float Angle) const;

	Vector2D Reflect(const Vector2D& Normal) const;
	Vector2D Refract(const Vector2D& Normal, float IndexOfRefraction) const;

	Vector2D ToDeg() const;
	Vector2D ToRad() const;

	Vector ToVector2D() const;
	Vector ToVector2D(float Z, float In) const;
	Vector ToVector2D(Vector2D Other) const;

private:

	float m_Vector[2];
};

FORCEINLINE Vector2D::Vector2D()
{
	m_Vector[0] = 0.f;
	m_Vector[1] = 0.f;
}
FORCEINLINE Vector2D::Vector2D(float Val)
{
	m_Vector[0] = Val;
	m_Vector[1] = Val;
}
FORCEINLINE Vector2D::Vector2D(float X, float Y)
{
	m_Vector[0] = X;
	m_Vector[1] = Y;
}

FORCEINLINE float Vector2D::Dot(const Vector2D& Other) const
{
	return m_Vector[0] * Other.m_Vector[0] + m_Vector[1] * Other.m_Vector[1];
}
FORCEINLINE Vector2D Vector2D::DotToVector(const Vector2D& Other) const
{
	return Vector2D(Dot(Other));
}

FORCEINLINE float Vector2D::Cross(const Vector2D& Other) const
{
	return m_Vector[0] * Other.m_Vector[1] - m_Vector[1] * Other.m_Vector[0];
}

FORCEINLINE float Vector2D::Distance(const Vector2D& Other) const
{
	return Math::Sqrt(DistanceSquared(Other));
}

FORCEINLINE float Vector2D::DistanceSquared(const Vector2D& Other) const
{
	return (Other - *this).LengthSquared();
}

FORCEINLINE float Vector2D::Length() const
{
	return Math::Sqrt(LengthSquared());
}

FORCEINLINE float Vector2D::LengthSquared() const
{
	return Dot(*this);
}

FORCEINLINE Vector2D Vector2D::operator+(const Vector2D& Other) const
{
	return Vector2D(m_Vector[0] + Other.m_Vector[0], m_Vector[1] + Other.m_Vector[1]);
}
FORCEINLINE Vector2D Vector2D::operator-(const Vector2D& Other) const
{
	return Vector2D(m_Vector[0] - Other.m_Vector[0], m_Vector[1] - Other.m_Vector[1]);
}
FORCEINLINE Vector2D Vector2D::operator*(const Vector2D& Other) const
{
	return Vector2D(m_Vector[0] * Other.m_Vector[0], m_Vector[1] * Other.m_Vector[1]);
}
FORCEINLINE Vector2D Vector2D::operator/(const Vector2D& Other) const
{
	return Vector2D(m_Vector[0] / Other.m_Vector[0], m_Vector[1] / Other.m_Vector[1]);
}

FORCEINLINE Vector2D Vector2D::operator+(float Amount) const
{
	return Vector2D(m_Vector[0] + Amount, m_Vector[1] + Amount);
}
FORCEINLINE Vector2D Vector2D::operator-(float Amount) const
{
	return Vector2D(m_Vector[0] - Amount, m_Vector[1] - Amount);
}
FORCEINLINE Vector2D Vector2D::operator*(float Amount) const
{
	return Vector2D(m_Vector[0] * Amount, m_Vector[1] * Amount);
}
FORCEINLINE Vector2D Vector2D::operator/(float Amount) const
{
	return Vector2D(m_Vector[0] / Amount, m_Vector[1] / Amount);
}

FORCEINLINE Vector2D Vector2D::operator-() const
{
	return Vector2D(-m_Vector[0], -m_Vector[1]);
}

FORCEINLINE Vector2D Vector2D::operator+=(const Vector2D& Other)
{
	m_Vector[0] += Other.m_Vector[0];
	m_Vector[1] += Other.m_Vector[1];
	return *this;
}

FORCEINLINE Vector2D Vector2D::operator-=(const Vector2D& Other)
{
	m_Vector[0] -= Other.m_Vector[0];
	m_Vector[1] -= Other.m_Vector[1];
	return *this;
}

FORCEINLINE Vector2D Vector2D::operator*=(const Vector2D& Other)
{
	m_Vector[0] *= Other.m_Vector[0];
	m_Vector[1] *= Other.m_Vector[1];
	return *this;
}

FORCEINLINE Vector2D Vector2D::operator/=(const Vector2D& Other)
{
	m_Vector[0] /= Other.m_Vector[0];
	m_Vector[1] /= Other.m_Vector[1];
	return *this;
}

FORCEINLINE Vector2D Vector2D::operator+=(float Val)
{
	m_Vector[0] += Val;
	m_Vector[1] += Val;
	return *this;
}

FORCEINLINE Vector2D Vector2D::operator-=(float Val)
{
	m_Vector[0] -= Val;
	m_Vector[1] -= Val;
	return *this;
}

FORCEINLINE Vector2D Vector2D::operator*=(float Val)
{
	m_Vector[0] *= Val;
	m_Vector[1] *= Val;
	return *this;
}

FORCEINLINE Vector2D Vector2D::operator/=(float Val)
{
	m_Vector[0] /= Val;
	m_Vector[1] /= Val;
	return *this;
}

