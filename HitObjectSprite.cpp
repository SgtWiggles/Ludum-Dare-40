#include "HitObjectSprite.h"
#include<Thor/Vectors.hpp>

HitObjectSprite::HitObjectSprite(float radius, float total, float outline, sf::Color inner, sf::Color outer)
	: innerCircle()
	, outlineCircle()
	, hasConnector (false)
{
	innerCircle.setRadius(radius - total / 2);
	innerCircle.setOrigin({ radius - total / 2, radius - total / 2 });
	innerCircle.setOutlineColor(inner);
	innerCircle.setFillColor(sf::Color::Transparent);
	innerCircle.setOutlineThickness(total);

	const float inRad = innerCircle.getRadius();
	outlineCircle.setRadius(inRad - outline);
	outlineCircle.setOrigin({ inRad - outline,inRad - outline});
	outlineCircle.setOutlineColor(outer);
	outlineCircle.setFillColor(sf::Color::Transparent);
	outlineCircle.setOutlineThickness(total + (outline * 2));

	centerCircle.setRadius(radius / 10);
	centerCircle.setOrigin(centerCircle.getRadius(), centerCircle.getRadius());
	centerCircle.setOutlineColor(outer);
	centerCircle.setOutlineThickness(outline / 3.f);
	centerCircle.setFillColor(innerCircle.getOutlineColor());

	setPosition({ 0.f, 0.f });
}

void HitObjectSprite::setPosition(sf::Vector2f pos)
{
	innerCircle.setPosition(pos);
	outlineCircle.setPosition(pos);
	centerCircle.setPosition(pos);
}

sf::Vector2f HitObjectSprite::getPosition() const
{
	return innerCircle.getPosition();
}



void HitObjectSprite::draw(sf::RenderWindow & window)
{
	window.draw(outlineCircle);
	window.draw(innerCircle);
	window.draw(centerCircle);
	if (hasConnector)
	{
		const float rad = thor::length(innerCircle.getPosition() - center) - 0.5 * connectWidth;
		connector.setOrigin(rad, rad);
		connector.setRadius(rad);
		//window.draw(connector);
	}
	
}

void HitObjectSprite::addMulti(float width, sf::Vector2f cent)
{
	hasConnector = true;
	sf::Color color = innerCircle.getOutlineColor();
	color.a = 124;
	center = cent;
	connectWidth = width;
	connector.setFillColor(sf::Color::Transparent);
	connector.setOutlineColor(color);
	connector.setOutlineThickness(width);
	connector.setPosition(center);
}

sf::Color HitObjectSprite::getColor() const
{
	return innerCircle.getOutlineColor();
}

