#include "Bullet.hpp"

#include <math.h>

sf::Texture* Bullet::TEXTURE = NULL;
sf::Sprite Bullet::SPRITE = sf::Sprite();
bool Bullet::StaticInit()
{
	TEXTURE = new sf::Texture();

	if (!TEXTURE->loadFromFile("res/bullet.png"))
		return false;

	TEXTURE->setSmooth(true);
	TEXTURE->setRepeated(false);

	SPRITE.setOrigin(TEXTURE->getSize().x/2.f, TEXTURE->getSize().x/2.f);
	SPRITE.setTexture(*TEXTURE);

	return true;
}

void Bullet::StaticQuit()
{
	delete TEXTURE;
}


Bullet::Bullet() : Entity()
{
	this->boundsRadius = 6./2;
	this->damage = 0.0;
}

Bullet::~Bullet()
{
}


void Bullet::update(float dt)
{
	// Update bullet position.
	Entity::update(dt);
}

void Bullet::draw(sf::RenderTarget& rt)
{
	// Draw bullet.
	SPRITE.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
	rt.draw(SPRITE);
}
