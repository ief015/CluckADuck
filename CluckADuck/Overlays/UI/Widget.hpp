#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Widget
{
private:
	// Back is focused/top.
	std::vector<Widget*> widgets;
	Widget* parent;

	bool down;
	Widget* hover;
	int mouseX, mouseY;

public:
	Widget();
	virtual ~Widget();

	// Properties.
	double x, y, width, height;

	inline const Widget* getParent() const { return parent; }

	inline bool hasChildren() const { return widgets.size() > 0; }
	inline size_t getNumOfChildren() const { return widgets.size(); }

	const Widget* getFocused() const;
	bool isFocused() const;

	inline bool isMouseHolding() const { return this->down; }
	bool isMouseHovering() const;

	inline int getRelativeMouseX() const { return this->mouseX; }
	inline int getRelativeMouseY() const { return this->mouseY; }
	

	// Methods.
	void addWidget(Widget* widget);
	void removeWidget(Widget* widget);
	bool hasWidget(const Widget* widget) const;
	
	void update(float dt);
	void draw(sf::RenderTarget& rt);

	// Injection events.
	virtual void onMouseDown(int x, int y, unsigned b);
	virtual void onMouseUp(int x, int y, unsigned b);
	virtual void onMouseWheel(int x, int y, int d);
	virtual void onMouseMove(int x, int y, int dx, int dy);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);
	virtual void onKeyText(int ch);

protected:

	// Widget events.
	virtual void onUpdate(float dt);
	virtual void onDraw(sf::RenderTarget& rt, double scrx, double scry);
	virtual void onClicked(int x, int y, int b);
	virtual void onFocusGained();
	virtual void onFocusLost();
	virtual void onMouseEnter();
	virtual void onMouseLeave();
};
