#pragma once

#include "Widget.hpp"

class ClickableText : public Widget
{
private:
	bool clicked;
	sf::Color colIdle;
	sf::Color colHover;
	sf::Color colClick;

public:
	sf::Text text;

	ClickableText() : Widget()
	, clicked(false)
	, colIdle(200, 200, 200, 255)
	, colHover(215, 215, 215, 255)
	, colClick(250, 250, 250, 255)
	{

	}

	virtual ~ClickableText()
	{

	}

	inline bool pollClick()
	{
		if (this->clicked)
		{
			this->clicked = false;
			return true;
		}
		return false;
	}

	void updateSize()
	{
		this->width = text.getGlobalBounds().width;
		this->height = text.getGlobalBounds().height + 8.;
	}

	void setString(const std::string& str)
	{
		text.setString(str);
		this->updateSize();
	}

	void setColourIdle(const sf::Color& col)
	{
		colIdle = col;
	}

	void setColourHover(const sf::Color& col)
	{
		colHover = col;
	}

	void setColourClick(const sf::Color& col)
	{
		colClick = col;
	}
	

	virtual void onMouseDown(int x, int y, unsigned b) { }
	virtual void onMouseUp(int x, int y, unsigned b) { }
	virtual void onMouseWheel(int x, int y, int d) { }
	virtual void onMouseMove(int x, int y, int dx, int dy) { }
	virtual void onKeyDown(int key) { }
	virtual void onKeyUp(int key) { }
	virtual void onKeyText(int key) { }

protected:

	virtual void onUpdate(float dt) { }

	virtual void onDraw(sf::RenderTarget& rt, double scrx, double scry)
	{
		// Position text.
		text.setPosition(static_cast<float>(scrx), static_cast<float>(scry));

		// Get text colour.
		sf::Color* col = &colIdle;
		if (this->isMouseHolding())
			col = &colClick;
		else if (this->isMouseHovering())
			col = &colHover;

		// Set text colour and draw.
		text.setColor(*col);
		rt.draw(text);
	}

	virtual void onClicked(int x, int y, int b)
	{
		clicked = true;
	}

	virtual void onFocusGained()
	{

	}

	virtual void onFocusLost()
	{

	}

	virtual void onMouseEnter()
	{

	}

	virtual void onMouseLeave()
	{

	}
};
