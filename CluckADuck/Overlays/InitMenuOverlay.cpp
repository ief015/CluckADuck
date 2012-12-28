#include "InitMenuOverlay.hpp"

#include "../CluckADuck.hpp"
#include "../font.hpp"

#include "../Entities/Powerup.hpp"

InitMenuOverlay::InitMenuOverlay() : Overlay()
{
	this->game = NULL;

	this->overrideControls(true);

	txtMenu = sf::Text(
		"[P] Normal Mode\n"
		"[M] MoO Mode\n"
	, FontRes::getDefaultFont(), 48);

	txtMenu.setOrigin(0.f, txtMenu.getGlobalBounds().height/2.f);
	txtMenu.setColor(sf::Color(225, 180, 0));

	txtInstructions = sf::Text(
		"Shoot the ducks, but don't\n"
		"touch them!\n"
		"\n"
		"Power-Ups:\n"
		"\t+1 Life\n"
		"\t+1 Bomb (Right click to use)\n"
		"\tInvincibility mode (For 15 seconds)\n"
		"\tExtra points\n"
		"\n"
		"Reminder:\n"
		"You only start with off with\n"
		"one extra life and one bomb.\n"
		"Use them wisely!\n"
	, FontRes::getDefaultFont(), 18);
	txtInstructions.setColor(sf::Color(225, 160, 100));

	txtTitle = sf::Text("Cluck-A-Duck", FontRes::getDefaultFont(), 72);
	txtTitle.setStyle(sf::Text::Bold);
	txtTitle.setColor(sf::Color(255, 235, 0));
	txtTitle.setOrigin(txtTitle.getGlobalBounds().width/2.f, txtTitle.getGlobalBounds().height/2.f);

	txtSubTitle = sf::Text("Approximately Nine of Them Edition", FontRes::getDefaultFont(), 32);
	txtSubTitle.setColor(sf::Color(255, 225, 0));
	txtSubTitle.setOrigin(txtSubTitle.getGlobalBounds().width/2.f, txtSubTitle.getGlobalBounds().height/2.f);
}

InitMenuOverlay::~InitMenuOverlay()
{
}


void InitMenuOverlay::onShown()
{

}

void InitMenuOverlay::onHide()
{
	game->initGame((CluckADuck::GAME_MODE)gamemode);
}


void InitMenuOverlay::onUpdate(float dt)
{
}

void InitMenuOverlay::onDraw(sf::RenderTarget& rt)
{
	sf::RectangleShape rect(sf::Vector2f(static_cast<float>(rt.getSize().x), static_cast<float>(rt.getSize().y)));
	rect.setFillColor(sf::Color(0, 0, 0, 180));
	rt.draw(rect);

	txtTitle.setPosition(rt.getSize().x/2.f, 32.f);
	txtSubTitle.setPosition(rt.getSize().x/2.f, 100.f);
	rt.draw(txtTitle);
	rt.draw(txtSubTitle);

	txtInstructions.setPosition(rt.getSize().x - 350.f, rt.getSize().y/2.f + 24.f);
	rt.draw(txtInstructions);

	sprIcon.setTexture(*Powerup::TEXTURE_LIFE);
	sprIcon.setPosition(rt.getSize().x - 354.f, rt.getSize().y/2.f + 108.f);
	rt.draw(sprIcon);

	sprIcon.setTexture(*Powerup::TEXTURE_BOMB);
	sprIcon.setPosition(rt.getSize().x - 354.f, rt.getSize().y/2.f + 128.f);
	rt.draw(sprIcon);

	sprIcon.setTexture(*Powerup::TEXTURE_INV);
	sprIcon.setPosition(rt.getSize().x - 354.f, rt.getSize().y/2.f + 148.f);
	rt.draw(sprIcon);

	sprIcon.setTexture(*Powerup::TEXTURE_POINTS);
	sprIcon.setPosition(rt.getSize().x - 354.f, rt.getSize().y/2.f + 168.f);
	rt.draw(sprIcon);

	txtMenu.setPosition(12.f, rt.getSize().y/2.f + 72.f);
	rt.draw(txtMenu);
}


void InitMenuOverlay::onMouseDown(int x, int y, unsigned b)
{
}

void InitMenuOverlay::onMouseUp(int x, int y, unsigned b)
{
}

void InitMenuOverlay::onMouseWheel(int x, int y, int d)
{
}

void InitMenuOverlay::onMouseMove(int x, int y, int dx, int dy)
{
}

void InitMenuOverlay::onKeyDown(int key)
{
	switch (key)
	{
	case sf::Keyboard::P:
		this->gamemode = CluckADuck::MODE_NORMAL;
		this->close();
		break;
	case sf::Keyboard::M:
		this->gamemode = CluckADuck::MODE_MOO;
		this->close();
		break;
	}
}

void InitMenuOverlay::onKeyUp(int key)
{
}