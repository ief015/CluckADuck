#pragma once

#include "Overlay.hpp"
#include "../FontRes.hpp"

#include <SFML/Graphics.hpp>

class PauseOverlay : public Overlay
{
private:
	sf::Text txtPause;
	sf::Text txtPauseMsg;

public:
	PauseOverlay() : Overlay()
	{
		this->overrideControls(true);

		txtPause = sf::Text("Paused", FontRes::getArcadeFont(), 72);
		txtPause.setOrigin(txtPause.getGlobalBounds().width/2.f, txtPause.getGlobalBounds().height);
		txtPause.setColor(sf::Color(225, 180, 0));

		txtPauseMsg = sf::Text("Press ESCAPE to resume.", FontRes::getHandWritingFont(), 24);
		txtPauseMsg.setOrigin(txtPauseMsg.getGlobalBounds().width/2.f, txtPauseMsg.getGlobalBounds().height/2.f);
		txtPauseMsg.setColor(sf::Color(225, 120, 0));
	}
	virtual ~PauseOverlay() {}

	virtual void onShown() {}
	virtual void onHide() {}

	virtual void onUpdate(float dt) {}
	virtual void onDraw(sf::RenderTarget& rt)
	{
		sf::RectangleShape rect(sf::Vector2f(static_cast<float>(rt.getSize().x), static_cast<float>(rt.getSize().y)));
		rect.setFillColor(sf::Color(0, 0, 0, 180));
		rt.draw(rect);

		txtPause.setPosition(rt.getSize().x/2.f, rt.getSize().y/2.f);
		rt.draw(txtPause);

		txtPauseMsg.setPosition(rt.getSize().x/2.f, rt.getSize().y/2.f + 48.f);
		rt.draw(txtPauseMsg);
	}

	virtual void onMouseDown(int x, int y, unsigned b) { }
	virtual void onMouseUp(int x, int y, unsigned b) { }
	virtual void onMouseWheel(int x, int y, int d) { }
	virtual void onMouseMove(int x, int y, int dx, int dy) { }
	virtual void onKeyDown(int key)
	{
		switch (key)
		{
		case sf::Keyboard::Escape:
			this->close();
			break;
		}
	}
	virtual void onKeyUp(int key) { }
	virtual void onKeyText(int ch) { }
};