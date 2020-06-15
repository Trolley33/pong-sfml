#include "Application.hpp"
#include <math.h>

#define PI 3.14159265

App::App(int width, int height, sf::String title)
{
  // Create render window.
  window.create(sf::VideoMode(width, height), title);
  window.setFramerateLimit(30);

  // Create Players
  player1.rectShape = sf::RectangleShape(sf::Vector2f(20, 60));
  player2.rectShape = sf::RectangleShape(sf::Vector2f(20, 60));

  player1.rectShape.setPosition(sf::Vector2f(50, height / 2));              // create at top left
  player2.rectShape.setPosition(sf::Vector2f(width - 50 - 20, height / 2)); // create at top right

  player1.rectShape.setFillColor(sf::Color::White);
  player2.rectShape.setFillColor(sf::Color::White);

  // Create ball
  ball.circleShape = sf::CircleShape(15.0f); // initial radius
  ball.circleShape.setFillColor(sf::Color::White);
  ball.circleShape.setOrigin(sf::Vector2f(ball.circleShape.getRadius(), ball.circleShape.getRadius()));
  ball.circleShape.setPosition(sf::Vector2f(width / 2, height / 2));
  ball.vel = sf::Vector2f(5, 0);

  // Init timer variables.
  frameDelta = 0;

  sf::Clock clock;

  while (window.isOpen())
  {
    frameDelta = clock.restart().asSeconds();

    processEvents();
    processInput();
    doMovements();

    render();
  }
}

void App::render()
{
  window.clear();

  window.draw(player1.rectShape);
  window.draw(player2.rectShape);

  window.draw(ball.circleShape);

  window.display();
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
  const int pixelsPerSeconds = 325;

  player1.verticalVel = 0;
  player2.verticalVel = 0;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
  {
    player1.verticalVel = pixelsPerSeconds;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
  {
    player1.verticalVel = -pixelsPerSeconds;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
  {
    player2.verticalVel = pixelsPerSeconds;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
  {
    player2.verticalVel = -pixelsPerSeconds;
  }
}

void App::doMovements()
{
  player1.rectShape.move(0, frameDelta * player1.verticalVel);
  player2.rectShape.move(0, frameDelta * player2.verticalVel);

  ball.circleShape.move(ball.vel);

  if (ball.circleShape.getPosition().x <= 0 || ball.circleShape.getPosition().x >= window.getSize().x)
  {
    // rotateVec(ball.vel, 180.0);
    ball.vel.x = ball.vel.x * -1;
  }
}

void rotateVec(sf::Vector2f& vec, double angle)
{
  double x2 = (vec.x * std::cos(angle * PI / 180.0))
    - (vec.y * std::sin(angle * PI / 180.0));

  double y2 = (vec.x * std::sin(angle * PI / 180.0))
    + (vec.y * std::cos(angle * PI / 180.0));

  vec.x = x2;
  vec.y = y2;
}