#include "Player.h"

Player::Player(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, float speed):
	animation(texture,imageCount,switchTime)
{
	this->speed = speed;
	row = 0;
	faceRight = true;

	body.setSize(sf::Vector2f(64.0f, 64.0f));
	body.setOrigin(body.getSize() / 2.0f);
	body.setPosition(206.0f, 206.0f);
	body.setTexture(texture);

}
Player::~Player()
{
}

void Player::Update(float deltaTime)
{
	sf::Vector2f movement(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		movement.x -= speed * deltaTime;
		faceRight = false;

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		movement.x += speed * deltaTime;
		faceRight = true;
	}

	if (movement.x == 0.0f)
		row = 0;
	else row = 1;

	//	if (movement.x > 0.0f)
	//		faceRight = true;
	//	else
	//		faceRight = false;
	//}

	animation.Update(row, deltaTime, faceRight);


	body.setTextureRect(animation.uvRect);
	body.move(movement);
	
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	//{
	//	movement.x -= speed * deltaTime;
	//}
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	//{
	//	movement.x -= speed * deltaTime;
	//}

}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
