#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Ball.h"
#include "Paddle.h"

using namespace sf;

RenderWindow window;

float frameWidth = 1366;
float frameHeight = 768;

sf::Clock clockt;
bool isPlaying = false;
bool gameover = false;

Paddle leftPaddle;
Paddle rightPaddle;
Ball ball;

Paddle obstacle;

Texture textureBall;
Texture textureBack;
RectangleShape background;

sf::Font font;
sf::Text leftScore;
sf::Text rightScore;
sf::Text gameoverText;

SoundBuffer buf;
SoundBuffer BGMbuf;
Sound sound;
Sound BGM;

int score1 = 0;
int score2 = 0;

void Initiate();
void Reset();
void Update();

bool obstacleUp = true;

int main()
{
	window.create(VideoMode(frameWidth, frameHeight), "PONG");
	Initiate();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed ||
				((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
			{
				window.close();
				break;
			}

			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
			{
				if (!isPlaying && !gameover)
				{
					Reset();
				}
			}
		}

		if (gameover)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				gameover = false;
				score1 = 0;
				score2 = 0;
			}
		}


		if (isPlaying&&!gameover)
		{

			float deltaTime = clockt.restart().asSeconds();
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) &&
				(leftPaddle.picture.getPosition().y - leftPaddle.paddleSize.y / 2 > 5.f))
			{
				leftPaddle.picture.move(0.f, - leftPaddle.speed * deltaTime);
			}
			else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) &&
				(leftPaddle.picture.getPosition().y + leftPaddle.paddleSize.y / 2 < frameHeight - 5.f))
			{
				leftPaddle.picture.move(0.f, leftPaddle.speed * deltaTime);
			}

			if (rightPaddle.picture.getPosition().y - rightPaddle.paddleSize.y / 2 > 5.f
				&&ball.picture.getPosition().y + ball.radius < rightPaddle.picture.getPosition().y + rightPaddle.paddleSize.y / 2)
			{
				rightPaddle.picture.move(0.f, -rightPaddle.speed * deltaTime);
			}
			else if ((rightPaddle.picture.getPosition().y + rightPaddle.paddleSize.y / 2 < frameHeight - 5.f)
				&& ball.picture.getPosition().y - ball.radius > rightPaddle.picture.getPosition().y - rightPaddle.paddleSize.y / 2)
			{
				rightPaddle.picture.move(0.f, rightPaddle.speed * deltaTime);
			}

			if (obstacle.picture.getPosition().y - obstacle.paddleSize.y / 2 < 5.f)
				obstacleUp = false;
			else if (obstacle.picture.getPosition().y + obstacle.paddleSize.y / 2 > frameHeight - 5.f)
				obstacleUp = true;

			if (obstacleUp)
				obstacle.picture.move(0.0f, -obstacle.speed* deltaTime);
			else
				obstacle.picture.move(0.0f, obstacle.speed* deltaTime);


			if (ball.picture.getPosition().x > frameWidth / 2)
			{
				ball.vx -= deltaTime*ball.a;
			}
			else
			{

				ball.vx += deltaTime*ball.a;
			}
			
			if (ball.picture.getPosition().y > frameHeight / 2)
			{
				ball.vy -= deltaTime*ball.a;
			}
			else
			{

				ball.vy += deltaTime*ball.a;
			}

			ball.picture.move(ball.vx*deltaTime, ball.vy*deltaTime);



			//physics
			if (ball.picture.getPosition().x - ball.radius < 0.f)
			{
				isPlaying = false;
				score2++;
				rightScore.setString("score:" + std::to_string(score2));

				if (score2 >= 5)
				{
					gameoverText.setString("Game Over, Player2 Wins");
					gameover = true;
				}
			}
			else if (ball.picture.getPosition().x + ball.radius > frameWidth)
			{
				isPlaying = false;
				score1++;
				leftScore.setString("score:" + std::to_string(score1));

				if (score1 >= 5)
				{
					gameoverText.setString("Game Over, Player1 Wins");
					gameover = true;
				}
			}


			if (ball.picture.getPosition().y - ball.radius < 0.f)
			{
				ball.vy = -ball.vy;
				ball.picture.setPosition(ball.picture.getPosition().x, ball.radius + 0.1f);
			}
			else if (ball.picture.getPosition().y + ball.radius > frameHeight)
			{
				ball.vy = -ball.vy;
				ball.setPosition(ball.picture.getPosition().x, frameHeight - ball.radius - 0.1f);
			} 



			if (ball.picture.getPosition().x - ball.radius < leftPaddle.picture.getPosition().x + leftPaddle.paddleSize.x / 2 &&
				ball.picture.getPosition().x - ball.radius > leftPaddle.picture.getPosition().x - leftPaddle.paddleSize.x / 2 &&
				ball.picture.getPosition().y + ball.radius >= leftPaddle.picture.getPosition().y - leftPaddle.paddleSize.y / 2 &&
				ball.picture.getPosition().y - ball.radius <= leftPaddle.picture.getPosition().y + leftPaddle.paddleSize.y / 2)
			{
				if (ball.picture.getPosition().y > leftPaddle.picture.getPosition().y)
					ball.vy = ball.vy + (ball.picture.getPosition().y - leftPaddle.picture.getPosition().y)/1.5;
				else
					ball.vy = ball.vy + (leftPaddle.picture.getPosition().y - ball.picture.getPosition().y)/1.5;

				ball.vx = -ball.vx;
				if (ball.vx > 0)
					ball.vx += 100;
				else
					ball.vx -= 100;
				ball.setPosition(leftPaddle.picture.getPosition().x + ball.radius + leftPaddle.paddleSize.x / 2 + 0.1f, ball.picture.getPosition().y);
				sound.play();
			}

			if (ball.picture.getPosition().x + ball.radius > rightPaddle.picture.getPosition().x - rightPaddle.paddleSize.x / 2 &&
				ball.picture.getPosition().x + ball.radius < rightPaddle.picture.getPosition().x + rightPaddle.paddleSize.x / 2&&
				ball.picture.getPosition().y + ball.radius >= rightPaddle.picture.getPosition().y - rightPaddle.paddleSize.y / 2 &&
				ball.picture.getPosition().y - ball.radius <= rightPaddle.picture.getPosition().y + rightPaddle.paddleSize.y / 2)
			{
				if (ball.picture.getPosition().y > rightPaddle.picture.getPosition().y)
					ball.vy = ball.vy+(ball.picture.getPosition().y - rightPaddle.picture.getPosition().y)/1.5;
				else
					ball.vy = ball.vy - (rightPaddle.picture.getPosition().y - ball.picture.getPosition().y)/1.5;

				ball.vx = -ball.vx;

				if (ball.vx > 0)
					ball.vx += 100;
				else
					ball.vx -= 100;
				ball.setPosition(rightPaddle.picture.getPosition().x - ball.radius - rightPaddle.paddleSize.x / 2 - 0.1f, ball.picture.getPosition().y);
				sound.play();
			}

			if (ball.picture.getPosition().x + ball.radius > obstacle.picture.getPosition().x - obstacle.paddleSize.x / 2 &&
				ball.picture.getPosition().x + ball.radius < obstacle.picture.getPosition().x + obstacle.paddleSize.x / 2 &&
				ball.picture.getPosition().y + ball.radius >= obstacle.picture.getPosition().y - obstacle.paddleSize.y / 2 &&
				ball.picture.getPosition().y - ball.radius <= obstacle.picture.getPosition().y + obstacle.paddleSize.y / 2
				|| ball.picture.getPosition().x - ball.radius < obstacle.picture.getPosition().x + obstacle.paddleSize.x / 2 &&
				ball.picture.getPosition().x - ball.radius > obstacle.picture.getPosition().x - obstacle.paddleSize.x / 2 &&
				ball.picture.getPosition().y + ball.radius >= obstacle.picture.getPosition().y - obstacle.paddleSize.y / 2 &&
				ball.picture.getPosition().y - ball.radius <= obstacle.picture.getPosition().y + obstacle.paddleSize.y / 2)
			{


				if (ball.picture.getPosition().x>obstacle.picture.getPosition().x)
					ball.setPosition(obstacle.picture.getPosition().x + ball.radius + rightPaddle.paddleSize.x / 2 + 0.1f, ball.picture.getPosition().y);
				else
					ball.setPosition(obstacle.picture.getPosition().x - ball.radius - rightPaddle.paddleSize.x / 2 - 0.1f, ball.picture.getPosition().y);

				ball.vx = -ball.vx;
				sound.play();
			}
			Update();



		}

	}

	return EXIT_SUCCESS;

}

void Initiate()
{
	font.loadFromFile("consola.ttf");
	textureBack.loadFromFile("back.jpg");
	textureBall.loadFromFile("Earth.png");
	buf.loadFromFile("Explosion.wav");
	sound.setBuffer(buf);
	BGMbuf.loadFromFile("BGM.flac");
	BGM.setBuffer(BGMbuf);
	BGM.play();


	background.setSize(Vector2f(frameWidth, frameHeight));
	background.setPosition(0, 0);
	background.setTexture(&textureBack);

	window.draw(background);

	leftPaddle.setSize(25, 100);
	rightPaddle.setSize(25, 100);
	rightPaddle.speed = 400.f;

	obstacle.setSize(25,100);

	leftScore.setFont(font);
	leftScore.setCharacterSize(50);
	leftScore.setFillColor(Color::White);
	leftScore.setPosition(120, 10);

	rightScore.setFont(font);
	rightScore.setCharacterSize(50);
	rightScore.setFillColor(Color::White);
	rightScore.setPosition(1000, 10);



	gameoverText.setFont(font);
	gameoverText.setCharacterSize(80);
	gameoverText.setFillColor(Color::White);
	gameoverText.setPosition(200, 300);
	gameoverText.setString("Press space to start");

	window.draw(background);
	window.draw(gameoverText);
	window.display();
}

void Reset()
{
	clockt.restart();
	isPlaying = true;

	leftPaddle.setPosition(10 + leftPaddle.paddleSize.x / 2, frameHeight / 2);
	rightPaddle.setPosition(frameWidth - 10 - rightPaddle.paddleSize.x / 2, frameHeight / 2);

	obstacle.setPosition((frameWidth-obstacle.paddleSize.x/2)/2, obstacle.paddleSize.y/2);

	ball.setPosition(frameWidth / 2, frameHeight / 2);

	leftScore.setString("score:" + std::to_string(score1));
	rightScore.setString("score:" + std::to_string(score2));

	gameoverText.setString("");
	window.draw(gameoverText);

	ball.picture.setTexture(&textureBall);
	do
	{
		ball.vx = std::rand() % 1800 - 900;
		ball.vy = std::rand() % 1400 - 700;

	} while (std::abs(ball.vx) < 800 || std::abs(ball.vy) < 600);

	window.draw(background);
	window.draw(leftPaddle.picture);
	window.draw(rightPaddle.picture);
	window.draw(obstacle.picture);
	window.draw(ball.picture);
	window.draw(leftScore);
	window.draw(rightScore);
	window.draw(gameoverText);
	window.display();

}

void Update()
{
	

	window.clear(sf::Color::Black);
	window.draw(background);
	window.draw(leftPaddle.picture);
	window.draw(rightPaddle.picture);
	window.draw(obstacle.picture);
	window.draw(ball.picture);
	window.draw(leftScore);
	window.draw(rightScore);
	window.draw(gameoverText);
	window.display();

}
