#include <gk/clock.hpp>

/*
Clock::Clock()
{
    m_Previous = 0;
    m_Current = 0;
}

void Clock::Update()
{
    m_Previous = m_Current;
    m_Current = SDL_GetTicks();
}

uint32_t Clock::Delta()
{
    return (m_Current - m_Previous);
}

Timer::Timer(uint32_t period) : Clock()
{
    m_Period = period;
}

bool Timer::Tick()
{
    m_Current = SDL_GetTicks();

    if((m_Current - m_Previous) >= m_Period)
    {
        m_Previous = m_Current;
        return true;
    }

    return false;
}

float Timer::Delta()
{
    return (static_cast<float>(Clock::Delta()) / static_cast<float>(m_Period));
}
*/
