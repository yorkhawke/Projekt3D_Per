#ifndef TIME_H
#define TIME_H

#include "Globals.h"

class Time
{
public:
	Time();
	float GameTime()const;
	float DeltaTime()const;
	float TotalTime()const;
	float GetFpsTime()const;

	void ShowFPS();
	void Start();
	void Stop();
	void Reset();
	void Update();

private:
	double SecondsPerCount;
	float Delta;
	float FpsTime;
	__int64 BaseTime;
	__int64 PausedTime;
	__int64 StopTime;
	__int64 PrevTime;
	__int64 CurrTime;

	bool Stopped;

};

#endif