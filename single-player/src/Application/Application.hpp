#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "SFML/Graphics.hpp"
#include "SFML/Main.hpp"
#include "SFML/System.hpp"

class App
{
private:
	sf::RenderWindow window;

	sf::Int64 previousUpdate;
	sf::Int64 elapsedTime;

	sf::Int32 frameDelta;

public:
	App(int width, int height, sf::String title);

	void update();
	void fixedUpdate();

	void processEvents();
	void processInput();
};

#endif // __APPLICATION_H__