#pragma once

#include <SFML/Graphics.hpp>

class FontRes
{
private:
	static sf::Font DEFAULT_FONT;

public:

	inline static const sf::Font& getDefaultFont() { return DEFAULT_FONT; }

	static bool StaticInit();
	static void StaticQuit();
};
