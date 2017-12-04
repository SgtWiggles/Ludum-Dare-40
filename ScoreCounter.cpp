#include "ScoreCounter.h"
#include <string>


ScoreCounter::ScoreCounter(sf::Font& font)
{
	reset();

	ScaleUpDownAnimation scale{ {1.0f,1.0f}, {2.f,2.f} };
	anim.addAnimation(1, scale, sf::seconds(.1f));
	text.setFont(font);
	text.setCharacterSize(50.f);

}

size_t ScoreCounter::addScore(size_t amount)
{
	score += amount;
	return score;
}

size_t ScoreCounter::getScore()
{
	return score;
}

void ScoreCounter::reset()
{
	score = 0;
	displayScore = 1;
	text.setString("0");
}

void ScoreCounter::setPosition(sf::Vector2f pos)
{
	text.setPosition(pos);
}

void ScoreCounter::render(sf::RenderWindow & window)
{
	if (score != displayScore)
	{
		anim.playAnimation(1);
		text.setString(std::to_string(score));
		auto bounds = text.getLocalBounds();
		text.setOrigin({ bounds.width / 2.f, bounds.height  * 1.5f});
		
		displayScore = score;
	}
	anim.update(clock.restart());
	anim.animate(text);
	window.draw(text);
}


