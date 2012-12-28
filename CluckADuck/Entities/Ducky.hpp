#pragma once
#include "Entity.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Ducky : public Entity
{
private:
	static sf::Texture* TEXTURE;
	sf::Sprite spr;

	double scale;

	static sf::Sound* SND_QUACK1;
	static sf::Sound* SND_QUACK2;
	static sf::Sound* SND_QUACK3;
	static sf::Sound* SND_QUACK4;
	static sf::SoundBuffer* SNDBUF_QUACK1;
	static sf::SoundBuffer* SNDBUF_QUACK2;
	static sf::SoundBuffer* SNDBUF_QUACK3;
	static sf::SoundBuffer* SNDBUF_QUACK4;

public:
	Ducky();
	virtual ~Ducky();
	static bool StaticInit();
	static void StaticQuit();

	void setScale(double scl);

	bool enteringField;

	double maxHealth;
	double health;

	int score;
	int typeChar;

	void quack();

	virtual void update(float dt);
	virtual void draw(sf::RenderTarget& rt);
};
