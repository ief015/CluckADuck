#pragma once
#include "Entity.hpp"
#include "Bullet.hpp"

#include <SFML/Graphics.hpp>

class Player : public Entity
{
public: enum PLAYER_INPUT
	{
		INPUT_MOVE_UP = 0,
		INPUT_MOVE_DOWN,
		INPUT_MOVE_LEFT,
		INPUT_MOVE_RIGHT,
		INPUT_GUN_FIRE
	};

private:
	static sf::Texture* TEXTURE;
	sf::Sprite spr;

	int input;

protected:
	int mousex, mousey;
	void onInput(PLAYER_INPUT in);

public:
	Player();
	virtual ~Player();
	static bool StaticInit();
	static void StaticQuit();

	inline void clearInput() { input = 0; }
	inline void startInput(PLAYER_INPUT in) { input = input | (1<<in); this->onInput(in); }
	inline void endInput(PLAYER_INPUT in) { input = input & ~(1<<in); }
	inline bool checkInput(PLAYER_INPUT in) const { return (input & (1<<in)) != 0; }

	virtual void update(float dt);
	virtual void draw(sf::RenderTarget& rt);

	double accelerationSpeed;
	double maxMoveSpeed;
	double slowdownSpeed;

	double fireRate;
	double nextBullet;
	double bulletSpeed;
	double bulletDamage;
	Bullet* fireWeapon();

	int lives;

	void giveInvincibility(double time);
	bool invincibility;
	double invincibilityTime;
	double invincibilityTimeSet;
	unsigned int bombCount;

	inline void updateMousePos(int x, int y) { mousex = x; mousey = y; }

};
