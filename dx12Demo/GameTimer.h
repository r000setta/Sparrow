#pragma once

class GameTimer {
public:
	GameTimer();
	float TotalTime() const;
	float DeltaTime() const;
	void Reset();	//开始消息循环前调用
	void Start();	
	void Stop();
	void Tick();	//每帧调用

private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;

};