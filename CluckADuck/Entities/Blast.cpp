#include "Blast.hpp"


double Blast::SHOCKWAVE_SPEED = 11.;

Blast::Blast(double lifetime) : Entity()
, life(lifetime)
{
	boundsRadius = 0.;
}

Blast::~Blast()
{

}

bool Blast::StaticInit()
{
	return true;
}

void Blast::StaticQuit()
{

}


void Blast::update(float dt)
{
	// Don't update if finished.
	if (this->isFinished())
		return;

	// Increase lifetime and blast radius.
	this->boundsRadius += SHOCKWAVE_SPEED;
	this->life -= dt;

//	Entity::update(dt);
}

void Blast::draw(sf::RenderTarget& rt)
{
	sf::CircleShape c(this->boundsRadius);

	c.setPosition(static_cast<float>(this->pos.x), static_cast<float>(this->pos.y));
	c.setOrigin(static_cast<float>(this->boundsRadius), static_cast<float>(this->boundsRadius));

	c.setOutlineThickness(3.f);
	c.setFillColor(sf::Color::Transparent);
	c.setOutlineColor(sf::Color(255, 255, 255, 150));

	rt.draw(c);
}
