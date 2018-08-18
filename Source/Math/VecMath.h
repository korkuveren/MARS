#pragma once

#include "EngineCore/MemoryManager.h"
#include "Platform/PlatformVectorMath.h"
#include "Math.h"

typedef PlatformVector Vector;

struct VectorConstants
{
public:
	static const Vector ZERO;
	static const Vector ONE;
	static const Vector TWO;
	static const Vector HALF;
	static const Vector INF;
	static const Vector MASK_X;
	static const Vector MASK_Y;
	static const Vector MASK_Z;
	static const Vector MASK_W;
	static const Vector SIGN_MASK;
};

template<>
inline Vector Math::Lerp(const Vector& Val1, const Vector& Val2, const float& Amount)
{
	return (Val2 - Val1) * Vector::Load1f(Amount) + Val1;
}

