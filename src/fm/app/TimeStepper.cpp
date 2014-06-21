#include <fm/app/TimeStepper.hpp>

namespace zn
{

//------------------------------------------------------------------------------
void TimeStepper::onBeginFrame()
{
	sf::Time t = time();

	m_timeBefore = t;

	// Record FPS
	if(t - m_lastFPS > sf::milliseconds(1000))
	{
		m_lastFPS = t;
		m_recordedFPS = m_fps;
		m_fps = 0;
	}
	++m_fps;
}

//------------------------------------------------------------------------------
void TimeStepper::onEndFrame()
{
	m_rawDelta = time() - m_timeBefore;
	if(m_rawDelta > m_maxDelta)
	{
		m_rawDelta = m_maxDelta;
	}
}

//------------------------------------------------------------------------------
u32 TimeStepper::callSteps(std::function<void(sf::Time)> stepFunc)
{
	sf::Time delta = m_rawDelta;
	if(m_recordedFPS != 0)
	{
		delta = sf::milliseconds(1000 / m_recordedFPS);
	}

	u32 updatesCount = 0;

	m_storedDelta += delta;
	if(m_storedDelta >= m_minDelta)
	{
		s32 cycles = m_storedDelta.asMilliseconds() / m_maxDelta.asMilliseconds();
		for (s32 i = 0; i < cycles; ++i)
		{
			stepFunc(m_maxDelta);
			++updatesCount;
		}

		sf::Time remainder = sf::milliseconds(m_storedDelta.asMilliseconds() % m_maxDelta.asMilliseconds());
		if(remainder > m_minDelta)
		{
			stepFunc(remainder);
			m_storedDelta = sf::milliseconds(0);
			++updatesCount;
		}
		else
		{
			m_storedDelta = remainder;
		}
	}

	return updatesCount;
}

} // namespace zn

