#include "InitMenuOverlay.hpp"

#include "../CluckADuck.hpp"
#include "../FontRes.hpp"

#include "../Entities/Powerup.hpp"

InitMenuOverlay::InitMenuOverlay(CluckADuck* g) : Overlay()
{
	game = g;
	this->overrideControls(true);

	// Set up UI widgets.
	btnNormalMode = new ClickableText();
	btnNormalMode->text.setFont(FontRes::getArcadeFont());
	btnNormalMode->text.setCharacterSize(32);
	btnNormalMode->setString("Normal Mode");
	btnNormalMode->setColourIdle(sf::Color(255, 180, 0));
	btnNormalMode->setColourHover(sf::Color(255, 255, 150));
	btnNormalMode->setColourClick(sf::Color(255, 255, 200));
	ui->addWidget(btnNormalMode);

	btnMooMode = new ClickableText();
	btnMooMode->text.setFont(FontRes::getArcadeFont());
	btnMooMode->text.setCharacterSize(32);
	btnMooMode->setString("Moo Mode");
	btnMooMode->setColourIdle(sf::Color(255, 180, 0));
	btnMooMode->setColourHover(sf::Color(255, 255, 150));
	btnMooMode->setColourClick(sf::Color(255, 255, 200));
	ui->addWidget(btnMooMode);

	// Set up instructions text.
	txtInstructions = sf::Text(
		"Shoot the ducks, but don't\n"
		"touch them! Don't let the time\n"
		"run out in Normal Mode!\n"
		"\n"
		"Power-Ups:\n"
		"\t+1 Life\n"
		"\t+1 Bomb (Right click to use)\n"
		"\tExplosive rounds (For 15 seconds)\n"
		"\tInvincibility mode (For 15 seconds)\n"
		"\tExtra points\n"
		"\n"
		"Reminder:\n"
		"You only start with off with\n"
		"one extra life and one bomb.\n"
		"Use them wisely!\n"
		, FontRes::getHandWritingFont(), 16);
	txtInstructions.setColor(sf::Color(225, 160, 100));

	// Set up title text.
	txtTitle = sf::Text("Cluck-A-Duck", FontRes::getTitleFont(), 72);
	txtTitle.setStyle(sf::Text::Bold);
	txtTitle.setColor(sf::Color(255, 235, 0));
	txtTitle.setOrigin(txtTitle.getGlobalBounds().width/2.f, txtTitle.getGlobalBounds().height/2.f);

	// Set up sub title text.
	txtSubTitle = sf::Text("Approximately nine of them edition", FontRes::getTitleFont(), 32);
	txtSubTitle.setColor(sf::Color(255, 225, 0));
	txtSubTitle.setOrigin(txtSubTitle.getGlobalBounds().width/2.f, txtSubTitle.getGlobalBounds().height/2.f);
}

InitMenuOverlay::~InitMenuOverlay()
{
	delete btnNormalMode;
	delete btnMooMode;
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
	// Update widgets.
	Overlay::onUpdate(dt);

	if (btnNormalMode->pollClick())
	{
		this->gamemode = CluckADuck::MODE_NORMAL;
		this->close();
	}

	if (btnMooMode->pollClick())
	{
		this->gamemode = CluckADuck::MODE_MOO;
		this->close();
	}
	
}

void InitMenuOverlay::onDraw(sf::RenderTarget& rt)
{
	// Draw backdrop.
	sf::RectangleShape rect(sf::Vector2f(static_cast<float>(rt.getSize().x), static_cast<float>(rt.getSize().y)));
	rect.setFillColor(sf::Color(0, 0, 0, 180));
	rt.draw(rect);

	// Draw title and subtitle text.
	txtTitle.setPosition(rt.getSize().x/2.f, 32.f);
	txtSubTitle.setPosition(rt.getSize().x/2.f, 100.f);
	rt.draw(txtTitle);
	rt.draw(txtSubTitle);

	// Draw instructions text.
	txtInstructions.setPosition(rt.getSize().x - 350.f, rt.getSize().y/2.f - 128.f);
	rt.draw(txtInstructions);

	// Draw the powerup icons.
	sprIcon.setTexture(*Powerup::TEXTURE_LIFE);
	sprIcon.setPosition(rt.getSize().x - 354.f, rt.getSize().y/2.f + 10.f);
	rt.draw(sprIcon);

	sprIcon.setTexture(*Powerup::TEXTURE_BOMB);
	sprIcon.setPosition(rt.getSize().x - 354.f, rt.getSize().y/2.f + 38.f);
	rt.draw(sprIcon);

	sprIcon.setTexture(*Powerup::TEXTURE_EXPROUNDS);
	sprIcon.setPosition(rt.getSize().x - 354.f, rt.getSize().y/2.f + 66.f);
	rt.draw(sprIcon);

	sprIcon.setTexture(*Powerup::TEXTURE_INV);
	sprIcon.setPosition(rt.getSize().x - 354.f, rt.getSize().y/2.f + 94.f);
	rt.draw(sprIcon);

	sprIcon.setTexture(*Powerup::TEXTURE_POINTS);
	sprIcon.setPosition(rt.getSize().x - 354.f, rt.getSize().y/2.f + 122.f);
	rt.draw(sprIcon);

	// Draw UI widgets.
	btnNormalMode->x = 10.;
	btnNormalMode->y = rt.getSize().y/2.;
	btnMooMode->x = 10.;
	btnMooMode->y = rt.getSize().y/2. + 40.;
	Overlay::onDraw(rt);
}


void InitMenuOverlay::onMouseDown(int x, int y, unsigned b)
{
	Overlay::onMouseDown(x, y, b);
}

void InitMenuOverlay::onMouseUp(int x, int y, unsigned b)
{
	Overlay::onMouseUp(x, y, b);
}

void InitMenuOverlay::onMouseWheel(int x, int y, int d)
{
	Overlay::onMouseWheel(x, y, d);
}

void InitMenuOverlay::onMouseMove(int x, int y, int dx, int dy)
{
	if (dx == 0 && dy == 0)
		return;

	Overlay::onMouseMove(x, y, dx, dy);
}

void InitMenuOverlay::onKeyDown(int key)
{
	Overlay::onKeyDown(key);
}

void InitMenuOverlay::onKeyUp(int key)
{
	Overlay::onKeyUp(key);
}

void InitMenuOverlay::onKeyText(int ch)
{
	Overlay::onKeyUp(ch);
}
