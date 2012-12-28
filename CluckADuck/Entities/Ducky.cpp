#include "Ducky.hpp"
#include "../utils.hpp"

#include <math.h>

sf::Texture* Ducky::TEXTURE = NULL;
sf::Sound* Ducky::SND_QUACK1;
sf::Sound* Ducky::SND_QUACK2;
sf::Sound* Ducky::SND_QUACK3;
sf::Sound* Ducky::SND_QUACK4;
sf::SoundBuffer* Ducky::SNDBUF_QUACK1;
sf::SoundBuffer* Ducky::SNDBUF_QUACK2;
sf::SoundBuffer* Ducky::SNDBUF_QUACK3;
sf::SoundBuffer* Ducky::SNDBUF_QUACK4;
bool Ducky::StaticInit()
{
	TEXTURE = new sf::Texture();
	SND_QUACK1 = new sf::Sound();
	SND_QUACK2 = new sf::Sound();
	SND_QUACK3 = new sf::Sound();
	SND_QUACK4 = new sf::Sound();
	SNDBUF_QUACK1 = new sf::SoundBuffer();
	SNDBUF_QUACK2 = new sf::SoundBuffer();
	SNDBUF_QUACK3 = new sf::SoundBuffer();
	SNDBUF_QUACK4 = new sf::SoundBuffer();

	// Load ducky texture.
	if (!TEXTURE->loadFromFile("res/ducky.png"))
		return false;

	// Load quacking sounds.
	if (!SNDBUF_QUACK1->loadFromFile("res/quack1.wav"))
		return false;
	if (!SNDBUF_QUACK2->loadFromFile("res/quack2.wav"))
		return false;
	if (!SNDBUF_QUACK3->loadFromFile("res/quack3.wav"))
		return false;
	if (!SNDBUF_QUACK4->loadFromFile("res/quack4.wav"))
		return false;

	SND_QUACK1->setBuffer(*SNDBUF_QUACK1);
	SND_QUACK2->setBuffer(*SNDBUF_QUACK2);
	SND_QUACK3->setBuffer(*SNDBUF_QUACK3);
	SND_QUACK4->setBuffer(*SNDBUF_QUACK4);

	TEXTURE->setSmooth(true);
	TEXTURE->setRepeated(false);

	return true;
}

void Ducky::StaticQuit()
{
	delete TEXTURE;

	delete SND_QUACK1;
	delete SND_QUACK2;
	delete SND_QUACK3;
	delete SND_QUACK4;
	delete SNDBUF_QUACK1;
	delete SNDBUF_QUACK2;
	delete SNDBUF_QUACK3;
	delete SNDBUF_QUACK4;
}


Ducky::Ducky() : Entity()
{
	this->setScale(1.f);
	this->enteringField = true;
	this->typeChar = 0;

	spr.setOrigin(TEXTURE->getSize().x/2.f, TEXTURE->getSize().x/2.f);
	spr.setTexture(*TEXTURE);
}

Ducky::~Ducky()
{
}


void Ducky::setScale(double scl)
{
	// Scale up the bound radius.
	this->boundsRadius = 48./2 * scl;
	this->scale = scl;
}

void Ducky::quack()
{
	// Choose a random quack sound to play.

	int s = randomiinc(1,4);

	switch(s)
	{
	case 1:
		SND_QUACK1->play();
		break;
	case 2:
		SND_QUACK2->play();
		break;
	case 3:
		SND_QUACK3->play();
		break;
	case 4:
		SND_QUACK4->play();
		break;
	}
}

void Ducky::update(float dt)
{
	// Update position.
	Entity::update(dt);
}

void Ducky::draw(sf::RenderTarget& rt)
{
	// Set up transformations.
	spr.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
	spr.setRotation(static_cast<float>(atan2(vel.y, vel.x) / 3.141592653589793238462643383279502884 * 180.));
	spr.setScale(static_cast<float>(this->scale), static_cast<float>((vel.x>=0.)?this->scale:-this->scale));

	// Reddish colour indicates damage.
	char reddish = static_cast<unsigned char>(this->health/this->maxHealth*255);
	spr.setColor(sf::Color(255,reddish,reddish));

	// Draw duck.
	rt.draw(spr);
}
