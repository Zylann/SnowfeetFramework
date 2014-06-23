#ifndef HEADER_ZN_TIMESTEPPER_HPP_INCLUDED
#define HEADER_ZN_TIMESTEPPER_HPP_INCLUDED

#include <SFML/System.hpp>
#include <fm/types.hpp>

namespace zn
{

/// \brief This class encapsulates time management for mainLoop-based games,
/// and provides an algorithm for smoothing time delta variations, resulting in
/// a better player experience (no more little jitterings).
class ZN_API TimeStepper
{
public:

	/// \brief Creates a TimeStepper with default parameters.
	TimeStepper():
		m_fps(0),
		m_recordedFPS(60),
		m_minDelta(sf::seconds(1.f / 120.f)),
		m_maxDelta(sf::seconds(1.f / 60.f))
	{
		m_rawDelta = m_maxDelta;
	}

	/// \brief Gets eleapsed time from the TimeStepper's creation.
	inline sf::Time time() const
	{
		return m_time.getElapsedTime();
	}

	/// \brief Gets the measured delta time between onBeginFrame() and onEndFrame().
	inline sf::Time rawDelta() const
	{
		return m_rawDelta;
	}

	/// \brief Sets the expected range for the time delta.
	/// \param minDelta: minimum time interval
	/// \param minDelta: maximum time interval
	inline void setDeltaRange(sf::Time minDelta, sf::Time maxDelta)
	{
		m_minDelta = minDelta;
		m_maxDelta = maxDelta;
	}

	/// \brief Gets the minimal expected time delta
	inline sf::Time minDelta() const
	{
		return m_minDelta;
	}

	/// \brief Gets the maximal expected time delta
	inline sf::Time maxDelta() const
	{
		return m_maxDelta;
	}

	/// \brief Gets the number of frames measured in 1 second.
	/// This value is not instant, so it can be zero when you start using the TimeStepper.
	inline u32 recordedFPS() const
	{
		return m_recordedFPS;
	}

	/// \brief Call this function when a frame begins (before events, updates or rendering).
	void onBeginFrame();
	/// \brief Call this function when a frame ends (after event, updates or rendering).
	void onEndFrame();

	/// \brief Use this function to call a regular-time-delta step function (as your game logic update).
	/// depending on the current delta time, the provided callback will be called once or several times
	/// in order to get a fixed-time step logic.
	/// \param stepFunc: update callback to call
	/// \return number of times the given callback has been called
	u32 callSteps(std::function<void(sf::Time)> stepFunc);

private:

	u32 m_fps;
	u32 m_recordedFPS;
	sf::Time m_lastFPS;

	sf::Time m_minDelta;
	sf::Time m_maxDelta;
	sf::Time m_timeBefore;
	sf::Time m_rawDelta;
	sf::Time m_storedDelta;
	sf::Clock m_time;

};

} // namespace zn

#endif // HEADER_ZN_TIMESTEPPER_HPP_INCLUDED

