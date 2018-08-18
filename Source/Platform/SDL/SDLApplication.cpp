#include "SDLApplication.h"
#include "EngineCore/EngineUtils.h"
#include <SDL2/SDL.h>

uint32 SDLApplication::ActiveApplications = 0;

SDLApplication* SDLApplication::StartApplication()
{
	const uint32 _SDLFlags = SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS;
	uint32 _bInit = SDL_WasInit(_SDLFlags);
	if(_bInit != _SDLFlags && SDL_Init(_SDLFlags) != 0) 
	{
		DEBUG_LOG("SDLApplication", LOG_ERROR, "SDL_Init: %s", SDL_GetError());
		return nullptr;
	}

	return new SDLApplication();
}

SDLApplication::SDLApplication()
{
	ActiveApplications++;
	bIsRunning = true;
}

SDLApplication::~SDLApplication()
{
	ActiveApplications--;
	if (ActiveApplications == 0) 
	{
		SDL_Quit();
	}
}

void SDLApplication::HandleMessage(double Delta)
{
	SDL_Event _Event;
	(void)Delta;
	
	while (SDL_PollEvent(&_Event)) 
	{
		switch(_Event.type)
		{

		case SDL_QUIT: bIsRunning = false;
			break;

		default:
			break;
		};
	}
}