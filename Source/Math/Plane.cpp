#include "Plane.h"
#include "Matrix.h"

Plane Plane::transform(const Matrix& transform) const
{
	return Plane(transform.ToNormalMatrix().Transform(data)).normalized();
}

bool Plane::intersectPlanes(Spatial3D& intersectionPoint, const Plane& other1, const Plane& other2, float errorMargin) const
{
	Vector cross01 = data.Cross3(other1.data);
	float det = cross01.Dot3(other2.data)[0];
	float detSq = det*det;
	if(detSq < errorMargin) {
		return false;
	}
	Vector cross12 = other1.data.Cross3(other2.data);
	Vector cross20 = other2.data.Cross3(data);
	Vector w0 = data.Replicate(3);
	Vector w1 = other1.data.Replicate(3);
	Vector w2 = other2.data.Replicate(3);
	Vector invDet = Vector::Load1f(-Math::Reciprocal(det));

	intersectionPoint = Spatial3D(invDet*(w0*cross12+w1*cross20+w2*cross01));
	return true;
}


