#ifndef WINDOW_H
#define WINDOW_H

#include "Globals.h"

class Window
{
public:
	Window();
	Window(float screenWidth, float screenHeight);
	~Window();
	HWND getHWND();
	void setScreenWidth(int x);
	void setScreenHeight(int x);
protected:
	HWND window;
	float ScreenWidth, ScreenHeight;
};
#endif

