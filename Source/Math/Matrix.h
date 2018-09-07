#pragma once

#include "VecMath.h"
#include "Cartesian.h"
#include "Quaternion.h"
#include "Plane.h"

class Matrix
{
public:
	FORCEINLINE Matrix();
	FORCEINLINE Matrix(const Vector& vecX, const Vector& vecY,
			const Vector& vecZ, const Vector& vecOffset);

	static FORCEINLINE Matrix Identity();
	static FORCEINLINE Matrix Translate(const Cartesian3D& amt);
	static FORCEINLINE Matrix Scale(const Cartesian3D& amt);
	static FORCEINLINE Matrix Scale(float amt);
	static FORCEINLINE Matrix Ortho(float left, float right,
			float bottom, float top, float near, float far);
	static FORCEINLINE Matrix Perspective(float halfFov, float aspect,
			float nearZ, float farZ);
	static FORCEINLINE Matrix TransformMatrix(const Cartesian3D& translation,
			const Quaternion& rotation, const Cartesian3D& scale);

	void ExtractFrustumPlanes(Plane* planes) const;
	Matrix ToNormalMatrix() const;
	
	FORCEINLINE Matrix operator+ (const Matrix& other) const;
	FORCEINLINE Matrix& operator+= (const Matrix& other);
	FORCEINLINE Matrix operator* (const Matrix& other) const;
	FORCEINLINE Matrix& operator*= (const Matrix& other);
	FORCEINLINE Matrix operator* (float amt) const;
	FORCEINLINE Matrix& operator*= (float amt);
	FORCEINLINE bool operator==(const Matrix& other) const;
	FORCEINLINE bool operator!=(const Matrix& other) const;

	FORCEINLINE bool Equals(const Matrix& other, float errorMargin=1.e-4f) const;
	FORCEINLINE Vector Transform(const Vector& vector) const;

	FORCEINLINE Matrix Transpose() const;
	FORCEINLINE float Determinant4x4() const;
	FORCEINLINE float Determinant3x3() const;
	FORCEINLINE Matrix Inverse() const;

	FORCEINLINE Matrix ApplyScale(const Vector& scale);
	FORCEINLINE Vector RemoveScale(/*float ErrorMargin=1.e-8f*/); // @TODO(Devlinw): make use of the ErrorMargin
	FORCEINLINE Vector GetScale() const;
	Quaternion GetRotation() const;
	FORCEINLINE Vector GetTranslation() const;

	FORCEINLINE Vector operator[](uint32 index) const {
		assertCheck(index < 4);
		return m[index];
	}
private:
	Vector m[4];
};

FORCEINLINE Matrix Matrix::Identity()
{
	return Matrix(
			Vector::Make(1.0f, 0.0f, 0.0f, 0.0f),
			Vector::Make(0.0f, 1.0f, 0.0f, 0.0f),
			Vector::Make(0.0f, 0.0f, 1.0f, 0.0f),
			Vector::Make(0.0f, 0.0f, 0.0f, 1.0f));
}

FORCEINLINE Matrix Matrix::Translate(const Cartesian3D& amt)
{
	return Matrix(
		Vector::Make(1.0f, 0.0f, 0.0f, amt[0]),
		Vector::Make(0.0f, 1.0f, 0.0f, amt[1]),
		Vector::Make(0.0f, 0.0f, 1.0f, amt[2]),
		Vector::Make(0.0f, 0.0f, 0.0f, 1.0f));
}

FORCEINLINE Matrix Matrix::Scale(const Cartesian3D& amt)
{
	return Matrix(
			Vector::Make(amt[0], 0.0f, 0.0f, 0.0f),
			Vector::Make(0.0f, amt[1], 0.0f, 0.0f),
			Vector::Make(0.0f, 0.0f, amt[2], 0.0f),
			Vector::Make(0.0f, 0.0f, 0.0f, 1.0f));
}

FORCEINLINE Matrix Matrix::Scale(float amt)
{
	return Scale(Cartesian3D(amt));
}

FORCEINLINE Matrix Matrix::Ortho(float left, float right,
			float bottom, float top, float near, float far)
{
	float rwidth = Math::Reciprocal(right - left);
	float rheight = Math::Reciprocal(top - bottom);
	float rdepth = Math::Reciprocal(far - near);

	return Matrix(
			Vector::Make(2.0f * rwidth, 0.0f, 0.0f, -(right + left) * rwidth),
			Vector::Make(0.0f, 2.0f * rheight, 0.0f, -(top + bottom) * rheight),
			Vector::Make(0.0f, 0.0f, 2.0f * rdepth, -(far + near) * rdepth),
			Vector::Make(0.0f, 0.0f, 0.0f, 1.0f));
}

FORCEINLINE Matrix Matrix::Perspective(float halfFov, float aspect,
			float nearZ, float farZ)
{
	float rtanHalfFov = Math::Reciprocal(Math::Tan(halfFov));
	float scaleZ = nearZ == farZ ? 1.0f : farZ / (farZ - nearZ);
	float offsetZ = -nearZ * scaleZ;

	return Matrix(
			Vector::Make(rtanHalfFov, 0.0f, 0.0f, 0.0f),
			Vector::Make(0.0f, aspect * rtanHalfFov, 0.0f, 0.0f),
			Vector::Make(0.0f, 0.0f, scaleZ, offsetZ),
			Vector::Make(0.0f, 0.0f, 1.0f, 0.0f));
}

FORCEINLINE Matrix Matrix::TransformMatrix(const Cartesian3D& translation,
			const Quaternion& rotation, const Cartesian3D& scale)
{
	Matrix result;
	Vector::CreateTransformMatrix(&result, translation.AsIntrinsic(), rotation.AsIntrinsic(), scale.AsIntrinsic());
	return result;
}


FORCEINLINE Matrix::Matrix() {}

FORCEINLINE Matrix::Matrix(const Vector& vecX, const Vector& vecY,
			const Vector& vecZ, const Vector& vecOffset)
{
	m[0] = vecX;
	m[1] = vecY;
	m[2] = vecZ;
	m[3] = vecOffset;
}
	
FORCEINLINE Matrix Matrix::operator+ (const Matrix& other) const
{
	Matrix result;
	for(uint32 i = 0; i < 4; i++) {
		result.m[i] = m[i] + other.m[i];
	}
	return result;
}

FORCEINLINE Matrix& Matrix::operator+= (const Matrix& other)
{
	for(uint32 i = 0; i < 4; i++) {
		m[i] = m[i] + other.m[i];
	}
	return *this;
}

FORCEINLINE Matrix Matrix::operator* (const Matrix& other) const
{
	Matrix result;
	Vector::matrixMul(&result, this, &other);
	return result;
}

FORCEINLINE Matrix& Matrix::operator*= (const Matrix& other)
{
	Vector::matrixMul(this, this, &other);
	return *this;
}

FORCEINLINE Matrix Matrix::operator* (float amt) const
{
	Matrix result;
	Vector vecAmt = Vector::Load1f(amt);
	for(uint32 i = 0; i < 4; i++) {
		result.m[i] = m[i] * vecAmt;
	}
	return result;
}

FORCEINLINE Matrix& Matrix::operator*= (float amt)
{
	Vector vecAmt = Vector::Load1f(amt);
	for(uint32 i = 0; i < 4; i++) {
		m[i] = m[i] * vecAmt;
	}
	return *this;
}

FORCEINLINE bool Matrix::operator==(const Matrix& other) const
{
	for(uint32 i = 0; i < 4; i++) {
		if(!(m[i] != other.m[i]).IsZero4f()) {
			return false;
		}
	}
	return true;
}

FORCEINLINE bool Matrix::operator!=(const Matrix& other) const
{
	return !(*this == other);
}


FORCEINLINE bool Matrix::Equals(const Matrix& other, float errorMargin) const
{
	for(uint32 i = 0; i < 4; i++) {
		if(!(m[i].NotEquals(other.m[i], errorMargin)).IsZero4f()) {
			return false;
		}
	}
	return true;

}

FORCEINLINE Vector Matrix::Transform(const Vector& vector) const
{
	return vector.Transform(this);
}


FORCEINLINE Matrix Matrix::Transpose() const
{
	Matrix result;
	float m0[4];
	float m1[4];
	float m2[4];
	float m3[4];
	m[0].Store4f(m0);
	m[1].Store4f(m1);
	m[2].Store4f(m2);
	m[3].Store4f(m3);

	for(uint32 i = 0; i < 4; i++) {
		result.m[i] = Vector::Make(m0[i], m1[i], m2[i], m3[i]);
	}

	return result;
}

FORCEINLINE float Matrix::Determinant4x4() const
{
	return Vector::MatrixDeterminant4x4(nullptr, nullptr, m);
}

FORCEINLINE float Matrix::Determinant3x3() const
{
	return Vector::MatrixDeterminant3x3Vector(m);
}

FORCEINLINE Matrix Matrix::Inverse() const
{
	Matrix result;
	Vector::MatrixInverse(&result, this);
	return result;
}


FORCEINLINE Vector Matrix::GetScale() const
{
	Vector invScale = VectorConstants::ZERO;
	for(uint32 i = 0; i < 4; i++) {
		invScale = invScale + m[i]*m[i];
	}
	invScale = invScale.rSqrt().Select(VectorConstants::MASK_W,VectorConstants::ONE);
	return invScale.Reciprocal();
}

FORCEINLINE Vector Matrix::RemoveScale(/*float ErrorMargin*/)
{
	Vector invScale = VectorConstants::ZERO;
	for (auto& i : m) 
	{
		invScale = invScale + i*i;
	}
	invScale = invScale.rSqrt().Select(VectorConstants::MASK_W, VectorConstants::ONE);
	for (auto& i : m) 
	{
		i = i * invScale;
	}
	return invScale.Reciprocal();
}

FORCEINLINE Vector Matrix::GetTranslation() const
{
	return Vector::Make(m[0][3], m[1][3], m[2][3], m[3][3]);
}

FORCEINLINE Matrix Matrix::ApplyScale(const Vector& scale)
{
	for(uint32 i = 0; i < 4; i++) {
		m[i] = m[i] * scale;//Vector::make(scale[i], scale[i], scale[i], 0.0f);
	}
	return *this;
}
