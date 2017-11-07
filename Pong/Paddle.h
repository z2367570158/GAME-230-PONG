#pragma once
#include <SFML/Graphics.hpp>

class Paddle
{
public:
	sf::Vector2f paddleSize;
	float speed = 500.0f;
	sf::RectangleShape picture;

	void setSize(float width, float height);
	void setPosition(float x, float y);
};