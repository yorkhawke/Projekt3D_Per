#include "Time.h"

Time::Time() :SecondsPerCount(0.0), Delta(-1.0), BaseTime(0), PausedTime(0), PrevTime(0), CurrTime(0), Stopped(false)
{
	__int64 CountPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&CountPerSec);
	SecondsPerCount = 1.0 / (double)CountPerSec;
}

float Time::GameTime()const
{
	return (float)CurrTime;

}

void Time::Start()
{
	__int64 startTime;

	QueryPerformanceFrequency((LARGE_INTEGER*)&startTime);
	if (Stopped)
	{
		PausedTime += (startTime - StopTime);
		PrevTime = startTime;
		StopTime = 0;
		Stopped = false;
	}
}

void Time::ShowFPS()
{
	static int frameC = 0;
	static float TimeE = 0;

	frameC++;
	if (TotalTime() - TimeE >= 1.0f)
	{
		float Fps = (float)frameC;
		float mspf = 1000.0f / Fps;

		frameC = 0;
		TimeE += 1.0f;
		FpsTime = Fps;
	}

	std::cout << FpsTime << std::endl;
}


float Time::TotalTime()const
{
	if (Stopped)
	{
		return (float)(((StopTime - PausedTime) - BaseTime)*SecondsPerCount);

	}
	else
	{
		return (float)(((CurrTime - PausedTime) - BaseTime)*SecondsPerCount);
	}
}

void Time::Stop()
{
	if (!Stopped)
	{
		__int64 currTime;
		QueryPerformanceFrequency((LARGE_INTEGER*)&currTime);
		StopTime = currTime;
		Stopped = true;
	}
}

float Time::DeltaTime()const
{
	return (float)Delta;
}

void Time::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	BaseTime = currTime;
	PrevTime = currTime;
}

void Time::Update()
{
	if (Stopped)
	{
		Delta = 0.0f;
		return;
	}
	__int64 currTime;

	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	CurrTime = currTime;

	Delta = (CurrTime - PrevTime)*SecondsPerCount;

	PrevTime = CurrTime;

	if (Delta < 0.0f)
		Delta = 0.0f;


}