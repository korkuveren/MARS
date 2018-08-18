#pragma once

#include "EngineCore/EngineUtils.h"

class SDLApplication
{

public:

	static SDLApplication* StartApplication();
	
	virtual ~SDLApplication();
	virtual void HandleMessage(double Delta);
	virtual bool GetIsRunning() const { return bIsRunning; };

private:

	uint8 bIsRunning:1;
	static uint32 ActiveApplications;

	SDLApplication();
};
