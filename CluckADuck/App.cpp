#include "App.hpp"


App::App()
: running(false)
{

}

App::~App()
{
	this->kill();
}



void App::initialize(unsigned w, unsigned h)
{
	rw = new sf::RenderWindow();
	running = this->onInitialize(w, h);
}


void App::terminate()
{
	running = false;
}


void App::kill()
{
	this->onTerminate();

	rw->close();
	delete rw;
}



void App::processEvents()
{
	sf::Event ev;
	while (rw->pollEvent(ev))
	switch(ev.type)
	{

	case sf::Event::Closed:
		this->onWindowClose();
		break;

	case sf::Event::Resized:
		this->onWindowResize(ev.size.width, ev.size.height);
		break;

	case sf::Event::MouseButtonPressed:
		this->onMouseDown(ev.mouseButton.x, ev.mouseButton.y, ev.mouseButton.button);
		break;

	case sf::Event::MouseButtonReleased:
		this->onMouseUp(ev.mouseButton.x, ev.mouseButton.y, ev.mouseButton.button);
		break;

	case sf::Event::MouseMoved: {
		static int px = ev.mouseMove.x, py = ev.mouseMove.y;
		this->onMouseMove(ev.mouseMove.x, ev.mouseMove.y, ev.mouseMove.x - px, ev.mouseMove.y - py);
		px = ev.mouseMove.x;
		py = ev.mouseMove.y;
		break; }

	case sf::Event::MouseWheelMoved:
		this->onMouseWheel(ev.mouseWheel.x, ev.mouseWheel.y, ev.mouseWheel.delta);
		break;

	case sf::Event::KeyPressed:
		this->onKeyDown(ev.key.code);
		break;

	case sf::Event::KeyReleased:
		this->onKeyUp(ev.key.code);
		break;

	}
}

void App::tick(float ms)
{
	this->onTick(ms);
	mstime = msclk.restart();
}

void App::render()
{
	rw->popGLStates();
	this->onDrawGL();
	rw->pushGLStates();
	this->onDrawSF();

	rw->display();
	fpstime = fpsclk.restart();
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


bool App::onInitialize(unsigned w, unsigned h)
{
	return true;
}


void App::onTerminate()
{

}


void App::onTick(float ms)
{

}


void App::onDrawSF()
{

}


void App::onDrawGL()
{

}


void App::onMouseDown(int x, int y, unsigned b)
{

}


void App::onMouseUp(int x, int y, unsigned b)
{

}


void App::onMouseWheel(int x, int y, int d)
{

}


void App::onMouseMove(int x, int y, int dx, int dy)
{

}


void App::onKeyDown(int key)
{

}


void App::onKeyUp(int key)
{

}


void App::onWindowResize(unsigned w, unsigned h)
{

}


void App::onWindowClose()
{
	this->terminate();
}
