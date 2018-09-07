#include "Sphere.h"

// According to the data in Thomas Larsson's Fast and Tight Fitting Bounding
// Spheres, doing naive algorithm with best of AABB and average centers tends
// to achieve very good results on average (average was between 2% and 3%)
// from optimal bounding sphere in their dataset.
// This was better than any of the "fast" algorithms combined, including
// Ritter's algorithm, though it did worse than the proposed algorithm which
// would require more implementation for about the same speed
Sphere::Sphere(Spatial3D* points, uint32 amt)
{
	if(amt == 0) {
		data = VectorConstants::ZERO;
		return;
	}
	Spatial3D extents[2];
	extents[0] = points[0];
	extents[1] = points[0];
	Spatial3D meanPoint = VectorConstants::ZERO;
	for(uint32 i = 1; i < amt; i++) {
		Spatial3D point = points[i];
		extents[0] = extents[0].Inner().Min(point.Inner());
		extents[1] = extents[1].Inner().Max(point.Inner());
		meanPoint += point;
	}
	Spatial3D center1 = (extents[0]+extents[1])/2.0f;
	Spatial3D center2 = meanPoint * Math::Reciprocal(((float)amt));
	float radius21 = 0.0f;
	float radius22 = 0.0f;
	for(uint32 i = 0; i < amt; i++) {
		radius21 = Math::Max(radius21, (center1-points[i]).LengthSquared());
		radius22 = Math::Max(radius22, (center2-points[i]).LengthSquared());
	}
	if(radius22 < radius21) {
		data = center2.AsIntrinsic(Math::Sqrt(radius22));
	} else {
		data = center1.AsIntrinsic(Math::Sqrt(radius21));
	}
}

Sphere::Sphere(float* points, uint32 amt)
{
	if(amt == 0) {
		data = VectorConstants::ZERO;
		return;
	}
	Spatial3D extents[2];
	Spatial3D point0 = Cartesian3D(points[0],points[1],points[2]);
	extents[0] = point0;
	extents[1] = point0;
	Spatial3D meanPoint = VectorConstants::ZERO;
	uintptr index = 3;
	for(uint32 i = 1; i < amt; i++) {
		Spatial3D point = Cartesian3D(points[index],points[index+1],points[index+2]);
		extents[0] = extents[0].Inner().Min(point.Inner());
		extents[1] = extents[1].Inner().Max(point.Inner());
		meanPoint += point;
		index += 3;
	}
	Spatial3D center1 = (extents[0]+extents[1])/2.0f;
	Spatial3D center2 = meanPoint * Math::Reciprocal(((float)amt));
	float radius21 = 0.0f;
	float radius22 = 0.0f;
	index = 0;
	for(uint32 i = 0; i < amt; i++) {
		Spatial3D point = Cartesian3D(points[index],points[index+1],points[index+2]);
		radius21 = Math::Max(radius21, (center1-point).LengthSquared());
		radius22 = Math::Max(radius22, (center2-point).LengthSquared());
		index += 3;
	}
	if(radius22 < radius21) {
		data = center2.AsIntrinsic(Math::Sqrt(radius22));
	} else {
		data = center1.AsIntrinsic(Math::Sqrt(radius21));
	}

}

Sphere Sphere::transform(const Matrix& transform) const
{
	Vector center = transform.Transform(data.Select(VectorConstants::MASK_W, VectorConstants::ONE));
	
	float radius = data[3]*Math::Sqrt(Math::Max3(
			transform[0].Dot3(transform[0])[0],
			transform[1].Dot3(transform[1])[0],
			transform[2].Dot3(transform[2])[0]));

	return Sphere(Spatial3D(center),radius);
}

Sphere Sphere::addSphere(const Sphere& other) const
{
	if(contains(other)) {
		return Sphere(*this);
	} else if(data[3] == 0.0f || other.contains(*this)) {
		return Sphere(other);
	} else {
		Spatial3D dir = Spatial3D(other.data - data);
		float dist = dir.Length();
		dir /= dist;
		Spatial3D extreme1 = getCenter() - dir*data[3];
		Spatial3D extreme2 = other.getCenter() + dir*other.data[3];
		return Sphere(
				(extreme1+extreme2)*0.5f,
				(dist + data[3]+other.data[3])*0.5f);
	}
}

bool Sphere::intersectRay(const Spatial3D& start, const Spatial3D& rayDir, float& point1, float& point2) const
{
	Spatial3D rayToCenter(getCenter() - start);
	float distToCenterAlongRay = rayToCenter.Dot(rayDir);
	float sphereCenterDistToRaySq = rayToCenter.LengthSquared() - 
		distToCenterAlongRay*distToCenterAlongRay;
	float radius2 = data[3]*data[3];
	if(sphereCenterDistToRaySq > radius2) {
		return false;
	}
	float halfRayPenetrationDist = Math::Sqrt(radius2-sphereCenterDistToRaySq);
	point1 = distToCenterAlongRay - halfRayPenetrationDist;
	point2 = distToCenterAlongRay + halfRayPenetrationDist;
	return true;
}

bool Sphere::intersectLine(const Spatial3D& start, const Spatial3D& end) const
{
	float p1, p2;
	Spatial3D dir = (end-start);
	bool intersect = intersectRay(start, dir.Normalized(), p1, p2);
	return intersect && p1*p1 < dir.LengthSquared();
}
