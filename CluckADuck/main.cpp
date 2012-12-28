#include <stdio.h>

#include "App.hpp"

#include "CluckADuck.hpp"

int main(int argc, char* args[])
{
	App* app;
	const float TICKRATE = 1.f/60;

	app = new CluckADuck();
	app->initialize(800, 600);

	if (!app->isRunning())
	{
		delete app;
		return 1;
	}
	
	sf::Clock clk;
	sf::Time clktime;
	do
	{
		app->processEvents();
		while ((clktime += clk.restart()) >= sf::seconds(TICKRATE))
		{
			clktime -= sf::seconds(TICKRATE);
			app->tick(TICKRATE*1000);
		}
		app->render();

		sf::sleep(sf::milliseconds(1));
	}
	while (app->isRunning());

	delete app;

	return 0;
}