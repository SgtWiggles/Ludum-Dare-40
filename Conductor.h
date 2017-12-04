#pragma once
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>

class Conductor
{
public:
	Conductor();
	~Conductor();

	void begin();
	void update();
	void setBpm(float bpm);
	
	sf::Time now() const;
	sf::Time next() const;
	sf::Time last() const;
	size_t currentTick() const;
	sf::Time beat_time(float) const; //returns time that this beat went off relative to the current beat!

	float getBeatProgress() const;
	float getBpm() const;
	sf::Time getDelta() const;

private:
	sf::Clock m_clock;
	sf::Time  m_lastFrame;
	sf::Time  m_lastBeat;
	sf::Time  m_sinceLastBeat;

	size_t m_cTick;

	float m_bpm;
	sf::Time m_time_per;

	sf::Sound m_tick_sound;
	sf::SoundBuffer m_tick_buffer;

};

