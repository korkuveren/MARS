#pragma once

#include "Vector.h"
#include "Matrix.h"

class AABB
{

public:

	FORCEINLINE AABB() {}
	FORCEINLINE AABB(const Vector3D& MinExtents, const Vector3D& MaxExtents);
	AABB(Vector3D* Points, uint32 Amount);
	AABB(float* Points, uint32 Amount, uint32 Stride = 0);

	bool IntersectRay(const Vector3D& Start, const Vector3D& rayDir, float& Point1, float& Point2) const;
	bool IntersectLine(const Vector3D& Start, const Vector3D& end) const;
	FORCEINLINE bool Intersects(const AABB& Other) const;
	AABB Transform(const Matrix& transform) const;

	FORCEINLINE AABB Eexpand(float Distance) const;
	FORCEINLINE AABB Eexpand(const Vector3D& Amount) const;
	FORCEINLINE AABB MoveTo(const Vector3D& Destination) const;
	FORCEINLINE Vector3D GetCenter() const;
	FORCEINLINE Vector3D GetExtents() const;
	FORCEINLINE Vector3D GetMinExtents() const;
	FORCEINLINE Vector3D GetMaxExtents() const;
	FORCEINLINE void GetCenterAndExtents(Vector3D& Center, Vector3D& Extents) const;
	FORCEINLINE float GetVolume() const;
	FORCEINLINE AABB Overlap(const AABB& Other) const;
	FORCEINLINE bool Contains(const Vector3D& point) const;
	FORCEINLINE bool Contains(const AABB& Other) const;

	FORCEINLINE AABB Translate(const Vector3D& Amount) const;
	FORCEINLINE AABB ScaleFromCenter(const Vector3D& Amount) const;
	FORCEINLINE AABB ScaleFromOrigin(const Vector3D& Amount) const;

	FORCEINLINE AABB AddPoint(const Vector3D& Other) const;
	FORCEINLINE AABB AddAABB(const AABB& Other) const;

	FORCEINLINE bool operator==(const AABB& Other) const;
	FORCEINLINE bool operator!=(const AABB& Other) const;
	FORCEINLINE bool Equals(const AABB& Other, float ErrorMargin=1.e-4f) const;

private:

	Vector3D m_Extents[2];
};

FORCEINLINE AABB::AABB(const Vector3D& MinExtents, const Vector3D& MaxExtents)
{
	m_Extents[0] = MinExtents;
	m_Extents[1] = MaxExtents;
}

FORCEINLINE bool AABB::Intersects(const AABB& Other) const
{
	return ((m_Extents[0].ToVector() >= Other.m_Extents[1].ToVector()) | (m_Extents[1].ToVector() <= Other.m_Extents[0].ToVector())).IsZero3f();
}

FORCEINLINE AABB AABB::Eexpand(float Distance) const
{
	return Eexpand(Vector3D(Distance));
}

FORCEINLINE AABB AABB::Eexpand(const Vector3D& Amount) const
{
	return AABB(m_Extents[0] - Amount, m_Extents[1] + Amount);
}

FORCEINLINE AABB AABB::MoveTo(const Vector3D& Destination) const
{
	return Translate(Destination - GetCenter());
}

FORCEINLINE Vector3D AABB::GetCenter() const
{
	return (m_Extents[1]+m_Extents[0]) * 0.5f;
}

FORCEINLINE Vector3D AABB::GetExtents() const
{
	return (m_Extents[1]-m_Extents[0]) * 0.5f;
}

FORCEINLINE Vector3D AABB::GetMinExtents() const
{
	return m_Extents[0];
}

FORCEINLINE Vector3D AABB::GetMaxExtents() const
{
	return m_Extents[1];
}

FORCEINLINE void AABB::GetCenterAndExtents(Vector3D& Center, Vector3D& Extents) const
{
	Extents = (this->m_Extents[1]-this->m_Extents[0]) * 0.5f;
	Center = this->m_Extents[0] + Extents;
}

FORCEINLINE float AABB::GetVolume() const
{
	Vector3D _Lengths = m_Extents[1] - m_Extents[0];
	return _Lengths[0] * _Lengths[1] * _Lengths[2];
}

FORCEINLINE AABB AABB::Overlap(const AABB& Other) const
{
	return AABB(m_Extents[0].Max(Other.m_Extents[0]), m_Extents[1].Min(Other.m_Extents[1]));
}

FORCEINLINE bool AABB::Contains(const Vector3D& point) const
{
	return ((point.ToVector() <= m_Extents[0].ToVector()) | (point.ToVector() >= m_Extents[1].ToVector())).IsZero3f();
}

FORCEINLINE bool AABB::Contains(const AABB& Other) const
{
	return((Other.m_Extents[0].ToVector() <= m_Extents[0].ToVector()) |
	 	  (Other.m_Extents[0].ToVector() >= m_Extents[1].ToVector())  |
		  (Other.m_Extents[1].ToVector() <= m_Extents[0].ToVector())  |
	 	  (Other.m_Extents[1].ToVector() >= m_Extents[1].ToVector())).IsZero3f();
}

FORCEINLINE AABB AABB::AddPoint(const Vector3D& Other) const
{
	return AABB(m_Extents[0].Min(Other), m_Extents[1].Max(Other));
}

FORCEINLINE AABB AABB::AddAABB(const AABB& Other) const
{
	return AABB(m_Extents[0].Min(Other.m_Extents[0]), m_Extents[1].Max(Other.m_Extents[1]));
}

FORCEINLINE AABB AABB::Translate(const Vector3D& Amount) const
{
	return AABB(m_Extents[0] + Amount, m_Extents[1] + Amount);
}

FORCEINLINE AABB AABB::ScaleFromOrigin(const Vector3D& Amount) const
{
	return AABB(m_Extents[0] * Amount, m_Extents[1] * Amount);
}

FORCEINLINE AABB AABB::ScaleFromCenter(const Vector3D& Amount) const
{
	Vector3D Extents, Center;
	GetCenterAndExtents(Center, Extents);
	Extents = Extents * Amount;
	return AABB(Center - Extents, Center + Extents);
}

FORCEINLINE bool AABB::operator==(const AABB& Other) const
{
	return (m_Extents[0] == Other.m_Extents[0]) && (m_Extents[1] == Other.m_Extents[1]);
}

FORCEINLINE bool AABB::operator!=(const AABB& Other) const
{
	return (m_Extents[0] != Other.m_Extents[0]) && (m_Extents[1] != Other.m_Extents[1]);
}

FORCEINLINE bool AABB::Equals(const AABB& Other, float ErrorMargin) const
{
	return m_Extents[0].Equals(Other.m_Extents[0], ErrorMargin) && m_Extents[1].Equals(Other.m_Extents[1], ErrorMargin);
}

