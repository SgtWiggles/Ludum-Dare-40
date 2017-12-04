#include "JudgeBar.h"


JudgeBar::JudgeBar(float radius, float total, float outline_size, sf::Color cCenter, sf::Color cOut, sf::Vector2f center)
	: m_inner_circle()
	, m_outline_circle()
	, m_hit_points()
	, outline_width(outline_size)
	, total_width(total)
{
	m_inner_circle.setRadius(radius - total / 2);
	m_inner_circle.setOrigin({ radius - total / 2, radius - total / 2 });
	m_inner_circle.setOutlineColor(cCenter);
	m_inner_circle.setFillColor(sf::Color::Transparent);
	m_inner_circle.setOutlineThickness(total);
	m_inner_circle.setPosition(center);


	const float inRad = m_inner_circle.getRadius();
	m_outline_circle.setRadius(inRad - outline_width);
	m_outline_circle.setOrigin({ inRad - outline_width,inRad - outline_width });
	m_outline_circle.setOutlineColor(cOut);
	m_outline_circle.setFillColor(sf::Color::Transparent);
	m_outline_circle.setOutlineThickness(total + (outline_width  * 2));
	m_outline_circle.setPosition(center);
}

JudgeBar::~JudgeBar()
{ }

void JudgeBar::add_hit_point(float radius, sf::Vector2f center)
{

	m_hit_points.push_back(sf::CircleShape());
	auto& circle = m_hit_points.back();
	circle.setRadius(radius);
	circle.setOrigin({ radius,radius });
	circle.setOutlineColor(m_outline_circle.getOutlineColor());
	circle.setFillColor(m_inner_circle.getOutlineColor());
	circle.setOutlineThickness(outline_width);
	circle.setPosition(center);
}

void JudgeBar::draw(sf::RenderWindow & window)
{
	window.draw(m_outline_circle);
	for (auto& i : m_hit_points)
	{
		window.draw(i);
	}
	window.draw(m_inner_circle);

}

void JudgeBar::reset()
{
	m_hit_points.clear();
}


