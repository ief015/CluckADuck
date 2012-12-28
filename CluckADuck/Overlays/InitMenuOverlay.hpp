#pragma once

#include "Overlay.hpp"

#include <SFML/Graphics.hpp>

class CluckADuck;
class InitMenuOverlay : public Overlay
{
private:
	sf::Text txtTitle;
	sf::Text txtSubTitle;

	sf::Text txtMenu;
	sf::Text txtInstructions;

	sf::Sprite sprIcon;

public:
	CluckADuck* game;
	int gamemode;
	InitMenuOverlay(CluckADuck* g);
	virtual ~InitMenuOverlay();

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
};
