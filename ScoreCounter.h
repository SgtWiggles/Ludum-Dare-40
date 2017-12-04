#pragma once
#include <SFML\Graphics.hpp>
#include <Thor\Animations.hpp>

class ScoreCounter
{
public:
	ScoreCounter(sf::Font& font);
	
	size_t addScore(size_t amount);
	size_t getScore();
	
	void reset();
	void setPosition(sf::Vector2f pos);
	void render(sf::RenderWindow& window);

private:
	struct ScaleUpDownAnimation
	{
		sf::Vector2f baseSize;
		sf::Vector2f delta;
		void operator()(sf::Transformable& animated, float progress)
		{
			float val = std::abs(((progress - 0.5f) / 2.0f));
			animated.setScale( 1.0f + delta.x * val, 1.0f + delta.y * val);
		}
	};


	sf::Clock clock;
	size_t displayScore;
	size_t score;
	sf::Text text;
	thor::Animator<sf::Text, size_t> anim;
};

