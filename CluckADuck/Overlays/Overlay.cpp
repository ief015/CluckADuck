#include "Overlay.hpp"


Overlay::Overlay()
: manager(NULL)
, hogControls(false)
, queueDeletion(false)
{
	ui = new Widget();
}

Overlay::~Overlay() 
{
	delete ui;
}


void Overlay::close()
{
	this->queueDeletion = true;
}


void Overlay::onShown()
{
	
}


void Overlay::onHide()
{

}


void Overlay::onUpdate(float dt)
{
	ui->update(dt);
}


void Overlay::onDraw(sf::RenderTarget& rt)
{
	ui->draw(rt);
}


void Overlay::onMouseDown(int x, int y, unsigned b)
{
	ui->onMouseDown(x, y, b);
}


void Overlay::onMouseUp(int x, int y, unsigned b)
{
	ui->onMouseUp(x, y, b);
}


void Overlay::onMouseWheel(int x, int y, int d)
{
	ui->onMouseWheel(x, y, d);
}


void Overlay::onMouseMove(int x, int y, int dx, int dy)
{
	ui->onMouseMove(x, y, dx, dy);
}


void Overlay::onKeyDown(int key)
{
	ui->onKeyDown(key);
}


void Overlay::onKeyUp(int key)
{
	ui->onKeyUp(key);
}

void Overlay::onKeyText(int ch)
{
	ui->onKeyText(ch);
}
