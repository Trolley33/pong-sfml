#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "SFML/Graphics.hpp"
#include "SFML/Main.hpp"
#include "SFML/System.hpp"

struct Ball
{
  sf::CircleShape circleShape;
  sf::Vector2f vel;
  float maxVel;
};

struct Player
{
  sf::RectangleShape rectShape;
  int verticalVel;
};

void rotateVec(sf::Vector2f& vec, double angle);
void setMagnitude(sf::Vector2f& vec, float newMag);
void clampValue(float& val, float min, float max);
float clampValue(float val, float min, float max);

class App
{
private:
  sf::RenderWindow window;

  float frameDelta;

  Player player1;
  Player player2;

  Ball ball;

  sf::Font font;
  sf::Text text;

  int scores[2];

public:
  App(sf::String title);

  void processEvents();
  void processInput();
  void doMovements();
  void render();
};

#endif // __APPLICATION_H__
