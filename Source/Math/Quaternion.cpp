#include "Quaternion.h"

const Quaternion Quaternion::Identity{0.f, 0.f, 0.f, 1.f};

Quaternion Quaternion::Normalized(float errorMargin) const
{
	static const Vector defaultQuat = Vector::Make(0.0f, 0.0f, 0.0f, 1.0f);
	Vector lenSq = m_Vector.Dot4(m_Vector);
	Vector mask = lenSq >= Vector::Load1f(errorMargin);
	Vector normalizedVec = m_Vector * lenSq.rSqrt();
	return Quaternion(normalizedVec.Select(mask, defaultQuat));
}

bool Quaternion::IsNormalized(float errorMargin) const
{
	return Math::Abs(1.0f - LengthSquared()) < errorMargin;
}


Spatial3D Quaternion::GetAxis() const
{
	float w = m_Vector[3];
	float rangleDivisor = Math::rSqrt(Math::Max(1.0f - w*w, 0.0f));
	return Spatial3D(m_Vector * Vector::Load1f(rangleDivisor));
}

float Quaternion::GetAngle() const
{
	return 2.0f * Math::Arccos(m_Vector[3]);
}

void Quaternion::AxisAndAngle(Spatial3D& axis, float& angle) const
{
	angle = GetAngle();
	axis = GetAxis();
}


Spatial3D Quaternion::Rotate(const Spatial3D& other) const
{
	return Spatial3D(m_Vector.QuatRotateVec(other.AsIntrinsic()));
}

Quaternion Quaternion::Slerp(const Quaternion& dest, float amt, float errorMargin) const
{
	float cosAngleInitial = Dot(dest);
	float cosAngle = Math::Select(cosAngleInitial, cosAngleInitial, -cosAngleInitial);

	float lerpAmt1 = 1.0f - amt;
	float lerpAmt2 = amt;
	if(cosAngle < (1.0f - errorMargin))
	{
		float rsinAngle = Math::rSqrt(1.0f - cosAngle * cosAngle);
		float angle = Math::Arccos(cosAngle);
		// NOTE: You can also get rsinangle from doing
		//     Math::reciprocal(Math::sin(angle));
		lerpAmt1 = Math::Sin(lerpAmt1 * angle) * rsinAngle;
		lerpAmt2 = Math::Sin(lerpAmt2 * angle) * rsinAngle;
	}

	lerpAmt2 = Math::Select(cosAngleInitial, lerpAmt2, -lerpAmt2);

	Vector lerpAmt1Vec = Vector::Load1f(lerpAmt1);
	Vector lerpAmt2Vec = Vector::Load1f(lerpAmt2);
	return Quaternion(m_Vector * lerpAmt1Vec + dest.m_Vector * lerpAmt2Vec);
}

Quaternion Quaternion::Conjugate() const
{
	static const Vector inverter = Vector::Make(-1.0f, -1.0f, -1.0f, 1.0f);
	return Quaternion(m_Vector * inverter);
}

Quaternion Quaternion::Inverse() const
{
	return Normalized().Conjugate();
}

