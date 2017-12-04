#include "HPManager.h"
#include <algorithm>
#include <fmt\printf.h>

HPManager::HPManager(sf::Color full, sf::Color half, sf::Color none)
	: hp(hpMax)
	, colors()
{
	colors[0] = sf::Vector3f(full.r, full.g, full.b);
	colors[1] = sf::Vector3f(half.r, half.g, half.b);
	colors[2] = sf::Vector3f(none.r, none.g, none.b);
}

HPManager::~HPManager()
{ }

void HPManager::damage(float amount)
{
	hp -= amount;
	if (hp > hpMax)
		hp = hpMax;
	//if (hp <= 0)
	//	hp = 100;
}

bool HPManager::dead() const
{
	return hp <= 0;
}

void HPManager::reset()
{
	hp = hpMax;
}

void HPManager::render(sf::RenderWindow & window)
{
	window.clear(hpToColor());
}

sf::Color HPManager::hpToColor()
{
	sf::Vector3f color(255, 255, 255);	
	color.x = colors[2].x + (hp / hpMax) * (colors[0].x - colors[2].x);
	color.y = colors[2].y + (hp / hpMax) * (colors[0].y - colors[2].y);
	color.z = colors[2].z + (hp / hpMax) * (colors[0].z - colors[2].z);
	return sf::Color(color.x, color.y, color.z, 255);
}
