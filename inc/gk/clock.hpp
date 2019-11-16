#ifndef GK_CLOCK_H
#define GK_CLOCK_H

#include <stdint.h>

class Clock
{
public:
	static uint32_t GetTime();
};

class Timer
{
private:
	uint32_t m_Previous;
	uint32_t m_Current;
	uint32_t m_Period;

public:
	Timer(uint32_t period);

	bool  Tick();
	float GetDelta();
};

class CallbackTimer : protected Timer
{
private:
	void (*m_Callback)();

public:
	CallbackTimer(uint32_t period, void (*callback)());

	void Tick();
};

#endif // GK_CLOCK_H