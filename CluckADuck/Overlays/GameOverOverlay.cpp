#include "GameOverOverlay.hpp"

#include "../CluckADuck.hpp"
#include "../font.hpp"

GameOverOverlay::GameOverOverlay(CluckADuck* g) : Overlay()
{
	game = g;

	this->overrideControls(true);

	txtGameOver = sf::Text("Game Over", FontRes::getDefaultFont(), 72);
	txtGameOver.setOrigin(txtGameOver.getGlobalBounds().width/2.f, txtGameOver.getGlobalBounds().height/2.f);
	txtGameOver.setColor(sf::Color(225, 120, 0));

	txtPressEsc = sf::Text("Press ENTER to restart.", FontRes::getDefaultFont(), 24);
	txtPressEsc.setOrigin(txtPressEsc.getGlobalBounds().width/2.f, txtPressEsc.getGlobalBounds().height/2.f);
	txtPressEsc.setColor(sf::Color(225, 120, 0));

	txtStats = sf::Text("", FontRes::getDefaultFont(), 18);
	txtStats.setPosition(10.f, 10.f);
	txtStats.setColor(sf::Color(225, 170, 0));
}

GameOverOverlay::~GameOverOverlay()
{
}


void GameOverOverlay::onShown()
{
	char buf[256];

	double t = game->elapsedTime;
	int hours, mins, secs;

	hours = (int)(t/3600000.);
	t -= hours*3600000.;

	mins = (int)(t/60000.);
	t -= mins*60000.;

	secs = (int)(t/1000.);

	sprintf(buf,

		"Elapsed time - %d:%s%d:%s%d\n"
		"Score - %d\n"
		"\n"
		"Ducks - %d\n"
		"Bosses - %d\n"
		"Total - %d\n"
		"\n"
		"Extra lives used - %d\n"
		"Invincibilities used - %d\n"
		"Bombs used - %d\n"

		, hours, (mins < 10 ? "0" : ""), mins, (secs < 10 ? "0" : ""), secs
		, game->score

		, game->duckNormalCount
		, game->duckBossCount
		, game->duckNormalCount + game->duckBossCount
		
		, game->usedExtraLives
		, game->usedInv
		, game->usedBombs
	);

	txtStats.setString(buf);
}

void GameOverOverlay::onHide()
{
	game->showMainMenu();
}


void GameOverOverlay::onUpdate(float dt)
{
}

void GameOverOverlay::onDraw(sf::RenderTarget& rt)
{
	sf::RectangleShape rect(sf::Vector2f(static_cast<float>(rt.getSize().x), static_cast<float>(rt.getSize().y)));
	rect.setFillColor(sf::Color(0, 0, 0, 180));
	rt.draw(rect);

	txtGameOver.setPosition(rt.getSize().x/2.f, rt.getSize().y/2.f);
	rt.draw(txtGameOver);

	txtPressEsc.setPosition(rt.getSize().x/2.f, rt.getSize().y/2.f + 64.f);
	rt.draw(txtPressEsc);

	rt.draw(txtStats);
}


void GameOverOverlay::onMouseDown(int x, int y, unsigned b)
{
}

void GameOverOverlay::onMouseUp(int x, int y, unsigned b)
{
}

void GameOverOverlay::onMouseWheel(int x, int y, int d)
{
}

void GameOverOverlay::onMouseMove(int x, int y, int dx, int dy)
{
}

void GameOverOverlay::onKeyDown(int key)
{
	switch (key)
	{
	case sf::Keyboard::Return:
		this->close();
		break;
	}
}

void GameOverOverlay::onKeyUp(int key)
{
}