#pragma once

#include "EngineCore/EngineUtils.h"
#include "VecMath.h"

/*
*	Representation of some discrete quantity in a 2D Cartesian space.
*	EG:
*	- Position
*	- Direction
**/
class Cartesian2D
{
public:
	static const Cartesian2D Identity;
	static const Cartesian2D FrontDir;
	static const Cartesian2D UpDir;
	static const Cartesian2D One;

	FORCEINLINE Cartesian2D();
	FORCEINLINE Cartesian2D(float Val);
	FORCEINLINE Cartesian2D(float X, float Y);

	FORCEINLINE float Dot(const Cartesian2D& Other) const;
	FORCEINLINE Cartesian2D DotAsS2D(const Cartesian2D& Other) const;
	FORCEINLINE float Cross(const Cartesian2D& Other) const;
	FORCEINLINE float Distance(const Cartesian2D& Other) const;
	FORCEINLINE float DistanceSquared(const Cartesian2D& Other) const;

	FORCEINLINE Cartesian2D operator+(const Cartesian2D& Other) const;
	FORCEINLINE Cartesian2D operator-(const Cartesian2D& Other) const;
	FORCEINLINE Cartesian2D operator*(const Cartesian2D& Other) const;
	FORCEINLINE Cartesian2D operator/(const Cartesian2D& Other) const;

	FORCEINLINE Cartesian2D operator+(float Amount) const;
	FORCEINLINE Cartesian2D operator-(float Amount) const;
	FORCEINLINE Cartesian2D operator*(float Amount) const;
	FORCEINLINE Cartesian2D operator/(float Amount) const;

	FORCEINLINE bool operator==(const Cartesian2D& Other) const;
	FORCEINLINE bool operator!=(const Cartesian2D& Other) const;
	FORCEINLINE bool Equals(const Cartesian2D& Other, float ErrorMargin=KINDA_SMALL_NUMBER) const;
	FORCEINLINE bool Equals(float Val, float ErrorMargin=KINDA_SMALL_NUMBER) const;

	FORCEINLINE Cartesian2D operator-() const;
	FORCEINLINE Cartesian2D& operator+=(const Cartesian2D& Other);
	FORCEINLINE Cartesian2D& operator-=(const Cartesian2D& Other);
	FORCEINLINE Cartesian2D& operator*=(const Cartesian2D& Other);
	FORCEINLINE Cartesian2D& operator/=(const Cartesian2D& Other);

	FORCEINLINE Cartesian2D& operator+=(float Val);
	FORCEINLINE Cartesian2D& operator-=(float Val);
	FORCEINLINE Cartesian2D& operator*=(float Val);
	FORCEINLINE Cartesian2D& operator/=(float Val);

	FORCEINLINE float operator[](uint32 Index) const;

	FORCEINLINE float X() const { return m_Vector[0]; }
	FORCEINLINE float Y() const { return m_Vector[1]; }

	FORCEINLINE void Set(float X, float Y);
	FORCEINLINE void Set(uint32 Index, float Val);
	FORCEINLINE float Max() const;
	FORCEINLINE float Min() const;
	FORCEINLINE float AbsMax() const;
	FORCEINLINE float AbsMin() const;

	FORCEINLINE Cartesian2D Abs() const;
	FORCEINLINE Cartesian2D Min(const Cartesian2D& Other) const;
	FORCEINLINE Cartesian2D Max(const Cartesian2D& Other) const;

	FORCEINLINE float Length() const;
	FORCEINLINE float LengthSquared() const;

	Cartesian2D& Normalize(float ErrorMargin = SUPER_SMALL_NUMBER);
	Cartesian2D Normalized(float ErrorMargin= SUPER_SMALL_NUMBER) const;
	FORCEINLINE bool GetIsNormalized(float ErrorMargin= SUPER_SMALL_NUMBER) const;

	void ToDirectionAndLength(Cartesian2D& Direction, float& Length, float ErrorMargin= KINDA_SMALL_NUMBER) const;

	Cartesian2D Rotate(float Angle) const;
	Cartesian2D Refract(const Cartesian2D& Normal, float IndexOfRefraction) const;

	FORCEINLINE Cartesian2D Reciprocal() const;
	FORCEINLINE Cartesian2D Reflect(const Cartesian2D& Normal) const;

	/**	You know what? I don't know what you'd need these for. But they're here. Enjoy. */
	FORCEINLINE Cartesian2D ToDeg() const;
	FORCEINLINE Cartesian2D ToRad() const;

	/*
	 *	Make this game 'vector' into a platform-specific vectorized intrinsic.
	 *	
	 *	@return Vectorized intrinsic containing this vector
	 **/
	FORCEINLINE Vector AsIntrinsic() const;

	/*
	 *	Make this game 'vector' - along with two other floats - into a platform-specific vectorized intrinsic.
	 *	
	 *	@param Z Used in the 3rd element of the intrinsic
	 *	@param In Used in the 4th element of the intrinsic
	 *	@return Vectorized intrinsic containing this vector and two other floats
	 **/
	FORCEINLINE Vector AsIntrinsic(float Z, float In) const;

	/*
	 *	Combine this game 'vector' and another into a platform-specific vectorized intrinsic.
	 *	
	 *	@param Other Placed in the 3rd and 4th element of the intrinsic 
	 *	@return Vectorized intrinsic containing this vector and another game 'vector'
	 **/
	FORCEINLINE Vector AsIntrinsic(Cartesian2D Other) const;

private:

	float m_Vector[2];
};

#include "Cartesian2D.inl"

/*
 *	Representation of some discrete quantity in a 3D Cartesian space.
 *	EG:
 *	- EulerAngle
 *	- Position
 *	- Direction
 **/
class Cartesian3D
{
	friend class Spatial3D;
	friend class Euler3D;
public:

	FORCEINLINE Cartesian3D();
	FORCEINLINE Cartesian3D(float Val);
	FORCEINLINE Cartesian3D(float X, float Y, float Z);
	FORCEINLINE Cartesian3D(const Cartesian2D& In);
	FORCEINLINE Cartesian3D(const Cartesian2D& In, float Z);
	FORCEINLINE Cartesian3D(const Vector& In);

	FORCEINLINE Cartesian3D operator+(const Cartesian3D& Other) const;
	FORCEINLINE Cartesian3D operator-(const Cartesian3D& Other) const;
	FORCEINLINE Cartesian3D operator*(const Cartesian3D& Other) const;
	FORCEINLINE Cartesian3D operator/(const Cartesian3D& Other) const;

	FORCEINLINE Cartesian3D operator+(float Amount) const;
	FORCEINLINE Cartesian3D operator-(float Amount) const;
	FORCEINLINE Cartesian3D operator*(float Amount) const;
	FORCEINLINE Cartesian3D operator/(float Amount) const;

	FORCEINLINE bool operator==(const Cartesian3D& Other) const;
	FORCEINLINE bool operator!=(const Cartesian3D& Other) const;
	FORCEINLINE bool Equals(const Cartesian3D& Other, float ErrorMargin= KINDA_SMALL_NUMBER) const;
	FORCEINLINE bool Equals(float Val, float ErrorMargin= KINDA_SMALL_NUMBER) const;

	FORCEINLINE Cartesian3D operator-() const;
	FORCEINLINE Cartesian3D& operator+=(const Cartesian3D& Other);
	FORCEINLINE Cartesian3D& operator-=(const Cartesian3D& Other);
	FORCEINLINE Cartesian3D& operator*=(const Cartesian3D& Other);
	FORCEINLINE Cartesian3D& operator/=(const Cartesian3D& Other);

	FORCEINLINE Cartesian3D& operator+=(float Val);
	FORCEINLINE Cartesian3D& operator-=(float Val);
	FORCEINLINE Cartesian3D& operator*=(float Val);
	FORCEINLINE Cartesian3D& operator/=(float Val);

	FORCEINLINE float operator[](uint32 Index) const;

	FORCEINLINE void Set(float X, float Y, float Z);
	FORCEINLINE void Set(uint32 Index, float Val);
	FORCEINLINE float Max() const;
	FORCEINLINE float Min() const;
	FORCEINLINE float AbsMax() const;
	FORCEINLINE float AbsMin() const;

	FORCEINLINE Cartesian3D Abs() const;
	FORCEINLINE Cartesian3D Min(const Cartesian3D& Other) const;
	FORCEINLINE Cartesian3D Max(const Cartesian3D& Other) const;

	/*
	 *	Make this game 'vector' into a platform-specific vectorized intrinsic.
	 *
	 *	@return Vectorized intrinsic containing this vector
	 **/
	FORCEINLINE Vector AsIntrinsic() const;
		
	/*
	 *	Make this game 'vector' - along with one other float -  into a platform-specific vectorized intrinsic.
	 *
	 *	@param In Used in the fourth element of the intrinsic.
	 *	@return Vectorized intrinsic containing this vector and one additional float.
	 **/
	FORCEINLINE Vector AsIntrinsic(float In) const;

protected:
	FORCEINLINE operator Vector&() { return m_Vector; }
	Vector m_Vector;
};

/** @note(devlinw): Since we won't be making use of 2D Eulers, Cartesian2D can serve as both for all I care. */
typedef Cartesian2D Spatial2D;

/*
 *	Specialization of Cartesian3D with functions that only make sense 
 *	operating on "coordinate vectors".
 **/
class Spatial3D
{
public:
	static const Spatial3D Identity;

	static const Spatial3D FrontDir;
	static const Spatial3D RightDir;
	static const Spatial3D UpDir;

	static const Spatial3D One;

	FORCEINLINE Spatial3D() : m_Inner() {}
	FORCEINLINE Spatial3D(const Vector& FromVector) : m_Inner(FromVector) {}
	FORCEINLINE Spatial3D(float X, float Y, float Z) : m_Inner(X, Y, Z) {}
	FORCEINLINE Spatial3D(const Cartesian3D& FromCartesian)	: m_Inner(FromCartesian) {}

	FORCEINLINE Cartesian3D& Inner() { return m_Inner; }
	FORCEINLINE const Cartesian3D& Inner() const { return m_Inner; }

	FORCEINLINE float X() const { return m_Inner[0]; }
	FORCEINLINE float Y() const { return m_Inner[1]; }
	FORCEINLINE float Z() const { return m_Inner[2]; }

	void DirAndLength(Spatial3D& Direction, float& Length) const;

	FORCEINLINE Spatial2D As2D() const;

	FORCEINLINE Spatial3D Project() const;
	FORCEINLINE Spatial3D Reciprocal() const;
	FORCEINLINE Spatial3D Reflect(const Spatial3D& Normal) const;

	FORCEINLINE float Length() const;
	FORCEINLINE float LengthSquared() const;

	Spatial3D Rotate(const Spatial3D& axis, float Angle) const;
	Spatial3D Refract(const Spatial3D& Normal, float IndexOfRefraction) const;

	FORCEINLINE float Dot(const Spatial3D& Other) const;
	FORCEINLINE Spatial3D DotAs3D(const Spatial3D& Other) const;
	FORCEINLINE Spatial3D Cross(const Spatial3D& Other) const;

	FORCEINLINE float Dist(const Spatial3D& Other) const;
	FORCEINLINE float DistSquared(const Spatial3D& Other) const;

	// to unit size
	Spatial3D& Normalize(float ErrorMargin = SUPER_SMALL_NUMBER);
	Spatial3D Normalized(float ErrorMargin = SUPER_SMALL_NUMBER) const;
	FORCEINLINE bool GetIsNormalized(float ErrorMargin = SUPER_SMALL_NUMBER) const;
	
	FORCEINLINE Vector AsIntrinsic(float W = 0.f) const { return m_Inner.AsIntrinsic(W); }

	FORCEINLINE bool operator==(const Spatial3D& Other) const { return m_Inner == Other.Inner(); }
	FORCEINLINE bool operator!=(const Spatial3D& Other) const { return m_Inner != Other.Inner(); }

	FORCEINLINE Spatial3D operator+(const Spatial3D& Other) const { return m_Inner + Other.Inner(); }
	FORCEINLINE Spatial3D operator-(const Spatial3D& Other) const { return m_Inner - Other.Inner(); }
	FORCEINLINE Spatial3D operator*(const Spatial3D& Other) const { return m_Inner * Other.Inner(); }
	FORCEINLINE Spatial3D operator/(const Spatial3D& Other) const { return m_Inner / Other.Inner(); }

	FORCEINLINE Spatial3D operator+(float Amount) const { return m_Inner + Amount; }
	FORCEINLINE Spatial3D operator-(float Amount) const { return m_Inner - Amount; }
	FORCEINLINE Spatial3D operator*(float Amount) const { return m_Inner * Amount; }
	FORCEINLINE Spatial3D operator/(float Amount) const { return m_Inner / Amount; }

	FORCEINLINE Spatial3D operator-() const { return -m_Inner; }
	FORCEINLINE Spatial3D& operator+=(const Spatial3D& Other) { m_Inner += Other.Inner(); return *this; }
	FORCEINLINE Spatial3D& operator-=(const Spatial3D& Other) { m_Inner -= Other.Inner(); return *this; }
	FORCEINLINE Spatial3D& operator*=(const Spatial3D& Other) { m_Inner *= Other.Inner(); return *this; }
	FORCEINLINE Spatial3D& operator/=(const Spatial3D& Other) { m_Inner /= Other.Inner(); return *this; }

	FORCEINLINE Spatial3D& operator+=(float Val) { m_Inner += Val; return *this; }
	FORCEINLINE Spatial3D& operator-=(float Val) { m_Inner -= Val; return *this; }
	FORCEINLINE Spatial3D& operator*=(float Val) { m_Inner *= Val; return *this; }
	FORCEINLINE Spatial3D& operator/=(float Val) { m_Inner /= Val; return *this; }

	FORCEINLINE float operator[](uint32 Index) const { return m_Inner[Index]; }

protected:
	Cartesian3D m_Inner;
};

/*
*	Specialization of Cartesian3D with functions that only make sense
*	operating on Euler angle rotations around the axes of a 3D Cartesian coordinate system.
**/
class Euler3D
{
public:
	static const Euler3D ZeroEuler;
	static const Euler3D EulerToDeg;
	static const Euler3D EulerToRad;

	FORCEINLINE Euler3D() : m_Inner() {}
	FORCEINLINE Euler3D(float Val) : m_Inner(Val) {}
	FORCEINLINE Euler3D(const Vector& FromVector) : m_Inner(FromVector) {}
	FORCEINLINE Euler3D(float Roll, float Pitch, float Yaw) : m_Inner(Roll, Pitch, Yaw) {}
	FORCEINLINE Euler3D(const Cartesian3D& FromCartesian) : m_Inner(FromCartesian) {}
	
	FORCEINLINE Cartesian3D& Inner() { return m_Inner; }
	FORCEINLINE const Cartesian3D& Inner() const { return m_Inner; }

	FORCEINLINE float Roll() const { return m_Inner[0]; }
	FORCEINLINE float Pitch() const { return m_Inner[1]; }
	FORCEINLINE float Yaw() const { return m_Inner[2]; }

	Euler3D(const Spatial3D& FromDirection);

	/*
	 *	Provides an alternate construction when you don't care about roll
	 **/
	FORCEINLINE static Euler3D FromPitchYaw(float Pitch, float Yaw, float Roll = 0.f) { return Euler3D(Roll, Pitch, Yaw); }

	FORCEINLINE Euler3D ToDeg() const;
	FORCEINLINE Euler3D ToRad() const;

	FORCEINLINE Vector AsIntrinsic(float W = 0.f) const { return m_Inner.AsIntrinsic(W); }

	FORCEINLINE bool operator==(const Euler3D& Other) const { return m_Inner == Other.Inner(); }
	FORCEINLINE bool operator!=(const Euler3D& Other) const { return m_Inner != Other.Inner(); }

	FORCEINLINE Euler3D operator+(const Euler3D& Other) const { return m_Inner + Other.Inner(); }
	FORCEINLINE Euler3D operator-(const Euler3D& Other) const { return m_Inner - Other.Inner(); }
	FORCEINLINE Euler3D operator*(const Euler3D& Other) const { return m_Inner * Other.Inner(); }
	FORCEINLINE Euler3D operator/(const Euler3D& Other) const { return m_Inner / Other.Inner(); }

	FORCEINLINE Euler3D operator+(float Amount) const { return m_Inner + Amount; }
	FORCEINLINE Euler3D operator-(float Amount) const { return m_Inner - Amount; }
	FORCEINLINE Euler3D operator*(float Amount) const { return m_Inner * Amount; }
	FORCEINLINE Euler3D operator/(float Amount) const { return m_Inner / Amount; }

	FORCEINLINE Euler3D operator-() const { return -m_Inner; }
	FORCEINLINE Euler3D& operator+=(const Euler3D& Other) { m_Inner += Other.Inner(); return *this; }
	FORCEINLINE Euler3D& operator-=(const Euler3D& Other) { m_Inner -= Other.Inner(); return *this; }
	FORCEINLINE Euler3D& operator*=(const Euler3D& Other) { m_Inner *= Other.Inner(); return *this; }
	FORCEINLINE Euler3D& operator/=(const Euler3D& Other) { m_Inner /= Other.Inner(); return *this; }

	FORCEINLINE Euler3D& operator+=(float Val) { m_Inner += Val; return *this; }
	FORCEINLINE Euler3D& operator-=(float Val) { m_Inner -= Val; return *this; }
	FORCEINLINE Euler3D& operator*=(float Val) { m_Inner *= Val; return *this; }
	FORCEINLINE Euler3D& operator/=(float Val) { m_Inner /= Val; return *this; }

	FORCEINLINE float operator[](uint32 Index) const { return m_Inner[Index]; }

protected:
	Cartesian3D m_Inner;
};

#include "Cartesian3D.inl"