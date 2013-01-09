#include "OverlayManager.hpp"

#include "Overlay.hpp"

OverlayManager::OverlayManager()
{
}

OverlayManager::~OverlayManager()
{
	overlays.clear();
}


void OverlayManager::pushOverlay(Overlay* ov)
{
	overlays.push_back(ov);
	ov->manager = this;
	ov->queueDeletion = false;
	ov->onShown();
}

void OverlayManager::removeOverlay(Overlay* ov)
{
	for (int i = 0, sz = overlays.size(); i < sz; ++i)
	{
		Overlay* o = overlays[i];
		if (o == ov)
		{
			o->onHide();

			o->manager = NULL;
			overlays.erase(overlays.begin()+i);
			break;
		}
	}
}


void OverlayManager::update(float dt)
{
	for (int i = 0, sz = overlays.size(); i < sz; ++i)
	{
		if (overlays[i]->queueDeletion)
		{
			this->removeOverlay(overlays[i]);
			--i; --sz;
		}
	}

	for (int i = 0, sz = overlays.size(); i < sz; ++i)
		overlays[i]->onUpdate(dt);
}

void OverlayManager::draw(sf::RenderTarget& rt)
{
	for (int i = 0, sz = overlays.size(); i < sz; ++i)
		overlays[i]->onDraw(rt);
}


bool OverlayManager::mouseDown(int x, int y, unsigned b)
{
	for (int i = overlays.size()-1; i >= 0; --i)
	{
		overlays[i]->onMouseDown(x, y, b);
		if (overlays[i]->hogControls)
			return true;
	}
	return false;
}

bool OverlayManager::mouseUp(int x, int y, unsigned b)
{
	for (int i = overlays.size()-1; i >= 0; --i)
	{
		overlays[i]->onMouseUp(x, y, b);
		if (overlays[i]->hogControls)
			return true;
	}
	return false;
}

bool OverlayManager::mouseWheel(int x, int y, int d)
{
	for (int i = overlays.size()-1; i >= 0; --i)
	{
		overlays[i]->onMouseWheel(x, y, d);
		if (overlays[i]->hogControls)
			return true;
	}
	return false;
}

bool OverlayManager::mouseMove(int x, int y, int dx, int dy)
{
	for (int i = overlays.size()-1; i >= 0; --i)
	{
		overlays[i]->onMouseMove(x, y, dx, dy);
		if (overlays[i]->hogControls)
			return true;
	}
	return false;
}

bool OverlayManager::keyDown(int key)
{
	for (int i = overlays.size()-1; i >= 0; --i)
	{
		overlays[i]->onKeyDown(key);
		if (overlays[i]->hogControls)
			return true;
	}
	return false;
}

bool OverlayManager::keyUp(int key)
{
	for (int i = overlays.size()-1; i >= 0; --i)
	{
		overlays[i]->onKeyUp(key);
		if (overlays[i]->hogControls)
			return true;
	}
	return false;
}

bool OverlayManager::keyText(int ch)
{
	for (int i = overlays.size()-1; i >= 0; --i)
	{
		overlays[i]->onKeyText(ch);
		if (overlays[i]->hogControls)
			return true;
	}
	return false;
}
