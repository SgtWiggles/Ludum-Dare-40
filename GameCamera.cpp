#include "GameCamera.h"

GameCamera::GameCamera(sf::Vector2f size)
	: m_view(sf::FloatRect({0,0}, size))
	//Assumes 16x16 tiles
{ }

GameCamera::~GameCamera()
{ }

sf::View GameCamera::get_view() const
{
	return m_view;
}

sf::Vector2f GameCamera::get_size() const
{
	return m_view.getSize();
}
