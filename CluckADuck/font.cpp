#include "font.hpp"

sf::Font FontRes::DEFAULT_FONT = sf::Font();

bool FontRes::StaticInit()
{
	if (!DEFAULT_FONT.loadFromFile("res/arial.ttf"))
		return false;
	return true;
}

void FontRes::StaticQuit()
{

}
