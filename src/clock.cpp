#include <gk/clock.hpp>

#include <gk/context.hpp>

uint32_t Clock::GetTime()
{
	return Context::GetTime();
}

Timer::Timer(uint32_t period)
{
	uint32_t time = Clock::GetTime();
	
	m_Previous = time;
	m_Current  = time;
	m_Period   = period;
}

bool Timer::Tick()
{
	bool ret = false;
	m_Current = Clock::GetTime();

	if (m_Current - m_Previous > m_Period)
	{
		m_Previous = m_Current;
		ret = true;
	}

	return ret;
}

float Timer::GetDelta()
{
	float n = static_cast<float>(m_Current - m_Previous);
	float d = static_cast<float>(m_Period);
	
	return n / d;
}

CallbackTimer::CallbackTimer(uint32_t period, void(*callback)()) : Timer(period)
{
	m_Callback = callback;
}

void CallbackTimer::Tick()
{
	if (Timer::Tick())
	{
		m_Callback();
	}
}
