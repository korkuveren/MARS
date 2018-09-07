#pragma once

#include "Cartesian.h"
#include "Matrix.h"

class Sphere
{
public:
	FORCEINLINE Sphere() {}
	FORCEINLINE Sphere(const Spatial3D& center, float radius) :
		data(center.AsIntrinsic(radius)) {}
	Sphere(Spatial3D* points, uint32 amt);
	Sphere(float* points, uint32 amt);

	bool intersectRay(const Spatial3D& start, const Spatial3D& rayDir, float& point1, float& point2) const;
	bool intersectLine(const Spatial3D& start, const Spatial3D& end) const;
	FORCEINLINE bool intersects(const Sphere& other, float errorMargin=1.e-4f) const;
	Sphere transform(const Matrix& transform) const;
	FORCEINLINE Sphere expand(float distance) const;
	FORCEINLINE Sphere moveTo(const Spatial3D& destination) const;
	FORCEINLINE float getVolume() const;
	FORCEINLINE float getRadius() const;
	FORCEINLINE Spatial3D getCenter() const;
	FORCEINLINE bool contains(const Spatial3D& point, float errorMargin=1.e-4f) const;
	FORCEINLINE bool contains(const Sphere& other, float errorMargin=1.e-4f) const;
	FORCEINLINE Sphere translate(const Spatial3D& amt) const;
	FORCEINLINE Sphere scaleFromCenter(float amt) const;
	FORCEINLINE Sphere scaleFromOrigin(float amt) const;
	FORCEINLINE Sphere addPoint(const Spatial3D& other) const;
	Sphere addSphere(const Sphere& other) const;
	FORCEINLINE Vector toVector() const;

	FORCEINLINE bool operator==(const Sphere& other) const;
	FORCEINLINE bool operator!=(const Sphere& other) const;
	FORCEINLINE bool equals(const Sphere& other, float errorMargin=1.e-4f) const;
private:
	Vector data;

	FORCEINLINE Sphere(const Vector& newData) :
		data(newData) {}
};

FORCEINLINE bool Sphere::intersects(const Sphere& other, float errorMargin) const
{
	Vector dist = data - other.data;
	float maxR = Math::Max(0.0f, data[3] + other.data[3] + errorMargin);
	return dist.Dot3(dist)[0] < maxR*maxR;
}

FORCEINLINE Sphere Sphere::expand(float distance) const
{
	return Sphere(data + Vector::Make(0.0f,0.0f,0.0f,distance));
}

FORCEINLINE Sphere Sphere::moveTo(const Spatial3D& destination) const
{
	return Sphere(destination, data[3]);
}

FORCEINLINE float Sphere::getVolume() const
{
	float radius = data[3];
	return MATH_PI*4.0f/3.0f * radius*radius*radius;
}

FORCEINLINE float Sphere::getRadius() const
{
	return data[3];
}

FORCEINLINE Spatial3D Sphere::getCenter() const
{
	return Spatial3D(data);
}

FORCEINLINE bool Sphere::contains(const Spatial3D& point, float errorMargin) const
{
	float r = data[3]+errorMargin;
	return point.DistSquared(Spatial3D(data)) < r*r;
}

FORCEINLINE bool Sphere::contains(const Sphere& other, float errorMargin) const
{
	// If other sphere is bigger than this, must return false.
	// Otherwise, false positives since the rest of the equation is direction
	// independent
	if(data[3] < other.data[3]+errorMargin) {
		return false;
	}
	Vector dataDist = data-other.data;
	float r = errorMargin+dataDist[3];
	float dSq = dataDist.Dot3(dataDist)[0];
	return dSq < r*r;
}

FORCEINLINE Sphere Sphere::translate(const Spatial3D& amt) const
{
	return Sphere(data + amt.AsIntrinsic(0.0f));
}

FORCEINLINE Sphere Sphere::scaleFromCenter(float amt) const
{
	return Sphere(data * Vector::Make(1.0f,1.0f,1.0f,amt));
}

FORCEINLINE Sphere Sphere::scaleFromOrigin(float amt) const
{
	return Sphere(data * Vector::Make(amt,amt,amt,amt));
}

FORCEINLINE Sphere Sphere::addPoint(const Spatial3D& other) const
{
	return addSphere(Sphere(other, 0.0f));
}

FORCEINLINE bool Sphere::operator==(const Sphere& other) const
{
	return (data != other.data).IsZero4f();
}

FORCEINLINE bool Sphere::operator!=(const Sphere& other) const
{
	return (data == other.data).IsZero4f();
}

FORCEINLINE bool Sphere::equals(const Sphere& other, float errorMargin) const
{
	return data.NotEquals(other.data, errorMargin).IsZero4f();
}

FORCEINLINE Vector Sphere::toVector() const
{
	return data;
}

