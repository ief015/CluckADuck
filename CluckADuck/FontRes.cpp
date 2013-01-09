#include "FontRes.hpp"

sf::Font* FontRes::FONT_TITLE = NULL;
sf::Font* FontRes::FONT_HANDWRITING = NULL;
sf::Font* FontRes::FONT_ARCADESTYLE = NULL;

bool FontRes::StaticInit()
{
	FONT_TITLE = new sf::Font();
	FONT_HANDWRITING = new sf::Font();
	FONT_ARCADESTYLE = new sf::Font();

	if (!FONT_TITLE->loadFromFile("res/adventure.ttf"))
		return false;
	if (!FONT_HANDWRITING->loadFromFile("res/segoeprb.ttf"))
		return false;
	if (!FONT_ARCADESTYLE->loadFromFile("res/04B11.ttf"))
		return false;

	return true;
}

void FontRes::StaticQuit()
{
	if (FONT_TITLE)
		delete FONT_TITLE;

	if (FONT_HANDWRITING)
		delete FONT_HANDWRITING;

	if (FONT_ARCADESTYLE)
		delete FONT_ARCADESTYLE;
}
