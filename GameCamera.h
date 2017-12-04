#pragma once
#include <SFML\Graphics.hpp>
class GameCamera
{
public:
	GameCamera(sf::Vector2f size);
	~GameCamera();

	sf::View get_view() const;
	sf::Vector2f get_size() const;

private:
	sf::View m_view;
};

