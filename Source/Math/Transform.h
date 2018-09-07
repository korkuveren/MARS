#pragma once

#include "Cartesian.h"
#include "Quaternion.h"
#include "Matrix.h"

class Transform
{
public:
	FORCEINLINE Transform() :
		m_Translation(Spatial3D::Identity),
		m_Rotation(Quaternion::Identity),
		m_Scale(Spatial3D::One) {}

	FORCEINLINE Transform(const Spatial3D& translationIn) :
		m_Translation(translationIn),
		m_Rotation(Quaternion::Identity),
		m_Scale(Spatial3D::One) {}

	FORCEINLINE Transform(const Quaternion& rotationIn) :
		m_Translation(Spatial3D::Identity),
		m_Rotation(rotationIn),
		m_Scale(Spatial3D::One) {}

	FORCEINLINE Transform(const Spatial3D& translationIn, const Quaternion& rotationIn,
			const Spatial3D& scaleIn) :
		m_Translation(translationIn),
		m_Rotation(rotationIn),
		m_Scale(scaleIn) {}

	FORCEINLINE Vector Transforms(const Vector& vector) const;
	FORCEINLINE Vector Transforms(const Spatial3D& vector, float w) const;
	FORCEINLINE Vector InverseTransform(const Vector& vector) const;
	FORCEINLINE Vector InverseTransform(const Spatial3D& vector, float w) const;
	FORCEINLINE Matrix ToMatrix() const;
	Matrix Inverse() const;
	FORCEINLINE void NormalizeRotation();
	FORCEINLINE bool IsRotationNormalized();

	FORCEINLINE Transform operator+(const Transform& other) const;
	FORCEINLINE Transform operator*(const Transform& other) const;

	FORCEINLINE Transform& operator+=(const Transform& other);
	FORCEINLINE Transform& operator*=(const Transform& other);

	FORCEINLINE Transform operator*(float other) const;
	FORCEINLINE Transform& operator*=(float other);

	FORCEINLINE Spatial3D GetTranslation() const;
	FORCEINLINE Quaternion GetRotation() const;
	FORCEINLINE Spatial3D GetScale() const;
	FORCEINLINE void Set(const Spatial3D& translation,
			const Quaternion& rotation, const Spatial3D& scale);
	FORCEINLINE void SetTranslation(const Spatial3D& translation);
	FORCEINLINE void SetRotation(const Quaternion& rotation);
	FORCEINLINE void SetScale(const Spatial3D& scale);

private:

	Spatial3D m_Translation;
	Quaternion m_Rotation;
	Spatial3D m_Scale;
};

FORCEINLINE Matrix Transform::ToMatrix() const
{
	return Matrix::TransformMatrix(m_Translation.Inner(), m_Rotation, m_Scale.Inner());
}

FORCEINLINE void Transform::NormalizeRotation()
{
	m_Rotation = m_Rotation.Normalized();
}

FORCEINLINE bool Transform::IsRotationNormalized()
{
	return m_Rotation.IsNormalized();
}

FORCEINLINE Transform Transform::operator+(const Transform& other) const
{
	return Transform(m_Translation + other.m_Translation,
			m_Rotation + other.m_Rotation, m_Scale + other.m_Scale);
}

FORCEINLINE Transform& Transform::operator+=(const Transform& other)
{
	m_Translation += other.m_Translation;
	m_Rotation += other.m_Rotation;
	m_Scale += other.m_Scale;
	return *this;
}

FORCEINLINE Transform Transform::operator*(const Transform& other) const
{
	return Transform(m_Translation * other.m_Translation,
			m_Rotation * other.m_Rotation, m_Scale * other.m_Scale);
}

FORCEINLINE Transform& Transform::operator*=(const Transform& other)
{
	m_Translation *= other.m_Translation;
	m_Rotation *= other.m_Rotation;
	m_Scale *= other.m_Scale;
	return *this;
}

FORCEINLINE Transform Transform::operator*(float other) const
{
	return Transform(m_Translation * other,
			m_Rotation * other, m_Scale * other);
}

FORCEINLINE Transform& Transform::operator*=(float other)
{
	m_Translation *= other;
	m_Rotation *= other;
	m_Scale *= other;
	return *this;
}

FORCEINLINE Vector Transform::Transforms(const Spatial3D& vector, float w) const
{
	return (m_Rotation.Rotate((m_Scale * vector).Inner()) + (m_Translation.Inner()) * w).AsIntrinsic(0.0f);
}

FORCEINLINE Vector Transform::Transforms(const Vector& vector) const
{
	return Transforms(Spatial3D(vector), vector[3]);
}

FORCEINLINE Vector Transform::InverseTransform(const Spatial3D& vector, float w) const
{
	return (m_Rotation.Inverse().Rotate((vector - (m_Translation) * w).Inner()) * m_Scale.Reciprocal().Inner()).AsIntrinsic(0.0f);
}

FORCEINLINE Vector Transform::InverseTransform(const Vector& vector) const
{
	return InverseTransform(Spatial3D(vector), vector[3]);
}


FORCEINLINE Spatial3D Transform::GetTranslation() const
{
	return m_Translation;
}

FORCEINLINE Quaternion Transform::GetRotation() const
{
	return m_Rotation;
}

FORCEINLINE Spatial3D Transform::GetScale() const
{
	return m_Scale;
}

FORCEINLINE void Transform::Set(const Spatial3D& translationIn,
		const Quaternion& rotationIn, const Spatial3D& scaleIn)
{
	m_Translation = translationIn;
	m_Rotation = rotationIn;
	m_Scale = scaleIn;
}

FORCEINLINE void Transform::SetTranslation(const Spatial3D& val)
{
	m_Translation = val;
}

FORCEINLINE void Transform::SetRotation(const Quaternion& val)
{
	m_Rotation = val;
}

FORCEINLINE void Transform::SetScale(const Spatial3D& val)
{
	m_Scale = val;
}

