#pragma once
#include <SFML\Graphics.hpp>
#include <array>
class HPManager
{
public:
	HPManager(sf::Color full, sf::Color half, sf::Color none);
	~HPManager();

	void damage(float amount);
	bool dead() const;
	void reset();

	void render(sf::RenderWindow& window);

private:
	sf::Color hpToColor();

	float hp;
	std::array<sf::Vector3f,3> colors;
	static constexpr float hpMax = 100.f;
	
};

