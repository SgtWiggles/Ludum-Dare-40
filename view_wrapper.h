#pragma once
#include <SFML\Graphics\View.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

template<class T>
struct view_wrapper
{
public:
	view_wrapper(sf::View&& pview, const sf::Vector2<T>& pos, const sf::Vector2<T>& psize)
		: view(pview)
		, position(pos)
		, size(psize)
	{}

	void update() 
	{
		view.setCenter(position);
		view.setSize(size);
	}
	
	void update_window(sf::RenderWindow& window) 
	{
		window.setView(view);
	}

	sf::View view;
	const sf::Vector2<T>& position;
	const sf::Vector2<T>& size;
};

