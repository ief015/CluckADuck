#pragma once
#include "OverlayManager.hpp"

#include <SFML/Graphics.hpp>

class Overlay
{
private:
	friend OverlayManager;
	OverlayManager* manager;
	bool hogControls;
	bool queueDeletion;

protected:
	inline void overrideControls(bool enable) { hogControls = enable; }

public:
	Overlay() : manager(NULL), hogControls(false), queueDeletion(false) {}
	virtual ~Overlay() {}

	inline void close() { queueDeletion = true; }
	inline bool isShowing() const { return manager != NULL; }
	inline bool isOverridingControls() const { return hogControls; }

	virtual void onShown() {}
	virtual void onHide() {}

	virtual void onUpdate(float dt) {}
	virtual void onDraw(sf::RenderTarget& rt) {}

	virtual void onMouseDown(int x, int y, unsigned b) { }
	virtual void onMouseUp(int x, int y, unsigned b) { }
	virtual void onMouseWheel(int x, int y, int d) { }
	virtual void onMouseMove(int x, int y, int dx, int dy) { }
	virtual void onKeyDown(int key) { }
	virtual void onKeyUp(int key) { }
};
