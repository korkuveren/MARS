#pragma once

#include "Platform/PlatformTimerManager.h"
#include "EngineCore/EngineUtils.h"

namespace Time
{
	inline double getTime()
	{
		return PlatformTiming::getTime();
	}

	inline void sleep(uint32 milliseconds)
	{
		PlatformTiming::sleep(milliseconds);
	}
};

