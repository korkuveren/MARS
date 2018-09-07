#include "Cartesian.h"

const Cartesian2D Cartesian2D::Identity	{ 0.f };
const Cartesian2D Cartesian2D::FrontDir	{ 1.f, 0.f };
const Cartesian2D Cartesian2D::UpDir	{ 0.f, 1.f };// ermagerd reeeeeeeee
const Cartesian2D Cartesian2D::One		{ 1.f };

const Spatial3D Spatial3D::Identity		{ 0.f };
const Spatial3D Spatial3D::FrontDir		{ 1.f, 0.f, 0.f };
const Spatial3D Spatial3D::RightDir		{ 0.f, 1.f, 0.f };
const Spatial3D Spatial3D::UpDir		{ 0.f, 0.f, 1.f };
const Spatial3D Spatial3D::One			{ 1.f };

const Euler3D Euler3D::ZeroEuler		{ 0.f };
const Euler3D Euler3D::EulerToDeg		{ MATH_RAD_TO_DEG_CONV };
const Euler3D Euler3D::EulerToRad		{ MATH_DEG_TO_RAD_CONV };

Cartesian2D& Cartesian2D::Normalize(float ErrorMargin /*= 1.e-8f*/)
{
	return (*this = Normalized(ErrorMargin));
}

Cartesian2D Cartesian2D::Normalized(float ErrorMargin) const
{
	float _LengthSq = LengthSquared();
	if (_LengthSq < ErrorMargin)
	{
		return Cartesian2D(0.f, 0.f);
	}

	return (*this) * Math::rSqrt(_LengthSq);
}

void Cartesian2D::ToDirectionAndLength(Cartesian2D& Direction, float& Length, float ErrorMargin) const
{
	float _LengthSq = LengthSquared();
	if (_LengthSq < ErrorMargin)
	{
		Direction = Cartesian2D(0.f, 0.f);
		Length = 0;
		return;
	}

	float _rLen = Math::rSqrt(_LengthSq);
	Direction = (*this) * _rLen;
	Length = Math::Reciprocal(_rLen);
}

Cartesian2D Cartesian2D::Rotate(float Angle) const
{
	float _Sin, _Cos;
	Math::SinCos(&_Sin, &_Cos, Angle);
	return Cartesian2D(_Cos * m_Vector[0] - _Sin * m_Vector[1], _Sin * m_Vector[0] + _Cos * m_Vector[1]);
}

Cartesian2D Cartesian2D::Refract(const Cartesian2D& Normal, float IndexOfRefraction) const
{
	float _CosNormalAngle = Dot(Normal);
	float _RefractanceSquared = 1.f - IndexOfRefraction * IndexOfRefraction * (1.f - _CosNormalAngle * _CosNormalAngle);

	if (_RefractanceSquared < 0.f)
	{
		return Cartesian2D(0.f);
	}

	float _NormalScale = IndexOfRefraction * _CosNormalAngle + Math::Sqrt(_RefractanceSquared);
	Cartesian2D _NormalScaleVec(_NormalScale);
	Cartesian2D _IndexOfRefractionVec(IndexOfRefraction);

	return (*this) * _IndexOfRefractionVec - _NormalScaleVec * Normal;
}

Spatial3D& Spatial3D::Normalize(float ErrorMargin /*= SUPER_SMALL_NUMBER*/)
{
	return (*this = Normalized(ErrorMargin));	
}

Spatial3D Spatial3D::Normalized(float ErrorMargin /*= SUPER_SMALL_NUMBER*/) const
{
	if (LengthSquared() < ErrorMargin) return Spatial3D(0.f);
	return Spatial3D(m_Inner.m_Vector.Normalize3());
}

void Spatial3D::DirAndLength(Spatial3D& Direction, float& Length) const
{
	Vector _Len = m_Inner.m_Vector.Dot3(m_Inner.m_Vector).rSqrt();
	Direction = Spatial3D(m_Inner.m_Vector * _Len);
	Length = Math::Reciprocal(_Len[0]);
}

Spatial3D Spatial3D::Rotate(const Spatial3D& Axis, float Angle) const
{
	float SinAngle;
	float CosAngle;
	
	Math::SinCos(&SinAngle, &CosAngle, -Angle);
	Vector _SinVec = Vector::Load1f(SinAngle);
	Vector _CosVec = Vector::Load1f(CosAngle);
	Vector _OneMinusCosVec = Vector::Load1f(1.f - CosAngle);
	
	Vector _RotatedX = m_Inner.m_Vector.Cross3(Axis.Inner().m_Vector * _SinVec);
	Vector _RotatedY = Axis.Inner().m_Vector * m_Inner.m_Vector.Dot3(Axis.Inner().m_Vector * _OneMinusCosVec);
	Vector _RotatedZ = m_Inner.m_Vector * _CosVec;

	return Spatial3D(_RotatedX + _RotatedY + _RotatedZ);
}

Spatial3D Spatial3D::Refract(const Spatial3D& Normal, float IndexOfRefraction) const
{
	float _CosNormalAngle = m_Inner.m_Vector.Dot3(Normal.Inner().m_Vector)[0];
	float _RefractanceSquared =  1.f - IndexOfRefraction * IndexOfRefraction * (1.f - _CosNormalAngle * _CosNormalAngle);
	
	if (_RefractanceSquared < 0.f) 
	{
		return Spatial3D(VectorConstants::ZERO);
	}

	float _NormalScale = IndexOfRefraction * _CosNormalAngle + Math::Sqrt(_RefractanceSquared);
	Vector _NormalScaleVec(Vector::Load1f(_NormalScale));
	Vector _IindexOfRefractionVec(Vector::Load1f(IndexOfRefraction));
	
	return Spatial3D(m_Inner.m_Vector * _IindexOfRefractionVec - _NormalScaleVec * Normal.Inner().m_Vector);
}

Euler3D::Euler3D(const Spatial3D& FromDirection)
{

}
