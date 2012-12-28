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
	input = 0;

	boundsRadius = 24./2;

	spr.setOrigin(TEXTURE->getSize().x/2.f, TEXTURE->getSize().x/2.f);
	spr.setTexture(*TEXTURE);

	accelerationSpeed = 0.15;
	slowdownSpeed = 0.08;
	maxMoveSpeed = 2.4;
	dodgeSpeed = 5.2;
	maxSpeed = 6.;

	lives = 0; // No extra lives.

	bombCount = 0;
	invincibility = false;
	invincibilityTime = 0.;
	invincibilityTimeSet = 0.;

	dodgeRate = 1000.; // 1 second
	nextDodge = dodgeRate;
}

Player::~Player()
{
}


void Player::onInput(PLAYER_INPUT in)
{
	switch(in)
	{
	case INPUT_MOVE_DODGE: {

		if (nextDodge < dodgeRate)
			break;

		vec2 dir = vec2New();

		bool testMovement = false;
		if (this->checkInput(INPUT_MOVE_UP))
		{
			dir.y -= 1.;
			testMovement = true;
		}
		if (this->checkInput(INPUT_MOVE_DOWN))
		{
			dir.y += 1.;
			testMovement = true;
		}
		if (this->checkInput(INPUT_MOVE_LEFT))
		{
			dir.x -= 1.;
			testMovement = true;
		}
		if (this->checkInput(INPUT_MOVE_RIGHT))
		{
			dir.x += 1.;
			testMovement = true;
		}

		if (!testMovement)
			break;

		dir = vec2Normalize(dir);

		vel.x += dodgeSpeed*dir.x;
		vel.y += dodgeSpeed*dir.y;

		nextDodge = 0.;

		break; }
	}
}

Bullet* Player::fireWeapon()
{
	if (nextBullet < fireRate)
		return NULL;

	Bullet* b = new Bullet();

	double ang = atan2(pos.y - mousey, pos.x - mousex);

	b->setPos(pos.x + -cos(ang)*boundsRadius, pos.y + -sin(ang)*boundsRadius);
	b->setVel(-cos(ang)*bulletSpeed, -sin(ang)*bulletSpeed);
	b->damage = bulletDamage;

	nextBullet = 0.;

	return b;
}

void Player::giveInvincibility(double time)
{
	invincibility = true;
	invincibilityTime = time;
	invincibilityTimeSet = time;
}

void Player::update(float dt)
{
	double len;
	vec2 norm = vec2Normalize(vel, &len);

	// Accelerate player.
	if (len <= maxMoveSpeed)
	{
		if (this->checkInput(INPUT_MOVE_UP))
		{
			vel.y -= accelerationSpeed;
		}
		if (this->checkInput(INPUT_MOVE_DOWN))
		{
			vel.y += accelerationSpeed;
		}
		if (this->checkInput(INPUT_MOVE_LEFT))
		{
			vel.x -= accelerationSpeed;
		}
		if (this->checkInput(INPUT_MOVE_RIGHT))
		{
			vel.x += accelerationSpeed;
		}
	}

	// Limit speed.
	norm = vec2Normalize(vel, &len);

	if (len > maxSpeed)
	{
		vel.x = norm.x*maxSpeed;
		vel.y = norm.y*maxSpeed;
	}

	// Slow down.
	if (vel.x > 0.)
	{
		vel.x -= norm.x * slowdownSpeed;
		if (vel.x < 0.)
			vel.x = 0.;
	}
	else if (vel.x < 0.)
	{
		vel.x -= norm.x * slowdownSpeed;
		if (vel.x > 0.)
			vel.x = 0.;
	}
	if (vel.y > 0.)
	{
		vel.y -= norm.y * slowdownSpeed;
		if (vel.y < 0.)
			vel.y = 0.;
	}
	else if (vel.y < 0.)
	{
		vel.y -= norm.y * slowdownSpeed;
		if (vel.y > 0.)
			vel.y = 0.;
	}

	// Update invincibility powerup.
	if (invincibility)
	{
		// Decrease invincibility timer.
		invincibilityTime -= dt;

		// Are we still invincible?
		if (invincibilityTime <= 0)
			invincibility = false;
	}

	// Increase timers.
	nextDodge += dt;
	nextBullet += dt;
	
	// Update position.
	Entity::update(dt);
}

void Player::draw(sf::RenderTarget& rt)
{
	spr.setPosition(pos.x, pos.y);
	spr.setRotation(static_cast<float>(atan2(pos.y-mousey, pos.x-mousex) / 3.141592653589793238462643383279502884 * 180.)-90.);

	rt.draw(spr);
}
