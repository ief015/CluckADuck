#pragma once
#include "Entity.hpp"

#include <SFML/Graphics.hpp>

class CluckADuck;

struct PowerupData_Points
{
	int points;
};

class Powerup : public Entity
{
public: enum POWERUP_TYPE
	{
		TYPE_UNKNOWN = 0,
		TYPE_BOMB,
		TYPE_EXPROUNDS,
		TYPE_INV,
		TYPE_LIFE,
		TYPE_POINTS
	};

	static sf::Texture* TEXTURE_BOMB;
	static sf::Texture* TEXTURE_EXPROUNDS;
	static sf::Texture* TEXTURE_INV;
	static sf::Texture* TEXTURE_LIFE;
	static sf::Texture* TEXTURE_POINTS;

private:
	sf::Sprite spr;
	bool isSetUp;
	CluckADuck* game;

public:
	Powerup();
	virtual ~Powerup();
	static bool StaticInit();
	static void StaticQuit();

	void setupBombPowerup(CluckADuck* g);
	void setupExpRoundsPowerup(CluckADuck* g);
	void setupInvPowerup(CluckADuck* g);
	void setupLifePowerup(CluckADuck* g);
	void setupPointsPowerup(CluckADuck* g, int score);

	void use();

	POWERUP_TYPE powerType;
	void* powerData;

	static double LIFETIME;
	double life;

	inline bool expired() const { return life <= 0.; }
	double blinkingTime;
	bool blink;

	virtual void update(float dt);
	virtual void draw(sf::RenderTarget& rt);
};
