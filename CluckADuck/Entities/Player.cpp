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

	// Default minimal properties.
	lives = 0;
	bombCount = 0;
	invincibility = false;
	invincibilityTime = 0.;
	invincibilityTimeSet = 0.;
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
	if (nextBullet < fireRate)
		return NULL;

	// Create new bullet.
	Bullet* b = new Bullet();

	// Set position and velocity based on where the
	// player is and direction player is looking.
	double ang = atan2(pos.y - mousey, pos.x - mousex);
	b->setPos(pos.x + -cos(ang)*boundsRadius, pos.y + -sin(ang)*boundsRadius);
	b->setVel(-cos(ang)*bulletSpeed, -sin(ang)*bulletSpeed);

	// Give the bullet some damage to it.
	b->damage = bulletDamage;

	// Reset cooldown.
	nextBullet = 0.;

	return b;
}

void Player::giveInvincibility(double time)
{
	// Give player invincibility. Time does not stack.
	invincibility = true;
	invincibilityTime = time;
	invincibilityTimeSet = time;
}

void Player::update(float dt)
{
	// Accelerate player.
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

	// Limit speed.
	double len;
	vec2 norm = vec2Normalize(vel, &len);
	if (len > maxMoveSpeed)
	{
		vel.x = norm.x * maxMoveSpeed;
		vel.y = norm.y * maxMoveSpeed;
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
	nextBullet += dt;
	
	// Update position.
	Entity::update(dt);
}

void Player::draw(sf::RenderTarget& rt)
{
	// Draw player.
	spr.setPosition(pos.x, pos.y);
	spr.setRotation(static_cast<float>(atan2(pos.y-mousey, pos.x-mousex) / 3.141592653589793238462643383279502884 * 180.)-90.);
	rt.draw(spr);
}
