#pragma once
#include <SFML\Graphics.hpp>

class MainMenu
{
public:
	MainMenu(sf::FloatRect screenRect, sf::Font& font);

	void render(sf::RenderWindow& window);
	void updateHighscore(size_t score);

	void setMainText(const std::string& string);

private:

	void setText(sf::Text& text, const std::string& string);

	sf::RectangleShape menuDim;
	sf::Text mainText;
	sf::Text highscoreWord;
	sf::Text highscoreNumber;
	size_t currentHighscore;
	sf::FloatRect screen;
};

