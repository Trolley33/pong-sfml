#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "SFML/Graphics.hpp"
#include "SFML/Main.hpp"
#include "SFML/System.hpp"

struct Ball
{
  sf::CircleShape circleShape;
  sf::Vector2f vel;
};
class App
{
private:
  sf::RenderWindow window;

  float frameDelta;

  sf::RectangleShape player1;
  sf::RectangleShape player2;
  Ball ball;

public:
  App(int width, int height, sf::String title);

  void processEvents();
  void processInput();
  void render();
};

#endif // __APPLICATION_H__
