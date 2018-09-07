#pragma once

Cartesian2D::Cartesian2D()
{
	m_Vector[0] = 0.f;
	m_Vector[1] = 0.f;
}
Cartesian2D::Cartesian2D(float Val)
{
	m_Vector[0] = Val;
	m_Vector[1] = Val;
}
Cartesian2D::Cartesian2D(float X, float Y)
{
	m_Vector[0] = X;
	m_Vector[1] = Y;
}

float Cartesian2D::Dot(const Cartesian2D& Other) const
{
	return m_Vector[0] * Other.m_Vector[0] + m_Vector[1] * Other.m_Vector[1];
}
Cartesian2D Cartesian2D::DotAsS2D(const Cartesian2D& Other) const
{
	return Cartesian2D(Dot(Other));
}

float Cartesian2D::Cross(const Cartesian2D& Other) const
{
	return m_Vector[0] * Other.m_Vector[1] - m_Vector[1] * Other.m_Vector[0];
}

float Cartesian2D::Distance(const Cartesian2D& Other) const
{
	return Math::Sqrt(DistanceSquared(Other));
}

float Cartesian2D::DistanceSquared(const Cartesian2D& Other) const
{
	return (Other - *this).LengthSquared();
}

float Cartesian2D::Length() const
{
	return Math::Sqrt(LengthSquared());
}

float Cartesian2D::LengthSquared() const
{
	return Dot(*this);
}

Cartesian2D Cartesian2D::operator+(const Cartesian2D& Other) const
{
	return Cartesian2D(m_Vector[0] + Other.m_Vector[0], m_Vector[1] + Other.m_Vector[1]);
}

Cartesian2D Cartesian2D::operator-(const Cartesian2D& Other) const
{
	return Cartesian2D(m_Vector[0] - Other.m_Vector[0], m_Vector[1] - Other.m_Vector[1]);
}

Cartesian2D Cartesian2D::operator*(const Cartesian2D& Other) const
{
	return Cartesian2D(m_Vector[0] * Other.m_Vector[0], m_Vector[1] * Other.m_Vector[1]);
}

Cartesian2D Cartesian2D::operator/(const Cartesian2D& Other) const
{
	return Cartesian2D(m_Vector[0] / Other.m_Vector[0], m_Vector[1] / Other.m_Vector[1]);
}

Cartesian2D Cartesian2D::operator+(float Amount) const
{
	return Cartesian2D(m_Vector[0] + Amount, m_Vector[1] + Amount);
}

Cartesian2D Cartesian2D::operator-(float Amount) const
{
	return Cartesian2D(m_Vector[0] - Amount, m_Vector[1] - Amount);
}

Cartesian2D Cartesian2D::operator*(float Amount) const
{
	return Cartesian2D(m_Vector[0] * Amount, m_Vector[1] * Amount);
}

Cartesian2D Cartesian2D::operator/(float Amount) const
{
	return Cartesian2D(m_Vector[0] / Amount, m_Vector[1] / Amount);
}

Cartesian2D Cartesian2D::operator-() const
{
	return Cartesian2D(-m_Vector[0], -m_Vector[1]);
}

Cartesian2D& Cartesian2D::operator+=(const Cartesian2D& Other)
{
	m_Vector[0] += Other.m_Vector[0];
	m_Vector[1] += Other.m_Vector[1];
	return *this;
}

Cartesian2D& Cartesian2D::operator-=(const Cartesian2D& Other)
{
	m_Vector[0] -= Other.m_Vector[0];
	m_Vector[1] -= Other.m_Vector[1];
	return *this;
}

Cartesian2D& Cartesian2D::operator*=(const Cartesian2D& Other)
{
	m_Vector[0] *= Other.m_Vector[0];
	m_Vector[1] *= Other.m_Vector[1];
	return *this;
}

Cartesian2D& Cartesian2D::operator/=(const Cartesian2D& Other)
{
	m_Vector[0] /= Other.m_Vector[0];
	m_Vector[1] /= Other.m_Vector[1];
	return *this;
}

Cartesian2D& Cartesian2D::operator+=(float Val)
{
	m_Vector[0] += Val;
	m_Vector[1] += Val;
	return *this;
}

Cartesian2D& Cartesian2D::operator-=(float Val)
{
	m_Vector[0] -= Val;
	m_Vector[1] -= Val;
	return *this;
}

Cartesian2D& Cartesian2D::operator*=(float Val)
{
	m_Vector[0] *= Val;
	m_Vector[1] *= Val;
	return *this;
}

Cartesian2D& Cartesian2D::operator/=(float Val)
{
	m_Vector[0] /= Val;
	m_Vector[1] /= Val;
	return *this;
}

bool Cartesian2D::operator==(const Cartesian2D& Other) const
{
	return m_Vector[0] == Other.m_Vector[0] && m_Vector[1] == Other.m_Vector[1];
}

bool Cartesian2D::operator!=(const Cartesian2D& Other) const
{
	return m_Vector[0] != Other.m_Vector[0] || m_Vector[1] != Other.m_Vector[1];
}

bool Cartesian2D::Equals(const Cartesian2D& Other, float ErrorMargin) const
{
	return (m_Vector[0] - Other.m_Vector[0]) < ErrorMargin && (m_Vector[1] - Other.m_Vector[1]) < ErrorMargin;
}

bool Cartesian2D::Equals(float Val, float ErrorMargin) const
{
	return (m_Vector[0] - Val) < ErrorMargin && (m_Vector[1] - Val) < ErrorMargin;
}

float Cartesian2D::operator[](uint32 Index) const
{
	assertCheck(Index < 2);
	return m_Vector[Index];
}

void Cartesian2D::Set(float X, float Y)
{
	m_Vector[0] = X;
	m_Vector[1] = Y;
}

void Cartesian2D::Set(uint32 Index, float Val)
{
	assertCheck(Index < 2);
	m_Vector[Index] = Val;
}

float Cartesian2D::Max() const
{
	return Math::Max(m_Vector[0], m_Vector[1]);
}

float Cartesian2D::Min() const
{
	return Math::Min(m_Vector[0], m_Vector[1]);
}

float Cartesian2D::AbsMax() const
{
	return Math::Max(Math::Abs(m_Vector[0]), Math::Abs(m_Vector[1]));
}

float Cartesian2D::AbsMin() const
{
	return Math::Min(Math::Abs(m_Vector[0]), Math::Abs(m_Vector[1]));
}

Cartesian2D Cartesian2D::Abs() const
{
	return Cartesian2D(Math::Abs(m_Vector[0]), Math::Abs(m_Vector[1]));
}

Cartesian2D Cartesian2D::Min(const Cartesian2D& Other) const
{
	return Cartesian2D(Math::Min(m_Vector[0], Other.m_Vector[0]), Math::Min(m_Vector[1], Other.m_Vector[1]));
}

Cartesian2D Cartesian2D::Max(const Cartesian2D& Other) const
{
	return Cartesian2D(Math::Max(m_Vector[0], Other.m_Vector[0]), Math::Max(m_Vector[1], Other.m_Vector[1]));
}

bool Cartesian2D::GetIsNormalized(float ErrorMargin) const
{
	return Math::Abs(1.f - LengthSquared()) < ErrorMargin;
}

Cartesian2D Cartesian2D::Reflect(const Cartesian2D& Normal) const
{
	Cartesian2D _DotAmount = Cartesian2D(2.f * Dot(Normal));
	return (*this) - (Normal * _DotAmount);
}

Cartesian2D Cartesian2D::Reciprocal() const
{
	return Cartesian2D(Math::Reciprocal(m_Vector[0]), Math::Reciprocal(m_Vector[1]));
}

Cartesian2D Cartesian2D::ToDeg() const
{
	return Cartesian2D(MATH_RAD_TO_DEG_CONV * m_Vector[0], MATH_RAD_TO_DEG_CONV * m_Vector[1]);
}

Cartesian2D Cartesian2D::ToRad() const
{
	return Cartesian2D(MATH_DEG_TO_RAD_CONV * m_Vector[0], MATH_DEG_TO_RAD_CONV * m_Vector[1]);
}

Vector Cartesian2D::AsIntrinsic() const
{
	return AsIntrinsic(0.f, 0.f);
}

Vector Cartesian2D::AsIntrinsic(float Z, float In) const
{
	return AsIntrinsic(Cartesian2D(Z, In));
}

Vector Cartesian2D::AsIntrinsic(Cartesian2D Other) const
{
	return Vector::Make(m_Vector[0], m_Vector[1], Other.m_Vector[0], Other.m_Vector[1]);
}