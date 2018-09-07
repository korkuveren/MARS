#pragma once

#include "Cartesian.h"
#include "Matrix.h"

class AABB
{

public:

	FORCEINLINE AABB() {}
	FORCEINLINE AABB(const Spatial3D& MinExtents, const Spatial3D& MaxExtents);
	AABB(Spatial3D* Points, uint32 Amount);
	AABB(float* Points, uint32 Amount, uint32 Stride = 0);

	bool IntersectRay(const Spatial3D& Start, const Spatial3D& rayDir, float& Point1, float& Point2) const;
	bool IntersectLine(const Spatial3D& Start, const Spatial3D& end) const;
	FORCEINLINE bool Intersects(const AABB& Other) const;
	AABB Transform(const Matrix& transform) const;

	FORCEINLINE AABB Eexpand(float Distance) const;
	FORCEINLINE AABB Eexpand(const Spatial3D& Amount) const;
	FORCEINLINE AABB MoveTo(const Spatial3D& Destination) const;
	FORCEINLINE Spatial3D GetCenter() const;
	FORCEINLINE Spatial3D GetExtents() const;
	FORCEINLINE Spatial3D GetMinExtents() const;
	FORCEINLINE Spatial3D GetMaxExtents() const;
	FORCEINLINE void GetCenterAndExtents(Spatial3D& Center, Spatial3D& Extents) const;
	FORCEINLINE float GetVolume() const;
	FORCEINLINE AABB Overlap(const AABB& Other) const;
	FORCEINLINE bool Contains(const Spatial3D& point) const;
	FORCEINLINE bool Contains(const AABB& Other) const;

	FORCEINLINE AABB Translate(const Spatial3D& Amount) const;
	FORCEINLINE AABB ScaleFromCenter(const Spatial3D& Amount) const;
	FORCEINLINE AABB ScaleFromOrigin(const Spatial3D& Amount) const;

	FORCEINLINE AABB AddPoint(const Spatial3D& Other) const;
	FORCEINLINE AABB AddAABB(const AABB& Other) const;

	FORCEINLINE bool operator==(const AABB& Other) const;
	FORCEINLINE bool operator!=(const AABB& Other) const;
	FORCEINLINE bool Equals(const AABB& Other, float ErrorMargin=1.e-4f) const;

private:

	Spatial3D m_Extents[2];
};

FORCEINLINE AABB::AABB(const Spatial3D& MinExtents, const Spatial3D& MaxExtents)
{
	m_Extents[0] = MinExtents;
	m_Extents[1] = MaxExtents;
}

FORCEINLINE bool AABB::Intersects(const AABB& Other) const
{
	return ((m_Extents[0].AsIntrinsic() >= Other.m_Extents[1].AsIntrinsic()) | (m_Extents[1].AsIntrinsic() <= Other.m_Extents[0].AsIntrinsic())).IsZero3f();
}

FORCEINLINE AABB AABB::Eexpand(float Distance) const
{
	return Eexpand(Spatial3D(Distance));
}

FORCEINLINE AABB AABB::Eexpand(const Spatial3D& Amount) const
{
	return AABB(m_Extents[0] - Amount, m_Extents[1] + Amount);
}

FORCEINLINE AABB AABB::MoveTo(const Spatial3D& Destination) const
{
	return Translate(Destination - GetCenter());
}

FORCEINLINE Spatial3D AABB::GetCenter() const
{
	return (m_Extents[1]+m_Extents[0]) * 0.5f;
}

FORCEINLINE Spatial3D AABB::GetExtents() const
{
	return (m_Extents[1]-m_Extents[0]) * 0.5f;
}

FORCEINLINE Spatial3D AABB::GetMinExtents() const
{
	return m_Extents[0];
}

FORCEINLINE Spatial3D AABB::GetMaxExtents() const
{
	return m_Extents[1];
}

FORCEINLINE void AABB::GetCenterAndExtents(Spatial3D& Center, Spatial3D& Extents) const
{
	Extents = (this->m_Extents[1]-this->m_Extents[0]) * 0.5f;
	Center = this->m_Extents[0] + Extents;
}

FORCEINLINE float AABB::GetVolume() const
{
	Spatial3D _Lengths = m_Extents[1] - m_Extents[0];
	return _Lengths[0] * _Lengths[1] * _Lengths[2];
}

FORCEINLINE AABB AABB::Overlap(const AABB& Other) const
{
	return AABB(m_Extents[0].Inner().Max(Other.m_Extents[0].Inner()), m_Extents[1].Inner().Min(Other.m_Extents[1].Inner()));
}

FORCEINLINE bool AABB::Contains(const Spatial3D& point) const
{
	return ((point.AsIntrinsic() <= m_Extents[0].AsIntrinsic()) | (point.AsIntrinsic() >= m_Extents[1].AsIntrinsic())).IsZero3f();
}

FORCEINLINE bool AABB::Contains(const AABB& Other) const
{
	return((Other.m_Extents[0].AsIntrinsic() <= m_Extents[0].AsIntrinsic()) |
	 	  (Other.m_Extents[0].AsIntrinsic() >= m_Extents[1].AsIntrinsic())  |
		  (Other.m_Extents[1].AsIntrinsic() <= m_Extents[0].AsIntrinsic())  |
	 	  (Other.m_Extents[1].AsIntrinsic() >= m_Extents[1].AsIntrinsic())).IsZero3f();
}

FORCEINLINE AABB AABB::AddPoint(const Spatial3D& Other) const
{
	return AABB(m_Extents[0].Inner().Min(Other.Inner()), m_Extents[1].Inner().Max(Other.Inner()));
}

FORCEINLINE AABB AABB::AddAABB(const AABB& Other) const
{
	return AABB(m_Extents[0].Inner().Min(Other.m_Extents[0].Inner()), m_Extents[1].Inner().Max(Other.m_Extents[1].Inner()));
}

FORCEINLINE AABB AABB::Translate(const Spatial3D& Amount) const
{
	return AABB(m_Extents[0] + Amount, m_Extents[1] + Amount);
}

FORCEINLINE AABB AABB::ScaleFromOrigin(const Spatial3D& Amount) const
{
	return AABB(m_Extents[0] * Amount, m_Extents[1] * Amount);
}

FORCEINLINE AABB AABB::ScaleFromCenter(const Spatial3D& Amount) const
{
	Spatial3D Extents, Center;
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
	return m_Extents[0].Inner().Equals(Other.m_Extents[0].Inner(), ErrorMargin) && m_Extents[1].Inner().Equals(Other.m_Extents[1].Inner(), ErrorMargin);
}

