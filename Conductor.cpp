#include "Conductor.h"
#include <fmt/printf.h>

Conductor::Conductor()
{
	m_tick_buffer.loadFromFile("resources/click/click.wav");
	m_tick_sound.setBuffer(m_tick_buffer);
}

Conductor::~Conductor()
{ 
	m_tick_sound.stop();
}


void Conductor::begin()
{
	m_clock.restart();
	m_lastFrame = m_clock.getElapsedTime();
	m_sinceLastBeat = sf::seconds(0);
	m_lastBeat = sf::seconds(0);
	m_cTick = 0;
	setBpm(100.f);
}

void Conductor::update()
{
	const sf::Time now = m_clock.getElapsedTime();
	const sf::Time delta = now - m_lastFrame;
	m_lastFrame = now;
	m_sinceLastBeat += delta;

	if (m_sinceLastBeat >= m_time_per)
	{
		//play sounds!
		m_tick_sound.stop();
		m_tick_sound.play();
		m_lastBeat += m_time_per;
		m_sinceLastBeat -= m_time_per;
		m_cTick += 1;
	}
}

void Conductor::setBpm(float bpm)
{
	m_bpm = bpm;
	m_time_per = sf::microseconds(60000000 / bpm);
}

sf::Time Conductor::now() const
{
	return m_lastFrame + getDelta();
}

sf::Time Conductor::next() const
{
	return m_lastBeat + m_time_per;
}

sf::Time Conductor::last() const
{
	return m_lastBeat;
}

size_t Conductor::currentTick() const
{
	return m_cTick;
}

sf::Time Conductor::beat_time(float beat) const
{
	//fmt::print("beat time calce last {} | beat {} | cTick {} | timeper {}\n", last().asMilliseconds(), beat, currentTick(), m_time_per.asMilliseconds());
	return last() + ((beat - currentTick())  * m_time_per);
}

float Conductor::getBeatProgress() const
{
	
	return float((now() - next())/ m_time_per);
}

float Conductor::getBpm() const
{
	return m_bpm;
}


sf::Time Conductor::getDelta() const
{
	const sf::Time now = m_clock.getElapsedTime();
	const sf::Time delta = now - m_lastFrame;
	return delta;
}
