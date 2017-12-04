#include "KeyStateManager.h"

#include "Conductor.h"
#include <fmt/printf.h>

KeyStateManager::KeyStateManager()
	: m_key(sf::Keyboard::Unknown)
	, m_stroke_counter(0)
	, m_last_state(KeyState::UP)
	, m_last_time()
{ }

void KeyStateManager::set_key(sf::Keyboard::Key key)
{
	m_key = key;
}

sf::Keyboard::Key KeyStateManager::get_key() const
{
	return m_key;
}

void KeyStateManager::update(const Conductor & conductor)
{
	const auto pressed = sf::Keyboard::isKeyPressed(m_key);
	if (m_key == sf::Keyboard::Num8)
	{	
		fmt::print("key {} | pressed {}",  m_key, pressed);
	}

	switch (m_last_state)
	{	
	case KeyState::UP: 
		if (pressed)
		{
			m_last_state = KeyState::DOWN;
			m_last_time = conductor.now();
			m_stroke_counter += 1;

		}
		break;

	case KeyState::DOWN:
		if (pressed) m_last_state = KeyState::HOLD;
		
		if (!pressed)
		{
			m_last_state = KeyState::RELEASE;
			m_last_time = conductor.now();
			m_stroke_counter += 1;
		}
		
		break;

	case KeyState::HOLD:
		if (!pressed)
		{
			m_last_state = KeyState::RELEASE;
			m_last_time = conductor.now();
			m_stroke_counter += 1;
		}

		break;
	
	case KeyState::RELEASE:
		if (!pressed)  m_last_state = KeyState::UP;
		
		if (pressed)
		{
			m_last_state = KeyState::DOWN;
			m_last_time = conductor.now();
			m_stroke_counter += 1;
		}

		break;


	default:
		break;
	}
}

KeyState KeyStateManager::get_key_state() const
{
	return KeyState(m_last_time, m_last_state, m_stroke_counter);
}