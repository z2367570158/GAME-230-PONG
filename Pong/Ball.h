#pragma once
#include <SFML/Graphics.hpp>

class Ball
{
public:
	float radius = 20.f;

	float a;
	float vx, vy;
	sf::CircleShape picture;

	void setPosition(float x, float y);
};