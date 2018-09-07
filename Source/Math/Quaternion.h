#pragma once

#include "Cartesian.h"

class Quaternion
{
public:
	static const Quaternion Identity;

	FORCEINLINE Quaternion() {}
	FORCEINLINE Quaternion(const Vector& quat);
	FORCEINLINE Quaternion(float X, float Y, float Z, float W);
	FORCEINLINE Quaternion(const Spatial3D& Axis, float Angle);
	
	FORCEINLINE float operator[](uint32 index) const;
	FORCEINLINE Quaternion operator+(const Quaternion& Other) const;
	FORCEINLINE Quaternion operator+=(const Quaternion& Other);
	FORCEINLINE Quaternion operator-(const Quaternion& Other) const;
	FORCEINLINE Quaternion operator-=(const Quaternion& Other);
	FORCEINLINE Quaternion operator*(const Quaternion& Other) const;
	FORCEINLINE Quaternion operator*=(const Quaternion& Other);
	FORCEINLINE Spatial3D operator*(const Spatial3D& Other) const;
	FORCEINLINE Quaternion operator*(float Amount) const;
	FORCEINLINE Quaternion operator*=(float Amount);
	FORCEINLINE Quaternion operator/(float Amount) const;
	FORCEINLINE Quaternion operator/=(float Amount);

	FORCEINLINE bool operator==(const Quaternion& Other) const;
	FORCEINLINE bool operator!=(const Quaternion& Other) const;
	FORCEINLINE bool Equals(const Quaternion& Other, float ErrorMargin=1.e-4f) const;

	FORCEINLINE float Dot(const Quaternion& Other) const;

	FORCEINLINE float Length() const;
	FORCEINLINE float LengthSquared() const;

	Quaternion Normalized(float ErrorMargin=1.e-8f) const;
	bool IsNormalized(float ErrorMargin=1.e-4f) const;

	Spatial3D GetAxis() const;
	float GetAngle() const;
	void AxisAndAngle(Spatial3D& Axis, float& Angle) const;

	Spatial3D Rotate(const Spatial3D& Other) const;

	// https://en.wikipedia.org/wiki/Rotation_matrix 
	/**	unimplemented */
	Euler3D ToEuler() const { assert(false); return Euler3D::ZeroEuler; }
	/** unimplemented */
	static Quaternion FromEuler(const Euler3D& Euler) { Euler; assert(false); return Quaternion::Identity; }

	Quaternion Slerp(const Quaternion& Destination, float Amount, float ErrorMargin=1.e-4f) const;
	Quaternion Conjugate() const;
	Quaternion Inverse() const;

	FORCEINLINE Spatial3D GetAxisX() const;
	FORCEINLINE Spatial3D GetAxisY() const;
	FORCEINLINE Spatial3D GetAxisZ() const;
	FORCEINLINE Vector AsIntrinsic() const;

private:

	Vector m_Vector;
};

template<>
FORCEINLINE Quaternion Math::Lerp(const Quaternion& val1, const Quaternion& val2,
		const float& Amount)
{
	float dotAmt = val1.Dot(val2);
	float dir = Math::Select(dotAmt, 1.0f, -1.0f);
	return (val2 * Amount) + val1 * (dir * (1.0f - Amount));
}

FORCEINLINE Quaternion::Quaternion(const Vector& quat) :
	m_Vector(quat) {}

FORCEINLINE Quaternion::Quaternion(float X, float Y, float Z, float W) :
	m_Vector(Vector::Make(X, Y, Z, W)) {}

FORCEINLINE Quaternion::Quaternion(const Spatial3D& Axis, float Angle)
{
	float vals[3];
	float sinAngle, cosAngle;
	Math::SinCos(&sinAngle, &cosAngle, Angle * 0.5f);
	Axis.AsIntrinsic().Store3f(vals);

	m_Vector = Vector::Make(
			vals[0] * sinAngle,
			vals[1] * sinAngle,
			vals[2] * sinAngle,
			cosAngle);
}

FORCEINLINE float Quaternion::operator[](uint32 index) const
{
	return m_Vector[index];
}

FORCEINLINE Quaternion Quaternion::operator+(const Quaternion& Other) const
{
	return Quaternion(m_Vector + Other.m_Vector);
}

FORCEINLINE Quaternion Quaternion::operator+=(const Quaternion& Other)
{
	m_Vector = m_Vector + Other.m_Vector;
	return *this;
}

FORCEINLINE Quaternion Quaternion::operator-(const Quaternion& Other) const
{
	return Quaternion(m_Vector - Other.m_Vector);
}

FORCEINLINE Quaternion Quaternion::operator-=(const Quaternion& Other)
{
	m_Vector = m_Vector - Other.m_Vector;
	return *this;
}

FORCEINLINE Quaternion Quaternion::operator*(const Quaternion& Other) const
{
	return Quaternion(m_Vector.QuatMul(Other.m_Vector));	
}

FORCEINLINE Quaternion Quaternion::operator*=(const Quaternion& Other)
{
	m_Vector = m_Vector.QuatMul(Other.m_Vector);
	return *this;
}

FORCEINLINE Spatial3D Quaternion::operator*(const Spatial3D& Other) const
{
	return Rotate(Other);
}

FORCEINLINE Quaternion Quaternion::operator*(float Amount) const
{
	return Quaternion(m_Vector * Vector::Load1f(Amount));
}

FORCEINLINE Quaternion Quaternion::operator*=(float Amount) 
{
	m_Vector = m_Vector * Vector::Load1f(Amount);
	return *this;
}

FORCEINLINE Quaternion Quaternion::operator/(float Amount) const
{
	return *this * Math::Reciprocal(Amount);
}

FORCEINLINE Quaternion Quaternion::operator/=(float Amount)
{
	return *this *= Math::Reciprocal(Amount);
}


FORCEINLINE bool Quaternion::operator==(const Quaternion& Other) const
{
	return (m_Vector != Other.m_Vector).IsZero4f();
}

FORCEINLINE bool Quaternion::operator!=(const Quaternion& Other) const
{
	return (m_Vector == Other.m_Vector).IsZero4f();
}

FORCEINLINE bool Quaternion::Equals(const Quaternion& Other, float ErrorMargin) const
{
	Vector error = Vector::Load1f(ErrorMargin);
	return ((m_Vector - Other.m_Vector).Abs() >= error).IsZero4f() ||
		((m_Vector + Other.m_Vector).Abs() >= error).IsZero4f();
}


FORCEINLINE float Quaternion::Dot(const Quaternion& Other) const
{
	return m_Vector.Dot4(Other.m_Vector)[0];
}


FORCEINLINE float Quaternion::Length() const
{
	return Math::Sqrt(LengthSquared());
}

FORCEINLINE float Quaternion::LengthSquared() const
{
	return m_Vector.Dot4(m_Vector)[0];
}

FORCEINLINE Spatial3D Quaternion::GetAxisX() const
{
	return Rotate(Spatial3D(1.0f, 0.0f, 0.0f));
}

FORCEINLINE Spatial3D Quaternion::GetAxisY() const
{
	return Rotate(Spatial3D(0.0f, 1.0f, 0.0f));
}

FORCEINLINE Spatial3D Quaternion::GetAxisZ() const
{
	return Rotate(Spatial3D(0.0f, 0.0f, 1.0f));
}

FORCEINLINE Vector Quaternion::AsIntrinsic() const
{
	return m_Vector;
}
