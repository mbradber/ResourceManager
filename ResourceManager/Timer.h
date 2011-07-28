#pragma once

class Timer
{
public:
	Timer();
	
	void start();
	void stop();
	void pause();
	void resume();
	void reset();

	float getGameTime();
	bool updateFPS();
	double getDelta();

private:
	bool paused;
	bool stopped;
	
	__int64 gameTime;
	__int64 initTime;

	bool fpsMutex;
	float currTime;
	float lastDelta;
	
};

