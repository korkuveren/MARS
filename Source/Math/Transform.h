#pragma once

#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

class Transform
{
public:
	FORCEINLINE Transform() :
		m_Translation(0.0f, 0.0f, 0.0f),
		m_Rotation(0.0f, 0.0f, 0.0f, 1.0f),
		m_Scale(1.0f, 1.0f, 1.0f) {}

	FORCEINLINE Transform(const Vector3D& translationIn) :
		m_Translation(translationIn),
		m_Rotation(0.0f, 0.0f, 0.0f, 1.0f),
		m_Scale(1.0f, 1.0f, 1.0f) {}

	FORCEINLINE Transform(const Quaternion& rotationIn) :
		m_Translation(0.0f, 0.0f, 0.0f),
		m_Rotation(rotationIn),
		m_Scale(1.0f, 1.0f, 1.0f) {}

	FORCEINLINE Transform(const Vector3D& translationIn, const Quaternion& rotationIn,
			const Vector3D& scaleIn) :
		m_Translation(translationIn),
		m_Rotation(rotationIn),
		m_Scale(scaleIn) {}

	FORCEINLINE Vector Transforms(const Vector& vector) const;
	FORCEINLINE Vector Transforms(const Vector3D& vector, float w) const;
	FORCEINLINE Vector InverseTransform(const Vector& vector) const;
	FORCEINLINE Vector InverseTransform(const Vector3D& vector, float w) const;
	FORCEINLINE Matrix ToMatrix() const;
	Matrix Inverse() const;
	FORCEINLINE void NormalizeRotation();
	FORCEINLINE bool IsRotationNormalized();

	FORCEINLINE Transform operator+(const Transform& other) const;
	FORCEINLINE Transform operator+=(const Transform& other);
	FORCEINLINE Transform operator*(const Transform& other) const;
	FORCEINLINE Transform operator*=(const Transform& other);
	FORCEINLINE Transform operator*(float other) const;
	FORCEINLINE Transform operator*=(float other);

	FORCEINLINE Vector3D GetTranslation() const;
	FORCEINLINE Quaternion GetRotation() const;
	FORCEINLINE Vector3D GetScale() const;
	FORCEINLINE void Set(const Vector3D& translation,
			const Quaternion& rotation, const Vector3D& scale);
	FORCEINLINE void SetTranslation(const Vector3D& translation);
	FORCEINLINE void SetRotation(const Quaternion& rotation);
	FORCEINLINE void SetScale(const Vector3D& scale);

private:

	Vector3D m_Translation;
	Quaternion m_Rotation;
	Vector3D m_Scale;
};

FORCEINLINE Matrix Transform::ToMatrix() const
{
	return Matrix::TransformMatrix(m_Translation, m_Rotation, m_Scale);
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

FORCEINLINE Transform Transform::operator+=(const Transform& other)
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

FORCEINLINE Transform Transform::operator*=(const Transform& other)
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

FORCEINLINE Transform Transform::operator*=(float other)
{
	m_Translation *= other;
	m_Rotation *= other;
	m_Scale *= other;
	return *this;
}

FORCEINLINE Vector Transform::Transforms(const Vector3D& vector, float w) const
{
	return (m_Rotation.Rotate(m_Scale * vector) + m_Translation * w).ToVector(0.0f);
}

FORCEINLINE Vector Transform::Transforms(const Vector& vector) const
{
	return Transforms(Vector3D(vector), vector[3]);
}

FORCEINLINE Vector Transform::InverseTransform(const Vector3D& vector, float w) const
{
	return (m_Rotation.Inverse().Rotate(vector - m_Translation * w) * m_Scale.Reciprocal()).ToVector(0.0f);
}

FORCEINLINE Vector Transform::InverseTransform(const Vector& vector) const
{
	return InverseTransform(Vector3D(vector), vector[3]);
}


FORCEINLINE Vector3D Transform::GetTranslation() const
{
	return m_Translation;
}

FORCEINLINE Quaternion Transform::GetRotation() const
{
	return m_Rotation;
}

FORCEINLINE Vector3D Transform::GetScale() const
{
	return m_Scale;
}

FORCEINLINE void Transform::Set(const Vector3D& translationIn,
		const Quaternion& rotationIn, const Vector3D& scaleIn)
{
	m_Translation = translationIn;
	m_Rotation = rotationIn;
	m_Scale = scaleIn;
}

FORCEINLINE void Transform::SetTranslation(const Vector3D& val)
{
	m_Translation = val;
}

FORCEINLINE void Transform::SetRotation(const Quaternion& val)
{
	m_Rotation = val;
}

FORCEINLINE void Transform::SetScale(const Vector3D& val)
{
	m_Scale = val;
}

