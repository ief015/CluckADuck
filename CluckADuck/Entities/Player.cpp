#include "Player.hpp"

#include <math.h>

sf::Texture* Player::TEXTURE = NULL;
bool Player::StaticInit()
{
	TEXTURE = new sf::Texture();

	if (!TEXTURE->loadFromFile("res/player.png"))
		return false;

	TEXTURE->setSmooth(true);
	TEXTURE->setRepeated(false);

	return true;
}

void Player::StaticQuit()
{
	delete TEXTURE;
}


Player::Player() : Entity()
{
	this->input = 0;

	this->boundsRadius = 24./2;

	spr.setOrigin(TEXTURE->getSize().x/2.f, TEXTURE->getSize().x/2.f);
	spr.setTexture(*TEXTURE);

	this->accelerationSpeed = 0.15;
	this->slowdownSpeed = 0.08;
	this->maxMoveSpeed = 2.4;

	// Default minimal properties.
	this->lives = 0;
	this->bombCount = 0;
	this->invincibility = false;
	this->invincibilityTime = 0.;
	this->invincibilityTimeSet = 0.;
	this->mousex = 0;
	this->mousey = 0;
}

Player::~Player()
{
}


void Player::onInput(PLAYER_INPUT in)
{
}

Bullet* Player::fireWeapon()
{
	// Are we able to fire weapon again yet?
	if (this->nextBullet < this->fireRate)
		return NULL;

	// Create new bullet.
	Bullet* b = new Bullet();

	// Set position and velocity based on where the
	// player is and direction player is looking.
	double ang = atan2(pos.y - this->mousey, pos.x - this->mousex);
	b->setPos(pos.x + -cos(ang) * this->boundsRadius, pos.y + -sin(ang) * this->boundsRadius);
	b->setVel(-cos(ang) * this->bulletSpeed, -sin(ang) * this->bulletSpeed);

	// Give the bullet some damage to it.
	b->damage = this->bulletDamage;

	// Reset cooldown.
	this->nextBullet = 0.;

	return b;
}

void Player::giveInvincibility(double time)
{
	// Give player invincibility. Time does not stack.
	this->invincibility = true;
	this->invincibilityTime = time;
	this->invincibilityTimeSet = time;
}

void Player::update(float dt)
{
	// Accelerate player.
	if (this->checkInput(INPUT_MOVE_UP))
	{
		vel.y -= this->accelerationSpeed;
	}
	if (this->checkInput(INPUT_MOVE_DOWN))
	{
		vel.y += this->accelerationSpeed;
	}
	if (this->checkInput(INPUT_MOVE_LEFT))
	{
		vel.x -= this->accelerationSpeed;
	}
	if (this->checkInput(INPUT_MOVE_RIGHT))
	{
		vel.x += this->accelerationSpeed;
	}

	// Limit speed.
	double len;
	vec2 norm = vec2Normalize(vel, &len);
	if (len > this->maxMoveSpeed)
	{
		vel.x = norm.x * this->maxMoveSpeed;
		vel.y = norm.y * this->maxMoveSpeed;
	}

	// Slow down.
	if (vel.x > 0.)
	{
		vel.x -= norm.x * this->slowdownSpeed;
		if (vel.x < 0.)
			vel.x = 0.;
	}
	else if (vel.x < 0.)
	{
		vel.x -= norm.x * this->slowdownSpeed;
		if (vel.x > 0.)
			vel.x = 0.;
	}
	if (vel.y > 0.)
	{
		vel.y -= norm.y * this->slowdownSpeed;
		if (vel.y < 0.)
			vel.y = 0.;
	}
	else if (vel.y < 0.)
	{
		vel.y -= norm.y * this->slowdownSpeed;
		if (vel.y > 0.)
			vel.y = 0.;
	}

	// Update invincibility powerup.
	if (this->invincibility)
	{
		// Decrease invincibility timer.
		this->invincibilityTime -= dt;

		// Are we still invincible?
		if (this->invincibilityTime <= 0)
			this->invincibility = false;
	}

	// Increase timers.
	this->nextBullet += dt;
	
	// Update position.
	Entity::update(dt);
}

void Player::draw(sf::RenderTarget& rt)
{
	// Draw player.
	spr.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
	spr.setRotation(static_cast<float>(atan2(pos.y - this->mousey, pos.x - this->mousex) / 3.141592653589793238462643383279502884 * 180.)-90.f);
	rt.draw(spr);
}
