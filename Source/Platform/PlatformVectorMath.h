#pragma once

#include "Platform.h"

#if defined(SIMD_CPU_ARCH_x86) || defined(SIMD_CPU_ARCH_x86_64)
#include "sse/sseVecmath.hpp"
	typedef SSEVector PlatformVector;
#else
#include "Generic/GenericVectorMath.h"
	typedef BaseVector PlatformVector;
#endif
