#pragma once
#include "App.hpp"

#include "font.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

#include "Overlays/OverlayManager.hpp"
#include "Overlays/PauseOverlay.hpp"
#include "Overlays/GameOverOverlay.hpp"
#include "Overlays/InitMenuOverlay.hpp"

#include "Entities/Player.hpp"
#include "Entities/Bullet.hpp"
#include "Entities/Ducky.hpp"
#include "Entities/Powerup.hpp"
#include "Entities/Blast.hpp"

class CluckADuck : public App
{
private:
	OverlayManager* overlay;
	PauseOverlay* overlayPause;
	GameOverOverlay* overlayGameOver;
	InitMenuOverlay* overlayInitMenu;

	int scrW, scrH;

	char debugMode;
	sf::Text txtDebug;

protected:
	virtual bool onInitialize(unsigned w, unsigned h);
	virtual void onTerminate();
	virtual void onTick(float ms);
	virtual void onDrawSF();
	virtual void onDrawGL();

	virtual void onMouseDown(int x, int y, unsigned b);
	virtual void onMouseUp(int x, int y, unsigned b);
	virtual void onMouseWheel(int x, int y, int d);
	virtual void onMouseMove(int x, int y, int dx, int dy);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);
	virtual void onWindowResize(unsigned w, unsigned h);
	virtual void onWindowClose();

public:
	CluckADuck();
	virtual ~CluckADuck();
	

	// Game data

	Player* player;
	std::vector<Ducky*> ducks;
	std::vector<Bullet*> bullets;
	std::vector<Powerup*> items;
	std::vector<Blast*> explosions;

	double gameTime;
	double elapsedTime;

	unsigned long score;

	unsigned int duckNormalCount;
	unsigned int duckBossCount;

	unsigned int level;
	unsigned int nextlvlscore;

	unsigned int usedExtraLives;
	unsigned int usedInv;
	unsigned int usedBombs;

	double duckSpawnRate;
	double nextDuckSpawn;


	// Resources

	sf::Text txtScore;
	sf::Text txtLevel;
	sf::Text txtTime;
	sf::Text txtLives;
	sf::Text txtInvincibilityTimer;
	sf::Text txtIndicator;

	sf::Texture texBombIcon;
	sf::Texture texPlayerIcon;
	sf::Sprite sprBombIcon;
	sf::Sprite sprPlayerIcon;

	sf::Music msc;

	sf::SoundBuffer* sndbufRubberDuck;
	sf::SoundBuffer* sndbufPickUp;
	sf::SoundBuffer* sndbufExplosion;
	sf::Sound sndRubberDuck;
	sf::Sound sndPickUp;
	sf::Sound sndExplosion;


	// Game mode data, functions and events.

	enum GAME_MODE
	{
		MODE_NORMAL = 0,
		MODE_MOO
	};
	int gamemode;

	struct Indicator
	{
		static const float LIFETIME;

		std::string msg;

		float life;
		sf::Color col;
		vec2 pos;
	};
	std::vector<Indicator*> indicators;
	Indicator* addIndicator(const char* msg, const vec2& pos, unsigned char r, unsigned char g, unsigned char b);
	Blast* addBlast(const vec2& pos, double lifetime = 300., double damage = 10.);

	void showMainMenu();
	void showGameOverMenu();
	void showPauseMenu();

	void initGame(GAME_MODE mode);
	void cleanupWorld();
	void gameOver();

	bool isPaused();

	void gmInit();
	void gmTick(float dt);
	void gmDraw(sf::RenderTarget& rt);

	void gmAddScore(int add);

	void gmDuckSpawned(Ducky* duck);
	void gmDuckKilled(Ducky* duck);
	void gmDuckHitPlayer(Ducky* duck);
	void gmPlayerHitDuck(Ducky* duck, Bullet* bullet);
	void gmPlayerExplodeDuck(Ducky* duck, Blast* blast);
	void gmPlayerShoot(Bullet* bullet);
	void gmPlayerDetonateBomb(Blast* blast);
	void gmPlayerPickUpPowerup(Powerup* powerup);
};
