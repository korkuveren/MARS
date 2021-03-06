#pragma once

#include "Cartesian.h"
class Matrix;

class Plane
{
public:
	FORCEINLINE Plane();
	FORCEINLINE Plane(const Vector& plane);
	FORCEINLINE Plane(float x, float y, float z, float w);
	FORCEINLINE Plane(const Spatial3D& normal, float w);
	Plane transform(const Matrix& transform) const;

	FORCEINLINE float dot(const Spatial3D& other) const;
	FORCEINLINE float dotVector(const Vector& other) const;
	FORCEINLINE float dotPlane(const Plane& other) const;
	FORCEINLINE Plane normalized(float errorMargin=1.e-8f) const;
	FORCEINLINE bool isNormalized(float errorMargin=1.e-4f) const;

	FORCEINLINE Spatial3D getNormal() const;
	FORCEINLINE Spatial3D reflect(const Spatial3D& point) const;

	bool intersectPlanes(Spatial3D& intersectionPoint,
			const Plane& other1, const Plane& other2,
			float errormargin=1.e-8f) const;
	FORCEINLINE float intersectLine(const Spatial3D& lineStart, const Spatial3D& lineEnd) const;
	FORCEINLINE float intersectRay(const Spatial3D& start, const Spatial3D& rayDir) const;

	FORCEINLINE Plane abs() const;

	FORCEINLINE Plane operator-() const;
	FORCEINLINE Plane operator+(const Plane& other) const;
	FORCEINLINE Plane operator-(const Plane& other) const;
	FORCEINLINE Plane operator*(const Plane& other) const;
	FORCEINLINE Plane operator/(const Plane& other) const;

	FORCEINLINE Plane operator+=(const Plane& other);
	FORCEINLINE Plane operator-=(const Plane& other);
	FORCEINLINE Plane operator*=(const Plane& other);
	FORCEINLINE Plane operator/=(const Plane& other);

	FORCEINLINE Plane operator*(float amt) const;
	FORCEINLINE Plane operator/(float amt) const;
	FORCEINLINE Plane operator*=(float val);
	FORCEINLINE Plane operator/=(float val);

	FORCEINLINE bool operator==(const Plane& other) const;
	FORCEINLINE bool operator!=(const Plane& other) const;
	FORCEINLINE bool equals(const Plane& other, float errorMargin=1.e-4f) const;
	FORCEINLINE Vector toVector() const;
private:
	Vector data;
};

FORCEINLINE Plane::Plane() {}

FORCEINLINE Plane::Plane(const Vector& plane) :
	data(plane) {}

FORCEINLINE Plane::Plane(float x, float y, float z, float w) :
	data(Vector::Make(x, y, z, w)) {}

FORCEINLINE Plane::Plane(const Spatial3D& normal, float w) :
	data(normal.AsIntrinsic(w)) {}

FORCEINLINE float Plane::dot(const Spatial3D& other) const
{
	return other.AsIntrinsic(1.0f).Dot4(data)[0];
}

FORCEINLINE float Plane::dotVector(const Vector& other) const
{
	return other.Dot4(data)[0];
}

FORCEINLINE float Plane::dotPlane(const Plane& other) const
{
	return other.data.Dot4(data)[0];
}

FORCEINLINE Plane Plane::normalized(float errorMargin) const
{
	(void)errorMargin;
	return Plane(data.Normalize3());
}

FORCEINLINE bool Plane::isNormalized(float errorMargin) const
{
	return Math::Abs(1.0f - (data.Dot3(data)[0])) < errorMargin;
}

FORCEINLINE Spatial3D Plane::getNormal() const
{
	return Spatial3D(data.Normalize3());
}

FORCEINLINE float Plane::intersectLine(const Spatial3D& lineStart, const Spatial3D& lineEnd) const
{
	return intersectRay(lineStart, lineEnd-lineStart);
}

FORCEINLINE float Plane::intersectRay(const Spatial3D& start, const Spatial3D& rayDir) const
{
	return -dot(start)/rayDir.Dot(getNormal());
}

FORCEINLINE Spatial3D Plane::reflect(const Spatial3D& point) const
{
	Vector pointAsVector(point.AsIntrinsic(1.0f));
	Vector dotAmt = VectorConstants::TWO * data.Dot4(pointAsVector);
	return Spatial3D(pointAsVector - (data * dotAmt));
}

FORCEINLINE Plane Plane::abs() const
{
	return Plane(data.Abs());
}

FORCEINLINE Plane Plane::operator-() const
{
	return Plane(-data);
}

FORCEINLINE Plane Plane::operator+(const Plane& other) const
{
	return Plane(data + other.data);
}

FORCEINLINE Plane Plane::operator-(const Plane& other) const
{
	return Plane(data - other.data);
}

FORCEINLINE Plane Plane::operator*(const Plane& other) const
{
	return Plane(data * other.data);
}

FORCEINLINE Plane Plane::operator/(const Plane& other) const
{
	return Plane(data / other.data);
}


FORCEINLINE Plane Plane::operator+=(const Plane& other)
{
	data = data + other.data;
	return *this;
}

FORCEINLINE Plane Plane::operator-=(const Plane& other)
{
	data = data - other.data;
	return *this;
}

FORCEINLINE Plane Plane::operator*=(const Plane& other)
{
	data = data * other.data;
	return *this;
}

FORCEINLINE Plane Plane::operator/=(const Plane& other)
{
	data = data / other.data;
	return *this;
}


FORCEINLINE Plane Plane::operator*(float amt) const
{
	return Plane(data * Vector::Load1f(amt));
}

FORCEINLINE Plane Plane::operator/(float amt) const
{
	return Plane(data * Vector::Load1f(Math::Reciprocal(amt)));
}

FORCEINLINE Plane Plane::operator*=(float val)
{
	data = data * Vector::Load1f(val);
	return *this;
}

FORCEINLINE Plane Plane::operator/=(float val)
{
	data = data * Vector::Load1f(Math::Reciprocal(val));
	return *this;
}


FORCEINLINE bool Plane::operator==(const Plane& other) const
{
	return (data != other.data).IsZero4f();
}

FORCEINLINE bool Plane::operator!=(const Plane& other) const
{
	return (data == other.data).IsZero4f();
}

FORCEINLINE bool Plane::equals(const Plane& other, float errorMargin) const
{
	return data.NotEquals(other.data, errorMargin).IsZero4f();
}

FORCEINLINE Vector Plane::toVector() const
{
	return data;
}
