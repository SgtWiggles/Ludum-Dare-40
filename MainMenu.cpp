#include "MainMenu.h"



MainMenu::MainMenu(sf::FloatRect screenRect, sf::Font& font)
{
	screen = screenRect;
	currentHighscore = 0;
	menuDim.setFillColor(sf::Color(0, 0, 0, 156));	
	menuDim.setSize({ screenRect.width, screenRect.height });
	menuDim.setPosition(0, 0);

	mainText.setFont(font);
	mainText.setCharacterSize(45);

	highscoreWord.setFont(font);
	highscoreWord.setCharacterSize(30);

	highscoreNumber.setFont(font);	
	highscoreNumber.setCharacterSize(30);

	setText(mainText, "Press space to begin!");
	setText(highscoreWord,"Current highscore:");
	setText(highscoreNumber,"0");

	mainText.setPosition(screen.width / 2.f, screen.height / 2.f);
	highscoreWord.setPosition(screen.width / 2.f, screen.height / 2.f + mainText.getLocalBounds().height);
	highscoreNumber.setPosition(screen.width / 2.f, screen.height / 2.f + mainText.getLocalBounds().height + highscoreWord.getLocalBounds().height);


}

void MainMenu::render(sf::RenderWindow & window)
{
	window.draw(menuDim);
	window.draw(mainText);
	window.draw(highscoreWord);
	window.draw(highscoreNumber);
	//mainText.setString(std::to_string());

}

void MainMenu::updateHighscore(size_t score)
{
	if (score > currentHighscore)
	{
		currentHighscore = score;
	}
	setText(highscoreNumber, std::to_string(currentHighscore));
}

void MainMenu::setMainText(const std::string& string)
{
	setText(mainText, string);
}

void MainMenu::setText(sf::Text & text, const std::string & string)
{
	text.setString(string);
	auto bounds = text.getLocalBounds();
	text.setOrigin({ bounds.width / 2.f, bounds.height  * 1.5f });
}

