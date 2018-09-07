#include "aabb.h"

AABB::AABB(Spatial3D* Points, uint32 Amount)
{
	if (Amount == 0) 
	{
		m_Extents[0] = Cartesian3D(0.f, 0.f, 0.f);
		m_Extents[1] = Cartesian3D(0.f, 0.f, 0.f);
		return;
	}

	m_Extents[0] = Points[0];
	m_Extents[1] = Points[0];
	for (uint32 Index = 1; Index < Amount; Index++) 
	{
		m_Extents[0] = m_Extents[0].Inner().Min(Points[Index].Inner());
		m_Extents[1] = m_Extents[1].Inner().Max(Points[Index].Inner());
	}
}

AABB::AABB(float* Points, uint32 Amount, uint32 Stride)
{
	if (Amount == 0) 
	{
		m_Extents[0] = Cartesian3D(0.f, 0.f, 0.f);
		m_Extents[1] = Cartesian3D(0.f, 0.f, 0.f);
		return;
	}

	Spatial3D _InitialPoint = Cartesian3D(Points[0],Points[1],Points[2]);
	m_Extents[0] = _InitialPoint;
	m_Extents[1] = _InitialPoint;
	uintptr index = 3;
	Stride += 3;

	for (uint32 Index = 1; Index < Amount; Index++) 
	{
		Spatial3D point = Cartesian3D(Points[index],Points[index+1],Points[index+2]);
		m_Extents[0] = m_Extents[0].Inner().Min(point.Inner());
		m_Extents[1] = m_Extents[1].Inner().Max(point.Inner());
		index += Stride;
	}
}

AABB AABB::Transform(const Matrix& InTransform) const
{
	Vector Center(GetCenter().AsIntrinsic(1.f));
	Vector Extents(GetExtents().AsIntrinsic(0.f));
	Vector absExtents = Extents.Abs();
	Matrix AbsMatrix(InTransform);
	
	for(uint32 Index = 0; Index < 4; Index++) 
	{
		AbsMatrix[Index] = AbsMatrix[Index].Abs();
	}

	Vector newCenter = InTransform.Transform(Center);
	Vector newExtents = AbsMatrix.Transform(absExtents);
	return AABB(newCenter - newExtents, newCenter + newExtents);
}

bool AABB::IntersectRay(const Spatial3D& start, const Spatial3D& rayDir, float& point1, float& point2) const
{
	Vector startVec = start.AsIntrinsic();
	Vector dirVec = rayDir.AsIntrinsic();
	Vector minVec = m_Extents[0].AsIntrinsic();
	Vector maxVec = m_Extents[1].AsIntrinsic();
	Vector rdirVec = dirVec.Reciprocal();
	Vector intersects1 = (minVec-startVec)*rdirVec;
	Vector intersects2 = (maxVec-startVec)*rdirVec;

	float mins[4];
	float maxs[4];
	intersects1.Min(intersects2).Store4f(mins);
	intersects1.Max(intersects2).Store4f(maxs);

	if((mins[0] > maxs[1]) || (mins[1] > maxs[0])) {
		return false;
	}
	if(mins[1] > mins[0]) {
		mins[0] = mins[1];
	}
	if(maxs[1] < maxs[0]) {
		maxs[0] = maxs[1];
	}
	if((mins[0] > maxs[2]) || (mins[2] > maxs[0])) {
		return false;
	}
	if(mins[2] > mins[0]) {
		mins[0] = mins[2];
	}
	if(maxs[2] < maxs[0]) {
		maxs[0] = maxs[2];
	}
	point1 = mins[0];
	point2 = maxs[0];
	return true;
}

bool AABB::IntersectLine(const Spatial3D& start, const Spatial3D& end) const
{
	float p1, p2;
	Spatial3D dir = (end-start);
	bool intersect = IntersectRay(start, dir.Normalized(), p1, p2);
	return intersect && p1*p1 < dir.LengthSquared();
} 
