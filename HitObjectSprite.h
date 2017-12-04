#pragma once
#include <SFML\Graphics.hpp>
#include <Thor\Shapes.hpp>
class HitObjectSprite
{
public:
	HitObjectSprite(float radius, float total,  float outline, sf::Color inner, sf::Color outer);

	void setPosition(sf::Vector2f pos);
	sf::Vector2f getPosition() const;
	void draw(sf::RenderWindow& window);
	void addMulti(float width, sf::Vector2f cent);
	sf::Color getColor() const;

private:

	sf::CircleShape centerCircle;
	sf::CircleShape innerCircle;
	sf::CircleShape outlineCircle;
	sf::CircleShape connector;
	sf::Vector2f center;
	float connectWidth;
	bool hasConnector;
};

