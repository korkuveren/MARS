#pragma once

#include "EngineCore/EngineUtils.h"
#include "EngineCore/Application.h"
#include "DataTypes/MString.h"

#include <SDL2/SDL.h>

typedef SDL_Window* WindowHandle;

class SDLWindow
{
public:
	SDLWindow(const Application& app, int32 width, int32 height, const String& title);
	virtual ~SDLWindow();

	WindowHandle getWindowHandle();
	void changeSize(int32 width, int32 height);
	void present();
	int32 getWidth() const;
	int32 getHeight() const;
private:
	WindowHandle window;
	int32 width;
	int32 height;
};
