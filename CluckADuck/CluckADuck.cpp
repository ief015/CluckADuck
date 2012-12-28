#include "CluckADuck.hpp"
#include "utils.hpp"
#include "font.hpp"

#include <sstream>


CluckADuck::CluckADuck()
{ }
CluckADuck::~CluckADuck()
{ }



bool CluckADuck::onInitialize(unsigned w, unsigned h)
{
	// Create window.
	this->getRW().create(sf::VideoMode(w, h), "Cluck-a-Duck: Approximately Nine of Them Edition", sf::Style::Close, sf::ContextSettings(16, 0, 4));
	this->getRW().setKeyRepeatEnabled(false);
	randomInit();

	scrW = w;
	scrH = h;

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
	debugMode = 0;
	txtDebug = sf::Text("", FontRes::getDefaultFont(), 11);
	txtDebug.setStyle(sf::Text::Bold);
	txtDebug.setColor(sf::Color(225, 225, 225, 200));
	txtDebug.setPosition(5.f, 5.f);

	// Set up overlays.
	overlay = new OverlayManager();
	overlayPause = new PauseOverlay();
	overlayGameOver = new GameOverOverlay();
	overlayGameOver->game = this;
	overlayInitMenu = new InitMenuOverlay();
	overlayInitMenu->game = this;

	// Set up HUD
	txtScore = sf::Text("", FontRes::getDefaultFont(), 30);
	txtScore.setStyle(sf::Text::Bold);
	txtScore.setColor(sf::Color(0, 170, 255, 225));
	txtScore.setPosition(10.f, scrH-35.f);

	txtLevel = sf::Text("", FontRes::getDefaultFont(), 22);
	txtLevel.setColor(sf::Color(0, 170, 255, 225));
	txtLevel.setPosition(10.f, scrH-64.f);

	txtLives = sf::Text("", FontRes::getDefaultFont(), 14);
	txtLives.setColor(sf::Color(50, 225, 100, 225));
	txtLives.setPosition(28.f, scrH-74.f);

	txtTime = sf::Text("", FontRes::getDefaultFont(), 22);
	txtTime.setColor(sf::Color(0, 235, 25, 225));
	txtTime.setPosition(scrW/2., 10.f);

	txtInvincibilityTimer = sf::Text("", FontRes::getDefaultFont(), 16);
	txtInvincibilityTimer.setColor(sf::Color(100, 200, 255, 235));
	txtInvincibilityTimer.setStyle(sf::Text::Bold);

	txtIndicator = sf::Text("", FontRes::getDefaultFont(), 18);
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

	sprPlayerIcon.setPosition(10.f, scrH-76.f);

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

	// Initialize game.
	player = new Player();
	score = 0;
	gameTime = 0.;
	elapsedTime = 0.;
	duckNormalCount = 0;
	duckBossCount = 0;
	level = 0;
	nextlvlscore = 0;
	usedExtraLives = 0;
	usedInv = 0;
	usedBombs = 0;

	// Show main menu.
	this->showMainMenu();

	// Game set up successfully.
	return true;
}


void CluckADuck::onTerminate()
{
	// Clear class resources.
	FontRes::StaticInit();

	Ducky::StaticQuit();
	Player::StaticQuit();
	Bullet::StaticQuit();
	Powerup::StaticQuit();
	Blast::StaticQuit();

	// Free player.
	delete player;

	// Clean world.
	this->cleanupWorld();

	// Free other resources.
	delete sndbufRubberDuck;
	delete sndbufPickUp;
	delete sndbufExplosion;
}


void CluckADuck::onTick(float ms)
{
	overlay->update(ms);

	if (isPaused()) return;

	// Spawn duck
	while (nextDuckSpawn >= duckSpawnRate)
	{
		Ducky* duck = new Ducky();

		int sideToSpawn = randomiinc(1, 4);
		switch(sideToSpawn)
		{
		case 1: // left
			duck->setPos(-50., randomd(0., static_cast<double>(scrH)));
			break;
		case 2: // right
			duck->setPos(scrW+50., randomd(0., static_cast<double>(scrH)));
			break;
		case 3: // top
			duck->setPos(randomd(0., static_cast<double>(scrW)), -50.);
			break;
		case 4: // bottom
			duck->setPos(randomd(0., static_cast<double>(scrW)), scrH+50.);
			break;
		}
		
		double ang = atan2( duck->getPos().y - randomd(100., scrH-100.),
							duck->getPos().x - randomd(150., scrW-150.));

		duck->setVel(-cos(ang)*1., -sin(ang)*1.);

		this->gmDuckSpawned(duck);

		ducks.push_back(duck);

		nextDuckSpawn -= duckSpawnRate;
	}

	// Check boundries.
	for (int i = 0, sz = ducks.size(); i < sz; ++i)
	{
		Ducky& duck = *ducks[i];

		if (duck.getPos().x >= static_cast<double>(scrW) || duck.getPos().x < 0.)
		{
			if (duck.enteringField)
				continue;
			duck.setVelX(-duck.getVel().x);
		}

		if (duck.getPos().y >= static_cast<double>(scrH) || duck.getPos().y < 0.)
		{
			if (duck.enteringField)
				continue;
			duck.setVelY(-duck.getVel().y);
		}

		if (duck.enteringField)
			duck.enteringField = false;
	}
	for (int i = 0, sz = items.size(); i < sz; ++i)
	{
		Powerup& powerup = *items[i];

		if (powerup.getPos().x >= static_cast<double>(scrW) || powerup.getPos().x < 0.)
			powerup.setVelX(-powerup.getVel().x);

		if (powerup.getPos().y >= static_cast<double>(scrH) || powerup.getPos().y < 0.)
			powerup.setVelY(-powerup.getVel().y);
	}
	for (int i = 0, sz = bullets.size(); i < sz; ++i)
	{
		Bullet* bullet = bullets[i];

		if (bullet->getPos().x >= static_cast<double>(scrW) || bullet->getPos().x < 0. ||
			bullet->getPos().y >= static_cast<double>(scrH) || bullet->getPos().y < 0. )
		{
			delete bullet;
			bullets.erase(bullets.begin()+i);
			--i; --sz;
		}
	}
	if (player->getPos().x >= static_cast<double>(scrW))
	{
		player->setPosX(static_cast<double>(scrW));
		player->setVelX(0.);
	}
	else if (player->getPos().x < 0.)
	{
		player->setPosX(0.);
		player->setVelX(0.);
	}
	if (player->getPos().y >= static_cast<double>(scrH))
	{
		player->setPosY(static_cast<double>(scrH));
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
		if (player->checkRadiusCollision(*duck))
		{
			// Duck hit the player.
			this->gmDuckHitPlayer(duck);
		}
	}

	// Perform player-powerup collison checks.
	for (int i = 0, sz = items.size(); i < sz; ++i)
	{
		Powerup* powerup = items[i];
		if (player->checkRadiusCollision(*powerup))
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

			if (bullet->checkRadiusCollision(*duck))
			{
				// Perform damage.
				this->gmPlayerHitDuck(duck, bullet);
				if (duck->health <= 0.)
				{
					// Duck died. :(
					this->gmDuckKilled(duck);

					// Remove duck.
					delete duck;
					ducks.erase(ducks.begin()+j);
					--j; --jsz;
				}

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

			if (blast->checkRadiusCollision(*duck))
			{
				// Perform damage.
				this->gmPlayerExplodeDuck(duck, blast);
				if (duck->health <= 0.)
				{
					// Duck died. :(
					this->gmDuckKilled(duck);

					// Remove duck.
					delete duck;
					ducks.erase(ducks.begin()+j);
					--j; --jsz;
				}
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
		ducks[i]->update(ms);

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
	nextDuckSpawn += ms;
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
	if (debugMode != 0 && !this->isPaused())
	{
		std::stringstream str;
		str << "FPS: " << (int)this->getFPS() << std::endl;

		if (debugMode >= 2)
		{
			str << "MS: " << (int)this->getMS() << std::endl;
			if (debugMode == 3)
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

	// Make explosion sound.
	sndExplosion.play();

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
	score = 0;
	elapsedTime = 0.;
	duckNormalCount = 0;
	duckBossCount = 0;
	nextDuckSpawn = 0.;
	usedExtraLives = 0;
	usedInv = 0;
	usedBombs = 0;

	// Set up player.
	player->clearInput();

	player->setPos(scrW/2., scrH/2.);
	player->setVel(0., 0.);
	player->lives = 1;
	player->bombCount = 0;
	player->invincibility = false;
	player->invincibilityTime = 0.;

	// Initialize game mode.
	gamemode = mode;
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
	if (gamemode == MODE_NORMAL)
	{
		// Set game values for Normal Mode.

		level = 1;
		nextlvlscore = 50;

		duckSpawnRate = 1000.;

		player->bulletSpeed = 12.;
		player->bulletDamage = 1.;
		player->fireRate = 325.;
		player->nextBullet = player->fireRate;

		gameTime = 60000.;
	}
	else if (gamemode == MODE_MOO)
	{
		// Set game values for MoO Mode.

		duckSpawnRate = 65.;
		
		player->bulletSpeed = 12.;
		player->bulletDamage = 1.;
		player->fireRate = 20.;
		player->nextBullet = player->fireRate;

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

	if (gamemode == MODE_NORMAL)
	{
		// If timer runs out of time, game over.
		if (gameTime <= 0)
		{
			this->gameOver();
		}

		// Decrease timer.
		gameTime -= dt;
	}

	// Increase elapsed time.
	elapsedTime += dt;
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

	// Display score.
	std::stringstream str;
	str << "Score: " << this->score;
	txtScore.setString(str.str());
	rt.draw(txtScore);

	// Display time.
	double t;
	if (gamemode == MODE_NORMAL)
	{
		t = gameTime;
	}
	else if (gamemode == MODE_MOO)
	{
		t = elapsedTime;
	}
	int hours = (int)(t/3600000);
	t -= hours*3600000.;
	int mins = (int)(t/60000);
	t -= mins*60000.;
	int secs = (int)(t/1000);
	str.str("");
	if (hours)
		str << hours << ":" << (mins < 10 ? "0" : "") << (mins == 0 ? "0" : "");
	if (mins)
		str << mins << ":" << (secs < 10 ? "0" : "") << (secs == 0 ? "0" : "");
	if (secs)
		str << secs;
	txtTime.setString(str.str());
	txtTime.setOrigin(txtTime.getGlobalBounds().width/2.f, txtTime.getGlobalBounds().height/2.f);
	rt.draw(txtTime);

	// Display lives.
	str.str("");
	str << "x " << player->lives;
	txtLives.setString(str.str());
	rt.draw(sprPlayerIcon);
	rt.draw(txtLives);

	// Display bombs available.
	for (int i = 0; i < player->bombCount; ++i)
	{
		sprBombIcon.setPosition(10.f + i*4.f, scrH-96.f);
		rt.draw(sprBombIcon);
	}

	
	if (player->invincibility)
	{
		// Display invicibility timer.

		str.str("");
		str << (int)(player->invincibilityTime/1000.);

		txtInvincibilityTimer.setString(str.str());
		txtInvincibilityTimer.setOrigin(txtInvincibilityTimer.getGlobalBounds().width/2.f,
										txtInvincibilityTimer.getGlobalBounds().height/2.f);
		txtInvincibilityTimer.setPosition(static_cast<float>(player->getPos().x), static_cast<float>(player->getPos().y + 16.));

		rt.draw(txtInvincibilityTimer);

		// Display invincibility shield indicator.
		sf::CircleShape c(26.f);
		c.setFillColor(sf::Color::Transparent);
		c.setOutlineThickness(static_cast<float>(player->invincibilityTime/player->invincibilityTimeSet * 6.));
		c.setOutlineColor(sf::Color(100, 200, 255, 235));
		c.setPosition(player->getPos().x, player->getPos().y);
		c.setOrigin(24.f, 24.f);
		rt.draw(c);
	}

	if (gamemode == MODE_NORMAL)
	{
		// Display game level.
		str.str("");
		str << "Level: " << level;
		txtLevel.setString(str.str());
		rt.draw(txtLevel);
	}
}

void CluckADuck::gmDuckSpawned(Ducky* duck)
{
	// Set up duck's properties.

	// Is it a big duck?
	bool boss = (randomdexpd(10) >= (gamemode==MODE_MOO ? 0.99 : 0.98));

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

	if (gamemode == MODE_NORMAL)
	{
		// Modify properties according to game level.

		duck->maxHealth += duck->maxHealth * (level-1)*1.75;

		if (boss)
			duck->score += 5*(level-1);
		else
			duck->score += 2*(level-1);
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
		// If boss duck, explode.
		this->addBlast(duck->getPos(), 225., 2.);

		// Increase kill count.
		this->duckBossCount++;
	}
	
	// Random powerup drop.
	Powerup* powerup = NULL;
	double r = randomdexpd( (gamemode == MODE_MOO) ? 12. : 8.);
	if (r >= (gamemode == MODE_MOO ? 0.995 : 0.975))
	{
		// Life power up.
		powerup = new Powerup();
		powerup->setupLifePowerup(this);
	}
	else if (r >= (gamemode == MODE_MOO ? 0.98 : 0.925))
	{
		// Invicibility power up.
		powerup = new Powerup();
		powerup->setupInvPowerup(this);
	}
	else if (r >= (gamemode == MODE_MOO ? 0.95 : 0.85))
	{
		// Bomb power up.
		powerup = new Powerup();
		powerup->setupBombPowerup(this);
	}
	else if (r >= (gamemode == MODE_MOO ? 0.25 : 0.125))
	{
		// Points power up.
		powerup = new Powerup();

		if (gamemode == MODE_NORMAL)
		{
			powerup->setupPointsPowerup(this, static_cast<int>((this->level-1)*5 + 10));
		}
		else if (gamemode == MODE_MOO)
		{
			powerup->setupPointsPowerup(this, 10);
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

	if (gamemode == MODE_NORMAL)
	{
		// Give some extra time.
		gameTime += 1200.;
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

	if (gamemode == MODE_NORMAL)
	{
		// Level up check.
		while (score >= nextlvlscore)
		{
			// Upgrade weapons.
			player->bulletDamage = 1. + (level * 0.75);
			player->fireRate -= 35 / (level * 0.75);
			if (player->fireRate < 16.)
				player->fireRate = 16.;

			// Make ducks spawn faster.
			duckSpawnRate -= 25 / (level*0.175);
			if (duckSpawnRate < 75.)
				duckSpawnRate = 75.;

			// Increase next level score requirement.
			nextlvlscore += level*50 + (level-1)*25;

			// Increase player level.
			++level;
		}
	}
}

void CluckADuck::gmPlayerShoot(Bullet* bullet)
{

}

void CluckADuck::gmPlayerDetonateBomb(Blast* blast)
{
	// Set damage.
	if (gamemode == MODE_NORMAL)
	{
		blast->damage = 10. + 10.*(level * 0.25);
	}
	else if (gamemode == MODE_MOO)
	{
		blast->damage = 10.;
	}

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

