#include "Ball.h"


void Ball::setPosition(float x, float y)
{
	a = 500.f;
	picture.setRadius(radius - 3);

	picture.setOrigin(radius / 2, radius / 2);
	picture.setPosition(sf::Vector2f(x, y));


}