#include "Window.h"

Window::Window()
{

}

Window::Window(float screenWidth, float screenHeight)
{
	ScreenHeight = screenHeight;
	ScreenWidth = screenHeight;
}

Window::~Window()
{

}


HWND Window::getHWND()
{
	return window;
}

void Window::setScreenWidth(int x)
{
	ScreenWidth = x;
}

void Window::setScreenHeight(int x)
{
	ScreenHeight = x;
}
