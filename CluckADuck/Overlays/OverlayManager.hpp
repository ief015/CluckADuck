#pragma once
#include <SFML/Graphics.hpp>

class Overlay;
class OverlayManager
{
private:
	std::vector<Overlay*> overlays;

public:
	OverlayManager();
	virtual ~OverlayManager();

	void pushOverlay(Overlay* ov);
	void removeOverlay(Overlay* ov);

	void update(float dt);
	void draw(sf::RenderTarget& rt);

	bool mouseDown(int x, int y, unsigned b);
	bool mouseUp(int x, int y, unsigned b);
	bool mouseWheel(int x, int y, int d);
	bool mouseMove(int x, int y, int dx, int dy);
	bool keyDown(int key);
	bool keyUp(int key);
};
