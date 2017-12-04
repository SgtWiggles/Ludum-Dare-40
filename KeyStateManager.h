#pragma once
#include <SFML\Window.hpp>


class Conductor;
struct KeyState;


struct KeyState
{
	enum State
	{
		  UP
		, DOWN
		, HOLD
		, RELEASE
		, UNKNOWN
	};

	KeyState()
		: time(sf::seconds(-1.f))
		, state(UNKNOWN)
		, stroke_counter(SIZE_MAX)
	{}

	KeyState(sf::Time pTime, State pState, size_t stroke)
		: time(pTime)
		, state(pState)
		, stroke_counter(stroke)
	{}

	friend bool operator==(const KeyState&, const KeyState&);

	const sf::Time time;
	const State state;
private:
	const size_t stroke_counter;
};

inline bool operator == (const KeyState& l, const KeyState& r)
{
	return l.stroke_counter == r.stroke_counter && l.state == r.state;
}

inline bool operator != (const KeyState& l, const KeyState& r)
{
	return !(l == r);
}

class KeyStateManager
{
public:
	KeyStateManager();
	void set_key(sf::Keyboard::Key key);
	sf::Keyboard::Key get_key() const;

	void update(const Conductor& conductor);
	KeyState get_key_state() const;

private:
	sf::Keyboard::Key m_key;
	sf::Time m_last_time;
	typename KeyState::State m_last_state;
	size_t m_stroke_counter;
};

