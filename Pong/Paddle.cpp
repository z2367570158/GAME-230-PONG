#include "Paddle.h"


void Paddle::setPosition(float x, float y)
{
	picture.setSize(paddleSize - sf::Vector2f(3, 3));
	picture.setOutlineThickness(3);
	picture.setOutlineColor(sf::Color::White);
	picture.setFillColor(sf::Color::White);
	picture.setOrigin(paddleSize / 2.f);
	picture.setPosition(sf::Vector2f(x,y));
}


void Paddle::setSize(float width, float height)
{
	paddleSize = sf::Vector2f(width, height);
}