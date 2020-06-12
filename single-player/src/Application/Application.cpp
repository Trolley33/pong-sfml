#include "Application.hpp"

App::App(int width, int height, sf::String title)
{
	// Create render window.
	window.create(sf::VideoMode(width, height), title);

	// Init timer variables.
	previousUpdate = elapsedTime = frameDelta = 0;
	sf::Clock clock;

	// Number of game ticks per second.
	const int tickRate = 30;

	while (window.isOpen())
	{
		// Process windows events (move, resize, close).
		processEvents();
		// Process user inputs.
		processInput();

		// Calculate time variables.
		frameDelta = clock.getElapsedTime().asMilliseconds();
		clock.restart();
		elapsedTime = elapsedTime + frameDelta;
		// If we are due a game tick,
		if (elapsedTime - previousUpdate >= (1000 / (double)tickRate))
		{
			this->fixedUpdate();
			previousUpdate = elapsedTime;
		}
		this->update();
	}
}

void App::update()
{
}

void App::fixedUpdate()
{
}

void App::processEvents()
{
	sf::Event e;
	while (window.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			window.close();
		}
	}
}

void App::processInput()
{
}