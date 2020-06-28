#include "Application.hpp"
#include "CollisionHandler/CollisionHandler.hpp"
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
  ball.circleShape = sf::CircleShape(5.0f); // initial radius
  ball.circleShape.setFillColor(sf::Color::White);
  ball.circleShape.setOrigin(sf::Vector2f(ball.circleShape.getRadius(), ball.circleShape.getRadius()));
  ball.circleShape.setPosition(sf::Vector2f(width / 2, height / 2));
  ball.maxVel = 15;
  ball.vel = sf::Vector2f(ball.maxVel, 0);

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
  // * Number of pixels the paddles move a second.
  const int pixelsPerSeconds = 325;

  // * Set to 0 by default, only move if button is held.
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
  std::cout << "\r" << ball.vel.x << "," << ball.vel.y;

  player1.rectShape.move(0, frameDelta * player1.verticalVel);
  player2.rectShape.move(0, frameDelta * player2.verticalVel);

  sf::Vector2f originalPosition = ball.circleShape.getPosition();

  ball.circleShape.move(ball.vel);

  // * Check for edge of screen collisions.
  if (ball.circleShape.getPosition().y - ball.circleShape.getRadius() <= 0 || ball.circleShape.getPosition().y + ball.circleShape.getRadius() >= window.getSize().y)
  {
    ball.vel.y *= -1;
    // * If we move off screen, in this frame, move back to where we were after adjusting velocity.
    ball.circleShape.setPosition(originalPosition);
    return;
  }

  if (ball.circleShape.getPosition().x - ball.circleShape.getRadius() <= 0 || ball.circleShape.getPosition().x + ball.circleShape.getRadius() >= window.getSize().x)
  {
    // * If we hit the x walls, reset ball to center.
    ball.circleShape.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    ball.vel.x = ball.maxVel;
    ball.vel.y = 0;

    player1.rectShape.setPosition(50, window.getSize().y / 2);
    player2.rectShape.setPosition(window.getSize().x - player2.rectShape.getSize().x - 50, window.getSize().y / 2);

    return;
  }

  // * If our ball is colliding with the left-most panel.
  if (CollisionHandler::collides(ball.circleShape, player1.rectShape))
  {
    // * Reverse the ball's motion, move the ball to the boundary between the objects.
    ball.vel.x *= -1;
    ball.circleShape.setPosition(player1.rectShape.getPosition().x + ball.circleShape.getRadius() + player1.rectShape.getSize().x, ball.circleShape.getPosition().y);

    // * If our player is moving, add some of the player's vertical velocity to the ball.
    if (player1.verticalVel != 0)
    {
      ball.vel.y = ball.vel.y + (player1.verticalVel * 0.05);
      if (ball.vel.y > (ball.maxVel / 3))
      {
        ball.vel.y = (ball.maxVel / 3);
      }
      else if (ball.vel.y < -(ball.maxVel / 3))
      {
        ball.vel.y = -(ball.maxVel / 3);
      }
      // * Clamp the ball's total velocity (magnitude of vector) to the max velocity.
      setMagnitude(ball.vel, ball.maxVel);
    }
    return;
  }

  // * As above but for the right paddle.
  if (CollisionHandler::collides(ball.circleShape, player2.rectShape))
  {
    ball.vel.x *= -1;
    ball.circleShape.setPosition(player2.rectShape.getPosition().x - ball.circleShape.getRadius(), ball.circleShape.getPosition().y);
    if (player2.verticalVel != 0)
    {
      ball.vel.y = ball.vel.y + (player2.verticalVel * 0.05);
      if (ball.vel.y > (ball.maxVel / 3))
      {
        ball.vel.y = (ball.maxVel / 3);
      }
      else if (ball.vel.y < -(ball.maxVel / 3))
      {
        ball.vel.y = -(ball.maxVel / 3);
      }
      setMagnitude(ball.vel, ball.maxVel);
    }
    return;
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

// * Takes a vector by reference and scales it's directions to have desired magnitude.
void setMagnitude(sf::Vector2f& vec, float newMag)
{
  float mag = std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
  vec.x = (vec.x / mag) * newMag;
  vec.y = (vec.y / mag) * newMag;
}
