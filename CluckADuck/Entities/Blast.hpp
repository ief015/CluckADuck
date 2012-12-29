#pragma once
#include "Entity.hpp"

#include <SFML/Graphics.hpp>

class Blast : public Entity
{
private:
//	static sf::Texture* TEXTURE;
//	sf::Sprite spr;

	static double SHOCKWAVE_SPEED;

	double life;
	double lifeTimeSet;

public:
	Blast(double lifetime);
	virtual ~Blast();
	static bool StaticInit();
	static void StaticQuit();

	double damage;

	inline bool isFinished() const { return life < 0.; }

	virtual void update(float dt);
	virtual void draw(sf::RenderTarget& rt);
};
