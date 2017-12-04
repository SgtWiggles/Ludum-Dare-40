#pragma once
#include <SFML\Graphics.hpp>
class JudgeBar
{
public:
	JudgeBar(float radius,float total, float outline_size, sf::Color cCenter, sf::Color cOut, sf::Vector2f center);
	~JudgeBar();
	void add_hit_point(float radius, sf::Vector2f center);

	void draw(sf::RenderWindow& window);
	void reset();

private:
	sf::CircleShape m_inner_circle;
	sf::CircleShape m_outline_circle;
	std::vector<sf::CircleShape> m_hit_points;
	float outline_width;
	float total_width;

};

