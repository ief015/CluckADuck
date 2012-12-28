#pragma once
#include "Entity.hpp"

#include <SFML/Graphics.hpp>

class Bullet : public Entity
{
private:
	static sf::Texture* TEXTURE;
	static sf::Sprite SPRITE;

public:
	Bullet();
	virtual ~Bullet();
	static bool StaticInit();
	static void StaticQuit();

	double damage;

	virtual void update(float dt);
	virtual void draw(sf::RenderTarget& rt);
};
