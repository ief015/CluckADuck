#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

class App
{
private:
	sf::RenderWindow* rw;
	sf::Time fpstime, mstime;
	sf::Clock fpsclk, msclk;
	bool running;

	void kill();

protected:
	inline sf::RenderWindow& getRW() { return *rw; }

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
	App();
	virtual ~App();

	void initialize(unsigned w, unsigned h);
	void terminate();
	void processEvents();
	void tick(float ms);
	void render();

	inline bool isRunning() const { return running; }
	inline float getFPS() const { return 1000.f / (fpstime.asSeconds() * 1000.f); }
	inline float getMS() const { return mstime.asSeconds() * 1000.f; }
};
