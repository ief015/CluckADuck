#pragma once

#include "Overlay.hpp"
#include "UI/NameTextbox.hpp"
#include "../Highscores.hpp"

#include <SFML/Graphics.hpp>

class CluckADuck;
class GameOverOverlay : public Overlay
{
private:
	sf::Text txtGameOver;
	sf::Text txtPressEnter;
	sf::Text txtPressEsc;
	sf::Text txtPressEscScoreboard;
	sf::Text txtStats;

	NameTextbox* nameTextbox;

	sf::Text txtScoreboardHeader;
	sf::Text txtScoreboardEntry;
	Highscores::Score* myScore;
	bool showHighscore;

public:
	CluckADuck* game;
	GameOverOverlay(CluckADuck* g);
	virtual ~GameOverOverlay();

	virtual void onShown();
	virtual void onHide();

	virtual void onUpdate(float dt);
	virtual void onDraw(sf::RenderTarget& rt);

	virtual void onMouseDown(int x, int y, unsigned b);
	virtual void onMouseUp(int x, int y, unsigned b);
	virtual void onMouseWheel(int x, int y, int d);
	virtual void onMouseMove(int x, int y, int dx, int dy);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);
	virtual void onKeyText(int ch);
};