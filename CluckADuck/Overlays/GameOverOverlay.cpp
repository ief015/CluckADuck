#include "GameOverOverlay.hpp"

#include "../CluckADuck.hpp"
#include "../FontRes.hpp"
#include "../Highscores.hpp"

GameOverOverlay::GameOverOverlay(CluckADuck* g) : Overlay()
{
	game = g;

	this->overrideControls(true);

	showHighscore = false;
	myScore = NULL;

	txtGameOver = sf::Text("Game Over", FontRes::getArcadeFont(), 72);
	txtGameOver.setOrigin(txtGameOver.getGlobalBounds().width/2.f, txtGameOver.getGlobalBounds().height);
	txtGameOver.setColor(sf::Color(225, 180, 0));

	txtPressEnter = sf::Text("Press ENTER to enter highscore.", FontRes::getHandWritingFont(), 24);
	txtPressEnter.setOrigin(txtPressEnter.getGlobalBounds().width/2.f, txtPressEnter.getGlobalBounds().height/2.f);
	txtPressEnter.setColor(sf::Color(225, 120, 0));

	txtPressEsc = sf::Text("Press ESCAPE to cancel.", FontRes::getHandWritingFont(), 24);
	txtPressEsc.setOrigin(txtPressEsc.getGlobalBounds().width/2.f, txtPressEsc.getGlobalBounds().height/2.f);
	txtPressEsc.setColor(sf::Color(225, 120, 0));

	txtPressEscScoreboard = sf::Text("Press ESCAPE to continue.", FontRes::getHandWritingFont(), 24);
	txtPressEscScoreboard.setOrigin(txtPressEscScoreboard.getGlobalBounds().width/2.f, txtPressEscScoreboard.getGlobalBounds().height/2.f);
	txtPressEscScoreboard.setColor(sf::Color(225, 120, 0));

	txtStats = sf::Text("", FontRes::getArcadeFont(), 18);
	txtStats.setPosition(10.f, 10.f);
	txtStats.setColor(sf::Color(225, 170, 0));

	txtScoreboardHeader = sf::Text("", FontRes::getHandWritingFont(), 14);
	txtScoreboardHeader.setColor(sf::Color(225, 150, 0));
	txtScoreboardHeader.setStyle(sf::Text::Bold);

	txtScoreboardEntry = sf::Text("", FontRes::getHandWritingFont(), 14);

	nameTextbox = new NameTextbox();
}

GameOverOverlay::~GameOverOverlay()
{
	delete nameTextbox;
}


void GameOverOverlay::onShown()
{
	// Write up stats.
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
		"Score - %u\n"
		"\n"
		"Ducks - %u\n"
		"Bosses - %u\n"
		"Total - %u\n"
		"\n"
		"Extra lives used - %u\n"
		"Explosive rounds detonated - %u\n"
		"Invincibilities used - %u\n"
		"Bombs used - %u\n"

		, hours, (mins < 10 ? "0" : ""), mins, (secs < 10 ? "0" : ""), secs
		, game->score

		, game->duckNormalCount
		, game->duckBossCount
		, game->duckNormalCount + game->duckBossCount
		
		, game->usedExtraLives
		, game->usedExpRounds
		, game->usedInv
		, game->usedBombs
	);
	txtStats.setString(buf);
	
	// Show highscore name text box.
	if (!ui->hasWidget(nameTextbox))
		ui->addWidget(nameTextbox);
	showHighscore = false;
	myScore = NULL;
}

void GameOverOverlay::onHide()
{
	game->showMainMenu();
}


void GameOverOverlay::onUpdate(float dt)
{
	Overlay::onUpdate(dt);
}


void GameOverOverlay::onDraw(sf::RenderTarget& rt)
{
	sf::RectangleShape rect(sf::Vector2f(static_cast<float>(rt.getSize().x), static_cast<float>(rt.getSize().y)));
	rect.setFillColor(sf::Color(0, 0, 0, 180));
	rt.draw(rect);

	rt.draw(txtStats);

	if (!showHighscore)
	{
		// Draw game over text.
		txtGameOver.setPosition(rt.getSize().x/2.f, rt.getSize().y/2.f);
		rt.draw(txtGameOver);

		// Draw enter text.
		txtPressEnter.setPosition(rt.getSize().x/2.f, rt.getSize().y/2.f + 48.f);
		rt.draw(txtPressEnter);

		// Draw cancel text.
		txtPressEsc.setPosition(rt.getSize().x/2.f, rt.getSize().y/2.f + 72.f);
		rt.draw(txtPressEsc);

		nameTextbox->x = 24.;
		nameTextbox->y = rt.getSize().y / 2. + 120.;
		Overlay::onDraw(rt);

	}
	else
	{
		Highscores& scoreboard = game->scoreboard;

		// Draw exit text.
		txtPressEscScoreboard.setPosition(rt.getSize().x/2.f, rt.getSize().y/2.f - 32.f);
		rt.draw(txtPressEscScoreboard);

		float posx, posy;
		posx = rt.getSize().x/4.f + 32.f;
		posy = rt.getSize().y/2.f;

		// Display scoreboard table headers.
		txtScoreboardHeader.setPosition(posx, posy);
		txtScoreboardHeader.setString("Name");
		rt.draw(txtScoreboardHeader);

		posx += 150.f;
		txtScoreboardHeader.setPosition(posx, posy);
		txtScoreboardHeader.setString("Score");
		rt.draw(txtScoreboardHeader);

		posx += 150.f;
		txtScoreboardHeader.setPosition(posx, posy);
		txtScoreboardHeader.setString("Time");
		rt.draw(txtScoreboardHeader);

		posy = rt.getSize().y/2.f + 24.f;

		// Display scores.
		char buf[32];
		bool myScoreDisplayed = false;
		int placeOffset = 0;
		for (size_t i = 0, sz = scoreboard.getScoreCount(); i < sz; ++i)
		{

			const Highscores::Score& score = scoreboard.getScore(i);

			// Reset horizontal position.
			posx = rt.getSize().x/4.f + 32.f - 20.f;

			// Set colour.
			if (&score == myScore)
			{
				txtScoreboardEntry.setColor(sf::Color(225, 180, 0));
				myScoreDisplayed = true;
			}
			else
			{
				txtScoreboardEntry.setColor(sf::Color(225, 120, 0));
			}

			// Skip unrelated gamemodes.
			if (score.gamemode != game->gamemode)
			{
				++placeOffset;
				continue;
			}

			sprintf(buf, "%u.", i-placeOffset+1);
			txtScoreboardEntry.setPosition(posx, posy);
			txtScoreboardEntry.setString(buf);
			rt.draw(txtScoreboardEntry);

			// Draw name.
			posx += 20.f;
			txtScoreboardEntry.setPosition(posx, posy);
			txtScoreboardEntry.setString(score.name);
			rt.draw(txtScoreboardEntry);

			// Draw score.
			posx += 150.f;
			sprintf(buf, "%u", score.score);
			txtScoreboardEntry.setPosition(posx, posy);
			txtScoreboardEntry.setString(buf);
			rt.draw(txtScoreboardEntry);

			// Draw time.
			posx += 150.f;
			double t = score.time;
			int hours, mins, secs;
			hours = (int)(t/3600000.);
			t -= hours*3600000.;
			mins = (int)(t/60000.);
			t -= mins*60000.;
			secs = (int)(t/1000.);
			sprintf(buf, "%d:%s%d:%s%d", hours, (mins < 10 ? "0" : ""), mins, (secs < 10 ? "0" : ""), secs);
			txtScoreboardEntry.setPosition(posx, posy);
			txtScoreboardEntry.setString(buf);
			rt.draw(txtScoreboardEntry);
			
			// Increase vertical position.
			posy += 24.f;

			
			// Force user score to be somewhere displayed.
			if (!myScoreDisplayed)
			{
				if (i == 8)
				{
					for (size_t j = i+1; j < sz; ++j)
					{
						if (&scoreboard.getScore(j) == myScore)
						{
							i = j-1;
							break;
						}
					}
					continue;
				}
			}

			// Limit number of scores to be drawn.
			if (i >= 9)
				break;

		}

	}
}


void GameOverOverlay::onMouseDown(int x, int y, unsigned b)
{
	Overlay::onMouseDown(x, y, b);
}


void GameOverOverlay::onMouseUp(int x, int y, unsigned b)
{
	Overlay::onMouseUp(x, y, b);
}


void GameOverOverlay::onMouseWheel(int x, int y, int d)
{
	Overlay::onMouseWheel(x, y, d);
}


void GameOverOverlay::onMouseMove(int x, int y, int dx, int dy)
{
	Overlay::onMouseMove(x, y, dx, dy);
}


void GameOverOverlay::onKeyDown(int key)
{
	switch (key)
	{

	case sf::Keyboard::Return:

		if (!showHighscore)
		{
			// Save score.
			Highscores& scoreboard = game->scoreboard;
			myScore = scoreboard.addScore(nameTextbox->getString(), game->gamemode, game->elapsedTime, game->score);
			scoreboard.saveScore(myScore);

			// Sort scoreboard.
			scoreboard.sortScoreboard(Highscores::SORT_SCORE);

			// Display scoreboard now.
			showHighscore = true;
		}

		break;

	case sf::Keyboard::Escape:

		if (showHighscore)
		{
			// Close overlay.
			this->close();
		}
		else
		{
			// Display scoreboard without saving score.
			showHighscore = true;
		}

		break;
	}

	Overlay::onKeyDown(key);
}


void GameOverOverlay::onKeyUp(int key)
{
	Overlay::onKeyUp(key);
}


void GameOverOverlay::onKeyText(int ch)
{
	Overlay::onKeyText(ch);
}
