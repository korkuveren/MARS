#pragma once

Cartesian3D::Cartesian3D(const Vector& In) :
	m_Vector(In) {}

Cartesian3D::Cartesian3D() :
	m_Vector(VectorConstants::ZERO) {}

Cartesian3D::Cartesian3D(float Val) :
	m_Vector(Vector::Make(Val, Val, Val, 0.0f)) {}

Cartesian3D::Cartesian3D(float X, float Y, float Z) :
	m_Vector(Vector::Make(X, Y, Z, 0.0f)) {}

Cartesian3D::Cartesian3D(const Cartesian2D& In) :
	m_Vector(Vector::Make(In[0], In[1], 0.f, 0.f)) {}

Cartesian3D::Cartesian3D(const Cartesian2D& In, float Z) :
	m_Vector(Vector::Make(In[0], In[1], Z, 0.f)) {}

Cartesian3D Cartesian3D::operator+(const Cartesian3D& Other) const
{
	return Cartesian3D(m_Vector + Other.m_Vector);
}

Cartesian3D Cartesian3D::operator-(const Cartesian3D& Other) const
{
	return Cartesian3D(m_Vector - Other.m_Vector);
}

Cartesian3D Cartesian3D::operator*(const Cartesian3D& Other) const
{
	return Cartesian3D(m_Vector * Other.m_Vector);
}

Cartesian3D Cartesian3D::operator/(const Cartesian3D& Other) const
{
	return Cartesian3D(m_Vector / Other.m_Vector);
}

Cartesian3D Cartesian3D::operator+(float Amount) const
{
	return Cartesian3D(m_Vector + Vector::Load1f(Amount));
}

Cartesian3D Cartesian3D::operator-(float Amount) const
{
	return Cartesian3D(m_Vector - Vector::Load1f(Amount));
}

Cartesian3D Cartesian3D::operator*(float Amount) const
{
	return Cartesian3D(m_Vector * Vector::Load1f(Amount));
}

Cartesian3D Cartesian3D::operator/(float Amount) const
{
	return Cartesian3D(m_Vector * Vector::Load1f(Math::Reciprocal(Amount)));
}

Cartesian3D Cartesian3D::operator-() const
{
	return Cartesian3D(-m_Vector);
}

Cartesian3D& Cartesian3D::operator+=(const Cartesian3D& Other)
{
	m_Vector = m_Vector + Other.m_Vector;
	return *this;
}

Cartesian3D& Cartesian3D::operator-=(const Cartesian3D& Other)
{
	m_Vector = m_Vector - Other.m_Vector;
	return *this;
}

Cartesian3D& Cartesian3D::operator*=(const Cartesian3D& Other)
{
	m_Vector = m_Vector * Other.m_Vector;
	return *this;
}

Cartesian3D& Cartesian3D::operator/=(const Cartesian3D& Other)
{
	m_Vector = m_Vector / Other.m_Vector;
	return *this;
}

Cartesian3D& Cartesian3D::operator+=(float Val)
{
	m_Vector = m_Vector + Vector::Load1f(Val);
	return *this;
}

Cartesian3D& Cartesian3D::operator-=(float Val)
{
	m_Vector = m_Vector - Vector::Load1f(Val);
	return *this;
}

Cartesian3D& Cartesian3D::operator*=(float Val)
{
	m_Vector = m_Vector * Vector::Load1f(Val);
	return *this;
}

Cartesian3D& Cartesian3D::operator/=(float Val)
{
	m_Vector = m_Vector * Vector::Load1f(Math::Reciprocal(Val));
	return *this;
}


bool Cartesian3D::operator==(const Cartesian3D& Other) const
{
	return (m_Vector != Other.m_Vector).IsZero3f();
}

bool Cartesian3D::operator!=(const Cartesian3D& Other) const
{
	return !(*this == Other);
}

bool Cartesian3D::Equals(const Cartesian3D& Other, float ErrorMargin) const
{
	return m_Vector.NotEquals(Other.m_Vector, ErrorMargin).IsZero3f();
}

bool Cartesian3D::Equals(float Val, float ErrorMargin) const
{
	return Equals(Cartesian3D(Val), ErrorMargin);
}

float Cartesian3D::operator[](uint32 Index) const
{
	assertCheck(Index < 3);
	return m_Vector[Index];
}

Vector Cartesian3D::AsIntrinsic() const
{
	return m_Vector;
}

Vector Cartesian3D::AsIntrinsic(float In) const
{
	return m_Vector.Select(VectorConstants::MASK_W, Vector::Load1f(In));
}

void Cartesian3D::Set(float X, float Y, float Z)
{
	m_Vector = Vector::Make(X, Y, Z, 0.f);
}

void Cartesian3D::Set(uint32 Index, float Val)
{
	m_Vector = m_Vector.Select(Vector::Mask(Index), Vector::Load1f(Val));
}

float Cartesian3D::Max() const
{
	float _Vals[3];
	m_Vector.Store3f(_Vals);
	return Math::Max3(_Vals[0], _Vals[1], _Vals[2]);
}

float Cartesian3D::Min() const
{
	float _Vals[3];
	m_Vector.Store3f(_Vals);
	return Math::Min3(_Vals[0], _Vals[1], _Vals[2]);
}

float Cartesian3D::AbsMax() const
{
	float _Vals[3];
	m_Vector.Abs().Store3f(_Vals);
	return Math::Max3(_Vals[0], _Vals[1], _Vals[2]);
}

float Cartesian3D::AbsMin() const
{
	float _Vals[3];
	m_Vector.Abs().Store3f(_Vals);
	return Math::Min3(_Vals[0], _Vals[1], _Vals[2]);
}

Cartesian3D Cartesian3D::Abs() const
{
	return Cartesian3D(m_Vector.Abs());
}

Cartesian3D Cartesian3D::Min(const Cartesian3D& Other) const
{
	return Cartesian3D(m_Vector.Min(Other.m_Vector));
}

Cartesian3D Cartesian3D::Max(const Cartesian3D& Other) const
{
	return Cartesian3D(m_Vector.Max(Other.m_Vector));
}


Spatial2D Spatial3D::As2D() const
{
	return Spatial2D(m_Inner[0], m_Inner[1]);
}

Spatial3D Spatial3D::Cross(const Spatial3D& Other) const
{
	return Cartesian3D(m_Inner.m_Vector.Cross3(Other.m_Inner.m_Vector));
}

float Spatial3D::Dot(const Spatial3D& Other) const
{
	return m_Inner.m_Vector.Dot3(Other.m_Inner.m_Vector)[0];
}

Spatial3D Spatial3D::DotAs3D(const Spatial3D& Other) const
{
	return Spatial3D(m_Inner.m_Vector.Dot3(Other.m_Inner.m_Vector));
}

float Spatial3D::Dist(const Spatial3D& Other) const
{
	return Math::Sqrt(DistSquared(Other));
}

float Spatial3D::DistSquared(const Spatial3D& Other) const
{
	Spatial3D _Temp = Other - *this;
	return _Temp.m_Inner.m_Vector.Dot3(_Temp.m_Inner.m_Vector)[0];
}

bool Spatial3D::GetIsNormalized(float ErrorMargin) const
{
	return Math::Abs(1.f - LengthSquared()) < ErrorMargin;
}

float Spatial3D::Length() const
{
	return Math::Sqrt(LengthSquared());
}

float Spatial3D::LengthSquared() const
{
	return m_Inner.m_Vector.Dot3(m_Inner.m_Vector)[0];
}

Spatial3D Spatial3D::Project() const
{
	Vector _ProjectVal = Vector::Load1f(Math::Reciprocal(m_Inner.m_Vector[2]));
	return Spatial3D(m_Inner.m_Vector * _ProjectVal);
}

Spatial3D Spatial3D::Reciprocal() const
{
	return Spatial3D(m_Inner.m_Vector.Reciprocal());
}

Spatial3D Spatial3D::Reflect(const Spatial3D& Normal) const
{
	Vector _DotAmount = VectorConstants::TWO * m_Inner.m_Vector.Dot3(Normal.m_Inner.m_Vector);
	return Spatial3D(m_Inner.m_Vector - (Normal.m_Inner.m_Vector * _DotAmount));
}

Euler3D Euler3D::ToDeg() const
{
	return Euler3D(*this * EulerToDeg);
}

Euler3D Euler3D::ToRad() const
{
	return Euler3D(*this * EulerToRad);
}
