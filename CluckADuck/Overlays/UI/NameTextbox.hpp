#pragma once

#include "Widget.hpp"
#include "../../FontRes.hpp"

#define NAME_MAX_CHARS 12

class NameTextbox : public Widget
{
private:
	char name[NAME_MAX_CHARS+1];
	int cursor;
	sf::Text txt;

public:
	NameTextbox() : Widget()
	, cursor(0)
	{
		memset(this->name, 0, sizeof(this->name));

		txt = sf::Text("", FontRes::getArcadeFont(), 60);
		txt.setColor(sf::Color(255, 180, 0));
	}

	virtual ~NameTextbox()
	{

	}

	virtual void onMouseDown(int x, int y, unsigned b) { }
	virtual void onMouseUp(int x, int y, unsigned b) { }
	virtual void onMouseWheel(int x, int y, int d) { }
	virtual void onMouseMove(int x, int y, int dx, int dy) { }
	virtual void onKeyDown(int key)
	{
		switch (key)
		{

		case sf::Keyboard::Back:
			// Check if not at beginning.
			if (this->cursor == 0)
				break;

			// Move cursor left.
			this->cursor--;

			this->name[this->cursor] = '\0';

			break;
			
		case sf::Keyboard::Return:

			break;

		case sf::Keyboard::Escape:

			break;

		}
	}

	virtual void onKeyUp(int key) { }
	virtual void onKeyText(int ch)
	{
		ch = (char)ch;
		if (isprint(ch))
		{
			this->name[this->cursor] = ch;

			if (this->cursor == NAME_MAX_CHARS)
				return;

			this->cursor++;
		}
	}

	inline const char* getString() const { return this->name; }

protected:

	virtual void onUpdate(float dt) { }

	virtual void onDraw(sf::RenderTarget& rt, double scrx, double scry)
	{
		sf::Vector2f pos(static_cast<float>(scrx), static_cast<float>(scry));
		
		for (int i = 0; i < NAME_MAX_CHARS; ++i)
		{
			// Set and increment position.
			txt.setPosition(pos);
			pos.x += 64;

			char c = this->name[i];

			txt.setString(sf::String(c ? c : '_'));
			rt.draw(txt);
		}

	}

	virtual void onClicked(int x, int y, int b) { }
	virtual void onFocusGained() { }
	virtual void onFocusLost() { }
	virtual void onMouseEnter() { }
	virtual void onMouseLeave() { }
};
