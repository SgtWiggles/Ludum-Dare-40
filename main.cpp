#include <iostream>

#include <SFML\Graphics.hpp>
#include <SFML\Window\Event.hpp>

#include <fmt\printf.h>

#include "Conductor.h"
#include "GameCamera.h"
#include "AnimationLoader.h"
#include "HitObjects.h"
#include "KeyBindingManager.h"
#include "HPManager.h"
#include "ScoreCounter.h"
#include "MainMenu.h"

struct difficulty
{
	difficulty()
	{
		reset();
	}

	void reset()
	{

		last=	0			 ;
		lastScore=	0		 ;
		maxObjNext=		5	 ;
		nextOpenLane=		5;

	}


	size_t last		;
	size_t lastScore;
	size_t maxObjNext;
	size_t nextOpenLane;

	void operator()(Conductor& conductor, ScoreCounter& score, HitObjects& hit)
	{


	const auto cTick = conductor.currentTick();
	if (last != cTick)
	{

		if (cTick % 15 == 0)
		{

			conductor.setBpm(conductor.getBpm() + 10);

		}
		last = cTick;
	}


	const auto cScore = score.getScore();
	if (lastScore != cScore)
	{
		if (cScore >= nextOpenLane)
		{
			hit.addLane();
			nextOpenLane += 25;
		}
		if (cScore >= maxObjNext)
		{
			hit.addMaxObjects();
			maxObjNext += 40;
		}

		lastScore = cScore;
	}

}
};


enum class GameState
{
	  Game
	, Menu

};

int main(int argc, char** argv)
{
	sf::ContextSettings window_context_settings;
	window_context_settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(600, 600), "LD40", sf::Style::Default, window_context_settings);
	
	sf::Font font;
	font.loadFromFile("./resources/GOODDP__.TTF");
	
	difficulty difficulty_func;
	Conductor conductor;
	GameCamera camera({800,800});
	HitObjects objects(4, camera.get_size());
	KeyBindingManager keybinds;
	HPManager hp( sf::Color(21, 124, 33)
				, sf::Color(241, 216, 133)
				, sf::Color(124, 21, 21));
	ScoreCounter score(font);
	MainMenu mainMenu({ {0.f,0.f}, camera.get_size() }, font);
	score.setPosition(camera.get_size() / 2.f);
	score.addScore(0);
	window.setView(camera.get_view());
	keybinds.load_key_bindings();

	//animator.addAnimation("up", anim[0], sf::seconds(0.1f));
	//world size 144,144
	sf::Clock clock;

	GameState state = GameState::Menu;

	while (window.isOpen())
	{
		switch (state)
		{
		case GameState::Game:
			score.reset();
			hp.reset();
			objects.reset();
			difficulty_func.reset();
			conductor.begin();
			conductor.setBpm(100);
			
			

			while (!hp.dead() && window.isOpen())
			{
				sf::Event e;
				while (window.pollEvent(e))
				{
					if (e.type == sf::Event::Closed)
					{
						window.close();
					}
				}

				hp.render(window);
				difficulty_func(conductor, score, objects);
				conductor.update();
				keybinds.update(conductor);

				objects.update(conductor);
				objects.handle_inputs(keybinds, conductor, hp, score);
				objects.destroy_old_objects();
				objects.prepare_draw();
				score.render(window);
				
				objects.draw(window);

				window.display();
			}
			
			mainMenu.setMainText("Failed... Space to Play Again.");
			mainMenu.updateHighscore(score.getScore());
			state = GameState::Menu;

			break;

		case GameState::Menu:
			while (state == GameState::Menu && window.isOpen())
			{
				sf::Event e;
				while (window.pollEvent(e))
				{
					if (e.type == sf::Event::Closed)
					{
						window.close();
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
					state = GameState::Game;

				hp.render(window);
				score.render(window);
				objects.draw(window);
				mainMenu.render(window);
				window.display();
			}
							
			break;
		}

	}

	return 0;
}