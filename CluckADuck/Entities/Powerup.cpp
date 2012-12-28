#include "Powerup.hpp"
#include "../CluckADuck.hpp"

double Powerup::LIFETIME = 15000.; // 15 seconds

sf::Texture* Powerup::TEXTURE_BOMB = NULL;
sf::Texture* Powerup::TEXTURE_INV = NULL;
sf::Texture* Powerup::TEXTURE_LIFE = NULL;
sf::Texture* Powerup::TEXTURE_POINTS = NULL;
bool Powerup::StaticInit()
{
	TEXTURE_BOMB = new sf::Texture();
	TEXTURE_INV = new sf::Texture();
	TEXTURE_LIFE = new sf::Texture();
	TEXTURE_POINTS = new sf::Texture();

	if (!TEXTURE_BOMB->loadFromFile("res/pwuBomb.png"))
		return false;
	if (!TEXTURE_INV->loadFromFile("res/pwuInv.png"))
		return false;
	if (!TEXTURE_LIFE->loadFromFile("res/pwuLife.png"))
		return false;
	if (!TEXTURE_POINTS->loadFromFile("res/pwuPoints.png"))
		return false;

	TEXTURE_BOMB->setSmooth(true);
	TEXTURE_BOMB->setRepeated(false);

	TEXTURE_INV->setSmooth(true);
	TEXTURE_INV->setRepeated(false);

	TEXTURE_LIFE->setSmooth(true);
	TEXTURE_LIFE->setRepeated(false);

	TEXTURE_POINTS->setSmooth(true);
	TEXTURE_POINTS->setRepeated(false);

	return true;
}

void Powerup::StaticQuit()
{
	delete TEXTURE_BOMB;
	delete TEXTURE_INV;
	delete TEXTURE_LIFE;
	delete TEXTURE_POINTS;
}


Powerup::Powerup() : Entity()
{
	boundsRadius = 24./2;
	powerData = NULL;
	powerType = TYPE_UNKNOWN;
	this->life = LIFETIME;
	this->blinkingTime = 0.;
	blink = false;
	isSetUp = false;
}

Powerup::~Powerup()
{
	if (powerData)
		delete powerData;
}


void Powerup::setupBombPowerup(CluckADuck* g)
{
	// Set this as a +1 Bomb powerup.

	this->game = g;
	this->powerType = TYPE_BOMB;

	spr.setTexture(*TEXTURE_BOMB);
	spr.setOrigin(spr.getTexture()->getSize().x/2.f, spr.getTexture()->getSize().x/2.f);

	isSetUp = true;
}

void Powerup::setupInvPowerup(CluckADuck* g)
{
	// Set this as an Invincible powerup.

	this->game = g;
	this->powerType = TYPE_INV;

	spr.setTexture(*TEXTURE_INV);
	spr.setOrigin(spr.getTexture()->getSize().x/2.f, spr.getTexture()->getSize().x/2.f);

	isSetUp = true;
}

void Powerup::setupLifePowerup(CluckADuck* g)
{
	// Set this as a +1 Life powerup.

	this->game = g;
	this->powerType = TYPE_LIFE;

	spr.setTexture(*TEXTURE_LIFE);
	spr.setOrigin(spr.getTexture()->getSize().x/2.f, spr.getTexture()->getSize().x/2.f);

	isSetUp = true;
}

void Powerup::setupPointsPowerup(CluckADuck* g, int score)
{
	// Set this as a Points powerup.

	this->game = g;
	this->powerType = TYPE_POINTS;

	spr.setTexture(*TEXTURE_POINTS);
	spr.setOrigin(spr.getTexture()->getSize().x/2.f, spr.getTexture()->getSize().x/2.f);

	powerData = new PowerupData_Points();
	((PowerupData_Points*)powerData)->points = score;

	isSetUp = true;
}


void Powerup::use()
{
	// Is this powerup ready to be used?
	if (!isSetUp)
		return;

	// Use powerup.
	switch (powerType)
	{
	case TYPE_BOMB:
		++game->player->bombCount; // Give a bomb.
		break;

	case TYPE_INV:
		game->player->giveInvincibility(15000.); // 15 seconds.
		break;

	case TYPE_LIFE:
		++game->player->lives; // Give a life.
		break;

	case TYPE_POINTS:
		// Add some points to score.
		game->gmAddScore(((PowerupData_Points*)powerData)->points);
		break;
	}
}


void Powerup::update(float dt)
{
	// Check if expired.
	if (this->expired())
		return;

	// Decrease lifetime.
	life -= dt;

	// Blink if under 5 seconds.
	if (life < 5000.)
	{
		blinkingTime += dt;

		if (blinkingTime > 250.)
		{
			blink = !blink;
			blinkingTime = 0.;
		}
	}

	Entity::update(dt);
}

void Powerup::draw(sf::RenderTarget& rt)
{
	if (blink)
		return;

	spr.setPosition(pos.x, pos.y);
	rt.draw(spr);
}
