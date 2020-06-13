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

struct Player
{
  sf::RectangleShape rectShape;
  int verticalVel;
};

class App
{
private:
  sf::RenderWindow window;

  float frameDelta;

  Player player1;
  Player player2;

  Ball ball;

public:
  App(int width, int height, sf::String title);

  void processEvents();
  void processInput();
  void render();
};

#endif // __APPLICATION_H__
