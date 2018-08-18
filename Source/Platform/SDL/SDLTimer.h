#ifndef TIMING_HPP
#define TIMING_HPP

#include "EngineCore/EngineUtils.h"

struct SDLTiming
{
	static double getTime();
	static void sleep(uint32 milliseconds);
};

#endif
