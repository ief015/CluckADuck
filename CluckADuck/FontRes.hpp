#pragma once

#include <SFML/Graphics.hpp>

class FontRes
{
private:
	static sf::Font* FONT_TITLE;
	static sf::Font* FONT_HANDWRITING;
	static sf::Font* FONT_ARCADESTYLE;

public:

	// For the main menu.
	inline static const sf::Font& getTitleFont() { return *FONT_TITLE; }

	// Typically used for showing hints.
	inline static const sf::Font& getHandWritingFont() { return *FONT_HANDWRITING; }

	// For displaying game stats, or to just look arcade-y.
	inline static const sf::Font& getArcadeFont() { return *FONT_ARCADESTYLE; }

	static bool StaticInit();
	static void StaticQuit();
};
