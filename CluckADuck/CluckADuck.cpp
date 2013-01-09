#include "CluckADuck.hpp"
#include "utils.hpp"
#include "FontRes.hpp"

#include <sstream>


CluckADuck::CluckADuck()
{ }
CluckADuck::~CluckADuck()
{ }


int CluckADuck::SCR_W = 800;
int CluckADuck::SCR_H = 600;


bool CluckADuck::onInitialize(unsigned w, unsigned h)
{
	player = NULL;
	sndbufRubberDuck = NULL;
	sndbufPickUp = NULL;
	sndbufExplosion = NULL;
	overlay = NULL;
	overlayPause = NULL;
	overlayGameOver = NULL;
	overlayInitMenu = NULL;

	// Create window.
	this->getRW().create(sf::VideoMode(w, h), "Cluck-A-Duck: Approximately Nine of Them Edition", sf::Style::Close, sf::ContextSettings(16, 0, 4));
	this->getRW().setKeyRepeatEnabled(false);
	SCR_W = w;
	SCR_H = h;

	// Set window icon. (Or, at least try to)
	sf::Image ico;
	if(ico.loadFromFile("res/duckico.png"))
	{
		this->getRW().setIcon(16, 16, ico.getPixelsPtr());
	}

	// Seed RNG.
	randomInit();

	// Initialize classes.
	if (!FontRes::StaticInit())
	{
		showErr("Error loading resource(s) from FontRes class.");
		return false;
	}
	if (!Ducky::StaticInit())
	{
		showErr("Error loading resource(s) from Ducky class.");
		return false;
	}
	if (!Player::StaticInit())
	{
		showErr("Error loading resource(s) from Player class.");
		return false;
	}
	if (!Bullet::StaticInit())
	{
		showErr("Error loading resource(s) from Bullet class.");
		return false;
	}
	if (!Powerup::StaticInit())
	{
		showErr("Error loading resource(s) from Powerup class.");
		return false;
	}
	if (!Blast::StaticInit())
	{
		showErr("Error loading resource(s) from Blast class.");
		return false;
	}

	// Set up debug.
	this->debugMode = 0;
	txtDebug = sf::Text("", FontRes::getArcadeFont(), 11);
	txtDebug.setStyle(sf::Text::Bold);
	txtDebug.setColor(sf::Color(225, 225, 225, 200));
	txtDebug.setPosition(5.f, 5.f);

	// Set up overlays.
	overlay = new OverlayManager();
	overlayPause = new PauseOverlay();
	overlayGameOver = new GameOverOverlay(this);
	overlayInitMenu = new InitMenuOverlay(this);

	// Set up HUD
	txtScore = sf::Text("", FontRes::getArcadeFont(), 30);
	txtScore.setStyle(sf::Text::Bold);
	txtScore.setColor(sf::Color(0, 170, 255, 225));
	txtScore.setPosition(10.f, SCR_H-35.f);

	txtLevel = sf::Text("", FontRes::getArcadeFont(), 22);
	txtLevel.setColor(sf::Color(0, 170, 255, 225));
	txtLevel.setPosition(10.f, SCR_H-58.f);

	txtLives = sf::Text("", FontRes::getArcadeFont(), 14);
	txtLives.setColor(sf::Color(50, 225, 100, 225));
	txtLives.setPosition(28.f, SCR_H-74.f);
	txtLives.setStyle(sf::Text::Bold);

	txtTime = sf::Text("", FontRes::getArcadeFont(), 22);
	txtTime.setColor(sf::Color(0, 235, 25, 225));
	txtTime.setPosition(SCR_W/2.f, 10.f);

	txtExplosiveRoundsTimer = sf::Text("", FontRes::getArcadeFont(), 16);
	txtExplosiveRoundsTimer.setColor(sf::Color(255, 100, 0, 235));
	txtExplosiveRoundsTimer.setStyle(sf::Text::Bold);

	txtInvincibilityTimer = sf::Text("", FontRes::getArcadeFont(), 16);
	txtInvincibilityTimer.setColor(sf::Color(100, 200, 255, 235));
	txtInvincibilityTimer.setStyle(sf::Text::Bold);

	txtIndicator = sf::Text("", FontRes::getArcadeFont(), 18);
	txtIndicator.setColor(sf::Color(255, 255, 255, 255));
	txtIndicator.setStyle(sf::Text::Bold);

	// Set up graphical resources
	texBombIcon = sf::Texture();
	texPlayerIcon = sf::Texture();
	sprBombIcon	= sf::Sprite();
	sprPlayerIcon = sf::Sprite();

	texBombIcon.loadFromFile("res/bombicon.png");
	texPlayerIcon.loadFromFile("res/playericon.png");

	sprBombIcon.setTexture(texBombIcon);
	sprPlayerIcon.setTexture(texPlayerIcon);

	sprPlayerIcon.setPosition(10.f, SCR_H-76.f);

	// Set up music.
	msc.openFromFile("res/duck.ogg");
	msc.setVolume(75.f);
	msc.setLoop(true);

	// Set up sounds.
	sndbufRubberDuck = new sf::SoundBuffer();
	sndbufPickUp = new sf::SoundBuffer();
	sndbufExplosion = new sf::SoundBuffer();
	sndbufRubberDuck->loadFromFile("res/rubberducky.wav");
	sndbufPickUp->loadFromFile("res/pickup.wav");
	sndbufExplosion->loadFromFile("res/bomb.wav");
	sndRubberDuck.setBuffer(*sndbufRubberDuck);
	sndPickUp.setBuffer(*sndbufPickUp);
	sndExplosion.setBuffer(*sndbufExplosion);
	sndExplosionSmall.setBuffer(*sndbufExplosion);
	sndExplosionSmall.setPitch(2.5f);
	sndExplosionSmall.setVolume(50.f);

	// Set up highscore board.
	if (Highscores::ERROR_RES score_err = scoreboard.load("highscores.qck"))
	{
		if (score_err == Highscores::ERR_CORRUPTED)
		{
			showErr("Error loading highscores: Corrupted highscore data. No highscore data was loaded.");
		}
		else if (score_err == Highscores::ERR_INVALIDFILE)
		{
			showErr("Error loading highscores: Invalid highscore data file. No highscore data was loaded.");
		}
		else if (score_err == Highscores::ERR_CANNOTCREATE)
		{
			showErr("Error loading highscores: Could not create data file. No highscore data was loaded.");
		}
		else if (score_err == Highscores::ERR_CANNOTREAD)
		{
			showErr("Error loading highscores: Could not read data file. No highscore data was loaded.");
		}
		else if (score_err == Highscores::ERR_LOWMEM)
		{
			showErr("Error loading highscores: Too low memory. No highscore data was loaded.");
		}
	}

	// Initialize game values.
	player = new Player();
	this->score = 0;
	this->gameTime = 0.;
	this->elapsedTime = 0.;
	this->duckNormalCount = 0;
	this->duckBossCount = 0;
	this->level = 0;
	this->nextlvlscore = 0;
	this->usedExtraLives = 0;
	this->usedInv = 0;
	this->usedBombs = 0;
	this->usedExpRounds = 0;

	// Show main menu.
	this->showMainMenu();

	// Game set up successfully.
	return true;
}


void CluckADuck::onTerminate()
{
	// Clear class resources.
	FontRes::StaticQuit();

	Ducky::StaticQuit();
	Player::StaticQuit();
	Bullet::StaticQuit();
	Powerup::StaticQuit();
	Blast::StaticQuit();

	// Clean world.
	this->cleanupWorld();

	// Free player.
	if (player)
		delete player;

	// Free overlays
	if (overlay)
		delete overlay;
	if (overlayPause)
		delete overlayPause;
	if (overlayGameOver)
		delete overlayGameOver;
	if (overlayInitMenu)
		delete overlayInitMenu;

	// Free other resources.
	if (sndbufRubberDuck)
		delete sndbufRubberDuck;
	if (sndbufPickUp)
		delete sndbufPickUp;
	if (sndbufExplosion)
		delete sndbufExplosion;
}


void CluckADuck::onTick(float ms)
{
	overlay->update(ms);

	if (this->isPaused()) return;

	// Spawn duck
	while (this->nextDuckSpawn >= this->duckSpawnRate)
	{
		Ducky* duck = new Ducky();

		int sideToSpawn = randomiinc(1, 4);
		switch(sideToSpawn)
		{
		case 1: // left
			duck->setPos(-50., randomd(0., static_cast<double>(SCR_H)));
			break;
		case 2: // right
			duck->setPos(SCR_W+50., randomd(0., static_cast<double>(SCR_H)));
			break;
		case 3: // top
			duck->setPos(randomd(0., static_cast<double>(SCR_W)), -50.);
			break;
		case 4: // bottom
			duck->setPos(randomd(0., static_cast<double>(SCR_W)), SCR_H+50.);
			break;
		}
		
		double ang = atan2( duck->getPos().y - randomd(100., SCR_H-100.),
							duck->getPos().x - randomd(150., SCR_W-150.));

		duck->setVel(-cos(ang)*1., -sin(ang)*1.);

		this->gmDuckSpawned(duck);

		ducks.push_back(duck);

		this->nextDuckSpawn -= this->duckSpawnRate;
	}

	// Check boundries for ducks.
	for (int i = 0, sz = ducks.size(); i < sz; ++i)
	{
		Ducky& duck = *ducks[i];

		if (duck.getPos().x >= static_cast<double>(SCR_W) || duck.getPos().x < 0.)
		{
			if (duck.enteringField)
				continue;
			duck.setVelX(-duck.getVel().x);
		}

		if (duck.getPos().y >= static_cast<double>(SCR_H) || duck.getPos().y < 0.)
		{
			if (duck.enteringField)
				continue;
			duck.setVelY(-duck.getVel().y);
		}

		if (duck.enteringField)
			duck.enteringField = false;
	}

	// Check boundries for powerups.
	for (int i = 0, sz = items.size(); i < sz; ++i)
	{
		Powerup& powerup = *items[i];

		if (powerup.getPos().x >= static_cast<double>(SCR_W) || powerup.getPos().x < 0.)
			powerup.setVelX(-powerup.getVel().x);

		if (powerup.getPos().y >= static_cast<double>(SCR_H) || powerup.getPos().y < 0.)
			powerup.setVelY(-powerup.getVel().y);
	}

	// Check boundries for bullets.
	for (int i = 0, sz = bullets.size(); i < sz; ++i)
	{
		Bullet* bullet = bullets[i];

		if (bullet->getPos().x >= static_cast<double>(SCR_W) || bullet->getPos().x < 0. ||
			bullet->getPos().y >= static_cast<double>(SCR_H) || bullet->getPos().y < 0. )
		{
			delete bullet;
			bullets.erase(bullets.begin()+i);
			--i; --sz;
		}
	}

	// Check boundries for player.
	if (player->getPos().x >= static_cast<double>(SCR_W))
	{
		player->setPosX(static_cast<double>(SCR_W));
		player->setVelX(0.);
	}
	else if (player->getPos().x < 0.)
	{
		player->setPosX(0.);
		player->setVelX(0.);
	}
	if (player->getPos().y >= static_cast<double>(SCR_H))
	{
		player->setPosY(static_cast<double>(SCR_H));
		player->setVelY(0.);
	}
	else if (player->getPos().y < 0.)
	{
		player->setPosY(0.);
		player->setVelY(0.);
	}

	// Perform player-duck collision checks.
	for (int i = 0, sz = ducks.size(); i < sz; ++i)
	{
		Ducky* duck = ducks[i];
		if (player->checkRadialCollision(*duck))
		{
			// Duck hit the player.
			this->gmDuckHitPlayer(duck);
		}
	}

	// Perform player-powerup collison checks.
	for (int i = 0, sz = items.size(); i < sz; ++i)
	{
		Powerup* powerup = items[i];
		if (player->checkRadialCollision(*powerup))
		{
			// Pick up powerup.
			this->gmPlayerPickUpPowerup(powerup);

			// Remove item.
			delete powerup;
			items.erase(items.begin()+i);
			--i; --sz;
		}
	}

	// Perform bullet-duck collision checks.
	for (int i = 0, sz = bullets.size(); i < sz; ++i)
	{
		Bullet* bullet = bullets[i];

		for (int j = 0, jsz = ducks.size(); j < jsz; ++j)
		{
			Ducky* duck = ducks[j];

			if (bullet->checkRadialCollision(*duck))
			{
				// Perform damage.
				this->gmPlayerHitDuck(duck, bullet);

				// Remove bullet.
				delete bullet;
				bullets.erase(bullets.begin()+i);
				--i; --sz;

				break;
			}
		}
	}

	// Perform explosion-duck collision checks.
	for (int i = 0, sz = explosions.size(); i < sz; ++i)
	{
		Blast* blast = explosions[i];

		for (int j = 0, jsz = ducks.size(); j < jsz; ++j)
		{
			Ducky* duck = ducks[j];

			if (blast->checkRadialCollision(*duck))
			{
				// Perform damage.
				this->gmPlayerExplodeDuck(duck, blast);
			}
		}
	}
	
	// Update player and fire weapon.
	if (player->checkInput(Player::INPUT_GUN_FIRE))
	{
		Bullet* bullet = player->fireWeapon();
		if (bullet)
		{
			this->gmPlayerShoot(bullet);
			bullets.push_back(bullet);
		}
	}
	player->update(ms);

	// Update all bullets.
	for (int i = 0, sz = bullets.size(); i < sz; ++i)
		bullets[i]->update(ms);

	// Update all ducks.
	for (int i = 0, sz = ducks.size(); i < sz; ++i)
	{
		Ducky* duck = ducks[i];

		// Check if duck is alive.
		if (duck->health <= 0.)
		{
			// Duck died. :(
			this->gmDuckKilled(duck);

			// Remove duck.
			delete duck;
			ducks.erase(ducks.begin()+i);
			--i; --sz;

			continue;
		}

		duck->update(ms);
	}

	// Update all powerups.
	for (int i = 0, sz = items.size(); i < sz; ++i)
	{
		Powerup* powerup = items[i];

		// Check if expired.
		if (powerup->expired())
		{
			delete powerup;
			items.erase(items.begin()+i);
			--i; --sz;
			continue;
		}

		powerup->update(ms);
	}

	// Update all explosions.
	for (int i = 0, sz = explosions.size(); i < sz; ++i)
	{
		Blast* blast = explosions[i];

		// Check if expired.
		if (blast->isFinished())
		{
			delete blast;
			explosions.erase(explosions.begin()+i);
			--i; --sz;
			continue;
		}

		blast->update(ms);
	}

	// Update gamemode.
	this->gmTick(ms);

	// Increase timers.
	this->nextDuckSpawn += ms;
}


void CluckADuck::onDrawSF()
{
	sf::RenderWindow& rw = this->getRW();
	rw.clear();

	// Draw player.
	player->draw(rw);

	// Draw bullets.
	for (int i = 0, sz = bullets.size(); i < sz; ++i)
		bullets[i]->draw(rw);

	// Draw ducks.
	for (int i = 0, sz = ducks.size(); i < sz; ++i)
		ducks[i]->draw(rw);

	// Draw powerups.
	for (int i = 0, sz = items.size(); i < sz; ++i)
		items[i]->draw(rw);

	// Draw explosions.
	for (int i = 0, sz = explosions.size(); i < sz; ++i)
		explosions[i]->draw(rw);

	// Do gamemode draw.
	this->gmDraw(rw);

	// Draw overlays.
	overlay->draw(rw);

	// Draw debug.
	if (this->debugMode != 0 && !this->isPaused())
	{
		std::stringstream str;
		str << "FPS: " << (int)this->getFPS() << std::endl;

		if (this->debugMode >= 2)
		{
			str << "MS: " << (int)this->getMS() << std::endl;
			if (this->debugMode == 3)
			{
				str << std::endl;
				str << "player.pos:\t" << player->getPos().x << "\t" << player->getPos().y << std::endl;
				str << "player.vel:\t" << player->getVel().x << "\t" << player->getVel().y << std::endl;
				str << std::endl;
				str << "bullets.count:\t" << bullets.size() << std::endl;
				str << "ducks.count:\t" << ducks.size() << std::endl;
				str << "items.count:\t" << items.size() << std::endl;
				str << "explosions.count:\t" << explosions.size() << std::endl;
				str << "indicators.count:\t" << indicators.size() << std::endl;
			}
		}

		txtDebug.setString(str.str());
		rw.draw(txtDebug);
	}
}


void CluckADuck::onDrawGL()
{
	sf::RenderWindow& rw = this->getRW();

}


void CluckADuck::onMouseDown(int x, int y, unsigned b)
{
	if (overlay->mouseDown(x, y, b))
		return;

	switch (b)
	{
	case sf::Mouse::Left: // Fire weapon.
		player->startInput(Player::INPUT_GUN_FIRE);
		break;

	case sf::Mouse::Right: // Use a bomb.
		if (player->bombCount > 0)
		{
			this->gmPlayerDetonateBomb(this->addBlast(player->getPos()));
		}
		break;
	}
}


void CluckADuck::onMouseUp(int x, int y, unsigned b)
{
	if (overlay->mouseUp(x, y, b))
		return;

	switch (b)
	{
	case sf::Mouse::Left: // Stop firing weapon.
		player->endInput(Player::INPUT_GUN_FIRE);
		break;
	}
}


void CluckADuck::onMouseWheel(int x, int y, int d)
{
	if (overlay->mouseWheel(x, y, d))
		return;
}


void CluckADuck::onMouseMove(int x, int y, int dx, int dy)
{
	if (overlay->mouseMove(x, y, dx, dy))
		return;

	player->updateMousePos(x, y);
}


void CluckADuck::onKeyDown(int key)
{
	if (overlay->keyDown(key))
		return;

	switch (key)
	{

		// Show/Hide debug.
	case sf::Keyboard::Tilde:
		this->debugMode++;
		if (this->debugMode > 3)
			this->debugMode = 0;
		break;

		// Pause game.
	case sf::Keyboard::Escape:
		this->showPauseMenu();
		break;

		// Start movement controls
	case sf::Keyboard::W:
	case sf::Keyboard::Up:
		player->startInput(Player::INPUT_MOVE_UP);
		break;
	case sf::Keyboard::S:
	case sf::Keyboard::Down:
		player->startInput(Player::INPUT_MOVE_DOWN);
		break;
	case sf::Keyboard::A:
	case sf::Keyboard::Left:
		player->startInput(Player::INPUT_MOVE_LEFT);
		break;
	case sf::Keyboard::D:
	case sf::Keyboard::Right:
		player->startInput(Player::INPUT_MOVE_RIGHT);
		break;

	}
}


void CluckADuck::onKeyUp(int key)
{
	if (overlay->keyUp(key))
		return;

	switch (key)
	{

		// End movement controls
	case sf::Keyboard::W:
	case sf::Keyboard::Up:
		player->endInput(Player::INPUT_MOVE_UP);
		break;
	case sf::Keyboard::S:
	case sf::Keyboard::Down:
		player->endInput(Player::INPUT_MOVE_DOWN);
		break;
	case sf::Keyboard::A:
	case sf::Keyboard::Left:
		player->endInput(Player::INPUT_MOVE_LEFT);
		break;
	case sf::Keyboard::D:
	case sf::Keyboard::Right:
		player->endInput(Player::INPUT_MOVE_RIGHT);
		break;

	}
}


void CluckADuck::onKeyText(int ch)
{
	if (overlay->keyText(ch))
		return;
}


void CluckADuck::onWindowResize(unsigned w, unsigned h)
{

}


void CluckADuck::onWindowClose()
{
	this->terminate();
}


/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////

Blast* CluckADuck::addBlast(const vec2& pos, double lifetime, double damage)
{
	Blast* blast = new Blast(lifetime);

	// Set specified properties.
	blast->setPos(pos);
	blast->damage = damage;

	// Add explosion to game field.
	explosions.push_back(blast);
	return blast;
}

const float CluckADuck::Indicator::LIFETIME = 1500.f;
CluckADuck::Indicator* CluckADuck::addIndicator(const char* msg, const vec2& pos, unsigned char r, unsigned char g, unsigned char b)
{
	Indicator* ind = new Indicator();

	// Set specified properties.
	ind->msg = msg;
	ind->col.r = r;
	ind->col.g = g;
	ind->col.b = b;
	ind->col.a = 255;
	ind->pos = pos;
	ind->life = 0.f;

	// Push indicator to game.
	indicators.push_back(ind);
	return ind;
}

bool CluckADuck::isPaused()
{
	return (overlayPause->isShowing()
		||	overlayGameOver->isShowing()
		||	overlayInitMenu->isShowing()
	);
}

void CluckADuck::showMainMenu()
{
	overlay->pushOverlay(overlayInitMenu);
}

void CluckADuck::showGameOverMenu()
{
	overlay->pushOverlay(overlayGameOver);
}

void CluckADuck::showPauseMenu()
{
	overlay->pushOverlay(overlayPause);
}

void CluckADuck::initGame(CluckADuck::GAME_MODE mode)
{
	// Reset world.
	this->cleanupWorld();

	// Reset some game values.
	this->score = 0;
	this->elapsedTime = 0.;
	this->duckNormalCount = 0;
	this->duckBossCount = 0;
	this->nextDuckSpawn = 0.;
	this->usedExtraLives = 0;
	this->usedInv = 0;
	this->usedBombs = 0;

	// Set up player.
	player->clearInput();

	player->setPos(SCR_W/2., SCR_H/2.);
	player->setVel(0., 0.);
	player->lives = 1;
	player->bombCount = 1;
	player->invincibility = false;
	player->explosiveRounds = false;

	// Initialize game mode.
	this->gamemode = mode;
	this->gmInit();
}

void CluckADuck::cleanupWorld()
{
	// Free bullets.
	for (size_t i = 0, sz = bullets.size(); i < sz; ++i)
		delete bullets[i];
	bullets.clear();

	// Free the ducks!
	for (size_t i = 0, sz = ducks.size(); i < sz; ++i)
		delete ducks[i];
	ducks.clear();

	// Free powerups.
	for (size_t i = 0, sz = items.size(); i < sz; ++i)
		delete items[i];
	items.clear();

	// Free explosions.
	for (size_t i = 0, sz = explosions.size(); i < sz; ++i)
		delete explosions[i];
	explosions.clear();

	// Free indicators.
	for (size_t i = 0, sz = indicators.size(); i < sz; ++i)
		delete indicators[i];
	indicators.clear();
}

void CluckADuck::gameOver()
{
	// Display game over menu.
	this->showGameOverMenu();

	// Stop music.
	if (msc.getStatus() == sf::Music::Playing)
		msc.stop();
}



void CluckADuck::gmInit()
{
	if (this->gamemode == MODE_NORMAL)
	{
		// Set game values for Normal Mode.

		this->level = 1;
		this->nextlvlscore = 50;

		this->duckSpawnRate = 1000.;

		player->bulletSpeed = 12.;
		player->bulletDamage = 1.;
		player->fireRate = 325.;
		player->nextBullet = player->fireRate;

		this->gameTime = 60000.;
	}
	else if (this->gamemode == MODE_MOO)
	{
		// Set game values for MoO Mode.

		this->duckSpawnRate = 65.;
		
		player->bulletSpeed = 12.;
		player->bulletDamage = 1.;
		player->fireRate = 20.;
		player->nextBullet = player->fireRate;

		this->gameTime = 0.;

		msc.play();
	}
}

void CluckADuck::gmTick(float dt)
{
	// Update indicators.
	for (int i = 0, sz = indicators.size(); i < sz; ++i)
	{
		Indicator* ind = indicators[i];
		
		// Decrease life time.
		ind->life += dt;

		// Check if expired.
		if (ind->life >= Indicator::LIFETIME)
		{
			// Remove indicator.
			delete ind;
			indicators.erase(indicators.begin() + i);
			--i; --sz;

			continue;
		}

		// Recalculate transparency.
		ind->col.a = 255 - static_cast<unsigned char>(ind->life / Indicator::LIFETIME * 255);

		// Move position upwards.
		ind->pos.y -= 0.5f;
	}

	if (this->gamemode == MODE_NORMAL)
	{
		// If timer runs out of time, game over.
		if (this->gameTime <= 0)
		{
			this->gameOver();
		}

		// Decrease timer.
		this->gameTime -= dt;
	}

	// Increase elapsed time.
	this->elapsedTime += dt;
}

void CluckADuck::gmDraw(sf::RenderTarget& rt)
{
	// Display indicators.
	for (int i = 0, sz = indicators.size(); i < sz; ++i)
	{
		Indicator& ind = *indicators[i];

		txtIndicator.setString(ind.msg);
		txtIndicator.setColor(ind.col);

		txtIndicator.setOrigin(txtIndicator.getGlobalBounds().width/2.f, txtIndicator.getGlobalBounds().height/2.f);
		txtIndicator.setPosition(static_cast<float>(ind.pos.x), static_cast<float>(ind.pos.y));

		rt.draw(txtIndicator);
	}

	char buf[32];

	// Display score.
	sprintf(buf, "Score: %d", this->score);
	txtScore.setString(buf);
	rt.draw(txtScore);

	// Display time.
	double t = 0.;
	int tOff = 0;
	if (this->gamemode == MODE_NORMAL)
	{
		t = this->gameTime;
	}
	else if (this->gamemode == MODE_MOO)
	{
		t = this->elapsedTime;
	}
	int hours = (int)(t/3600000);
	t -= hours*3600000.;
	int mins = (int)(t/60000);
	t -= mins*60000.;
	int secs = (int)(t/1000);

	if (hours)
		tOff += sprintf(buf, "%d:", hours);
	if (mins)
		tOff += sprintf(buf+tOff, "%s%d:", (mins<10 ? "0" : ""), mins);
	sprintf(buf+tOff, "%s%d", (secs<10 ? "0" : ""), secs);
	txtTime.setString(buf);

	txtTime.setOrigin(txtTime.getGlobalBounds().width/2.f, txtTime.getGlobalBounds().height/2.f);
	rt.draw(txtTime);

	// Display lives.
	sprintf(buf, "x %d", player->lives);
	txtLives.setString(buf);
	rt.draw(txtLives);
	rt.draw(sprPlayerIcon);

	// Display bombs available.
	for (unsigned i = 0; i < player->bombCount; ++i)
	{
		sprBombIcon.setPosition(10.f + i*5.f, SCR_H-96.f);
		rt.draw(sprBombIcon);
	}

	
	if (player->invincibility)
	{
		// Display invicibility timer.
		sprintf(buf, "%d", (int)(player->invincibilityTime/1000.));
		txtInvincibilityTimer.setString(buf);

		txtInvincibilityTimer.setOrigin(txtInvincibilityTimer.getGlobalBounds().width/2.f,
										txtInvincibilityTimer.getGlobalBounds().height/2.f);
		txtInvincibilityTimer.setPosition(static_cast<float>(player->getPos().x), static_cast<float>(player->getPos().y + 16.));

		rt.draw(txtInvincibilityTimer);

		// Display invincibility shield indicator.
		sf::CircleShape c(26.f);
		c.setFillColor(sf::Color::Transparent);
		c.setOutlineThickness(static_cast<float>(player->invincibilityTime/player->invincibilityTimeSet * 6.));
		c.setOutlineColor(sf::Color(100, 200, 255, 235));
		c.setPosition(static_cast<float>(player->getPos().x), static_cast<float>(player->getPos().y));
		c.setOrigin(26.f, 26.f);
		rt.draw(c);
	}

	if (player->explosiveRounds)
	{
		// Display explosive rounds timer.
		sprintf(buf, "%d", (int)(player->explosiveRoundsTime/1000.));
		txtExplosiveRoundsTimer.setString(buf);

		txtExplosiveRoundsTimer.setOrigin(txtExplosiveRoundsTimer.getGlobalBounds().width/2.f,
										txtExplosiveRoundsTimer.getGlobalBounds().height/2.f);
		txtExplosiveRoundsTimer.setPosition(static_cast<float>(player->getPos().x), static_cast<float>(player->getPos().y - 18.));

		rt.draw(txtExplosiveRoundsTimer);

		// Display explosive rounds indicator.
		sf::CircleShape c(30.f);
		c.setFillColor(sf::Color::Transparent);
		c.setOutlineThickness(static_cast<float>(player->explosiveRoundsTime/player->explosiveRoundsTimeSet * 6.));
		c.setOutlineColor(sf::Color(255, 100, 0, 235));
		c.setPosition(static_cast<float>(player->getPos().x), static_cast<float>(player->getPos().y));
		c.setOrigin(30.f, 30.f);
		rt.draw(c);
	}

	if (this->gamemode == MODE_NORMAL)
	{
		// Display game level.
		sprintf(buf, "Level: %d", this->level);
		txtLevel.setString(buf);
		rt.draw(txtLevel);
	}
}

void CluckADuck::gmDuckSpawned(Ducky* duck)
{
	// Set up duck's properties.

	// Is it a big duck?
	bool boss = (randomdexpd(10) >= (this->gamemode==MODE_MOO ? 0.99 : 0.98));

	if (boss)
	{
		// Make it big and have lots of health.
		duck->setScale(4.);

		duck->maxHealth = 60.;
		duck->score = 500;

		duck->typeChar = 'b'; // 'b'oss
	}
	else
	{
		// Normal duck.
		duck->maxHealth = 1.5;
		duck->score = 5;

		duck->typeChar = 'n'; // 'n'ormal
	}

	if (this->gamemode == MODE_NORMAL)
	{
		// Modify properties according to game level.

		duck->maxHealth += duck->maxHealth * (this->level-1)*1.75;

		if (boss)
			duck->score += 8*(this->level-1);
		else
			duck->score += 3*(this->level-1);
	}

	// Set duck to full health.
	duck->health = duck->maxHealth;
}

void CluckADuck::gmDuckKilled(Ducky* duck)
{
	// Quack.
	duck->quack();

	// Add score.
	this->gmAddScore(duck->score);
	{
		char buf[16];
		sprintf(buf, "+%d", duck->score);
		this->addIndicator(buf, duck->getPos(), 255, 200, 0);
	}

	if (duck->typeChar == 'n')
	{
		// Increase kill count.
		this->duckNormalCount++;
	}
	else if (duck->typeChar == 'b')
	{
		// Boss duck explodes.
		if (this->gamemode == MODE_NORMAL)
		{
			this->addBlast(duck->getPos(), 225., 10. + 10.*(this->level * 0.25));
		}
		else if (this->gamemode == MODE_MOO)
		{
			this->addBlast(duck->getPos(), 225., 10.);
		}

		// Make explosion sound.
		sndExplosion.play();

		// Increase kill count.
		this->duckBossCount++;
	}
	
	// Random powerup drop.
	Powerup* powerup = NULL;
	double r = randomdexpd( (this->gamemode == MODE_MOO) ? 24. : 8.);
	if (r >= 0.9925)
	{
		// Life power up.
		powerup = new Powerup();
		powerup->setupLifePowerup(this);
	}
	else if (r >= 0.98)
	{
		// Explosive rounds power up.
		powerup = new Powerup();
		powerup->setupExpRoundsPowerup(this);
	}
	else if (r >= 0.95)
	{
		// Invicibility power up.
		powerup = new Powerup();
		powerup->setupInvPowerup(this);
	}
	else if (r >= 0.90)
	{
		// Bomb power up.
		powerup = new Powerup();
		powerup->setupBombPowerup(this);
	}
	else if (r >= 0.125)
	{
		// Points power up.
		powerup = new Powerup();

		if (this->gamemode == MODE_NORMAL)
		{
			powerup->setupPointsPowerup(this, static_cast<int>((this->level-1)*5 + 10));
		}
		else if (this->gamemode == MODE_MOO)
		{
			powerup->setupPointsPowerup(this, 25);
		}
	}

	if (powerup)
	{
		// Set up power up and add it to the game field.
		powerup->setPos(duck->getPos());
		powerup->setVel(randomd(-0.5, 0.5), randomd(-0.5, 0.5));

		// Add powerup to game field.
		items.push_back(powerup);
	}

	if (this->gamemode == MODE_NORMAL)
	{
		// Give some extra time.
		this->gameTime += 1200.;
	}
}

void CluckADuck::gmDuckHitPlayer(Ducky* duck)
{
	// Don't do anything if in invincibility mode.
	if (player->invincibility)
		return;

	// Play rubber ducky sound.
	sndRubberDuck.play();

	// Check if any lives left.
	if (player->lives > 0)
	{
		// Give recovery invincibility.
		player->giveInvincibility(3000.);

		// Show an indication.
		this->addIndicator("-1 Life", player->getPos(), 255, 0, 0);

		// Take a life away.
		player->lives--;

		// Increase extra lives used counter.
		this->usedExtraLives++;

		return;
	}

	// End game.
	this->gameOver();
}

void CluckADuck::gmPlayerHitDuck(Ducky* duck, Bullet* bullet)
{
	// Do explosive damage if using explosive rounds.
	if (player->explosiveRounds)
	{
		// Create explosion.
		Blast * blast = this->addBlast(bullet->getPos(), 120.);

		// Set explosion damage.
		if (this->gamemode == MODE_NORMAL)
		{
			blast->damage = 0.35 + 0.35*(level * 0.35);
		}
		else if (this->gamemode == MODE_MOO)
		{
			blast->damage = 0.35;
		}

		// Make small explosion sound.
		sndExplosionSmall.play();

		// Increase explosive rounds counter.
		this->usedExpRounds++;
	}

	// Remove health.
	duck->health -= bullet->damage;
}

void CluckADuck::gmPlayerExplodeDuck(Ducky* duck, Blast* blast)
{
	// Remove health.
	duck->health -= blast->damage;
}

void CluckADuck::gmAddScore(int add)
{
	// Add score.
	this->score += add;

	if (this->gamemode == MODE_NORMAL)
	{
		// Level up check.
		while (this->score >= this->nextlvlscore)
		{
			// Upgrade weapons.
			player->bulletDamage = 1. + (this->level * 0.75);
			player->fireRate -= 35 / (this->level * 0.75);
			if (player->fireRate < 16.)
				player->fireRate = 16.;

			// Make ducks spawn faster.
			this->duckSpawnRate -= 25 / (this->level*0.165);
			if (this->duckSpawnRate < 75.)
				this->duckSpawnRate = 75.;

			// Increase next level score requirement.
			this->nextlvlscore += level*50 + (this->level-1)*25;

			// Increase player level.
			this->level++;
		}
	}
}

void CluckADuck::gmPlayerShoot(Bullet* bullet)
{

}

void CluckADuck::gmPlayerDetonateBomb(Blast* blast)
{
	// Set damage.
	if (this->gamemode == MODE_NORMAL)
	{
		blast->damage = 17.5 + 17.5*(level * 0.35);
	}
	else if (this->gamemode == MODE_MOO)
	{
		blast->damage = 10.;
	}

	// Make explosion sound.
	sndExplosion.play();

	// Increase used bomb counter.
	this->usedBombs++;

	// Remove a bomb.
	player->bombCount--;
}

void CluckADuck::gmPlayerPickUpPowerup(Powerup* powerup)
{
	// Play pick up sound.
	sndPickUp.play();
	
	// Pick up powerup.
	powerup->use();

	// Show indicator.
	switch (powerup->powerType)
	{
	case Powerup::TYPE_BOMB:
		this->addIndicator("+1 Bomb", powerup->getPos(), 255, 25, 0);
		break;
	case Powerup::TYPE_EXPROUNDS:
		this->addIndicator("Explosive Rounds", powerup->getPos(), 255, 25, 0);
		break;
	case Powerup::TYPE_INV:
		this->addIndicator("Invincibility", powerup->getPos(), 25, 50, 255);
		this->usedInv++;
		break;
	case Powerup::TYPE_LIFE:
		this->addIndicator("+1 Life", powerup->getPos(), 50, 255, 0);
		break;
	case Powerup::TYPE_POINTS: {
		char buf[24];
		sprintf(buf, "+%d Points", ((PowerupData_Points*)powerup->powerData)->points);
		this->addIndicator(buf, powerup->getPos(), 255, 200, 0);
		break; }
	}
}

