#include "Application.hpp"
#include "CollisionHandler/CollisionHandler.hpp"
#include <math.h>

#define PI 3.14159265

const int width = 640;
const int height = 640;

App::App(sf::String title)
{
  // Create render window.
  window.create(sf::VideoMode(width, height), title);
  window.setFramerateLimit(60);

  initVariables();

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

void App::initVariables()
{
  player1.score = 0;
  player2.score = 0;

  // * Load font from file.
  if (!font.loadFromFile("assets/fonts/trebuc.ttf"))
  {
    // TODO: properly handle error.
  }

  text.setFont(font);
  text.setCharacterSize(42);
  text.setStyle(sf::Text::Style::Bold);
  text.setFillColor(sf::Color::White);

  // * Set text value and position based on text width.
  text.setString(std::to_string(player1.score) + " - " + std::to_string(player2.score));
  text.setPosition((window.getSize().x / 2) - (text.getLocalBounds().width / 2), 10);

  // Create Players
  player1.rectShape = sf::RectangleShape(sf::Vector2f(20, 60));
  player2.rectShape = sf::RectangleShape(sf::Vector2f(20, 60));

  player1.rectShape.setPosition(sf::Vector2f(50, (height / 2) - (player1.rectShape.getSize().y / 2)));
  player2.rectShape.setPosition(sf::Vector2f(width - 50 - 20, (height / 2) - (player2.rectShape.getSize().y / 2)));

  player1.rectShape.setFillColor(sf::Color::White);
  player2.rectShape.setFillColor(sf::Color::White);

  // Create ball
  ball.circleShape = sf::CircleShape(5.0f); // initial radius
  ball.circleShape.setFillColor(sf::Color::White);
  ball.circleShape.setOrigin(sf::Vector2f(ball.circleShape.getRadius(), ball.circleShape.getRadius()));
  ball.circleShape.setPosition(sf::Vector2f(width / 2, height / 2));
  ball.maxVel = 700;
  ball.vel = sf::Vector2f(ball.maxVel, 0);

  // Init timer variables.
  frameDelta = 0;
}

void App::render()
{
  window.clear();

  window.draw(player1.rectShape);
  window.draw(player2.rectShape);

  window.draw(ball.circleShape);

  window.draw(text);

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
  // * Move each player based on their velocity and frame time difference.
  player1.rectShape.move(0, frameDelta * player1.verticalVel);
  player2.rectShape.move(0, frameDelta * player2.verticalVel);

  // ! BOT MOVEMENT, DELETE!
  player2.rectShape.setPosition(player2.rectShape.getPosition().x, ball.circleShape.getPosition().y - (player2.rectShape.getSize().y / 2));

  // * Clamp the movement of players between window height.
  player1.rectShape.setPosition(player1.rectShape.getPosition().x, clampValue(player1.rectShape.getPosition().y, 0, height - player1.rectShape.getSize().y));
  player2.rectShape.setPosition(player2.rectShape.getPosition().x, clampValue(player2.rectShape.getPosition().y, 0, height - player2.rectShape.getSize().y));

  sf::Vector2f originalPosition = ball.circleShape.getPosition();
  ball.circleShape.move(frameDelta * ball.vel);

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
    // * Left wall (quicker to check against width / 2 than to get the radius and subtract again).
    if (ball.circleShape.getPosition().x < (width / 2))
    {
      player1.score += 1;
    }
    else
    {
      player2.score += 1;
    }

    // * Set text value and position based on text width.
    text.setString(std::to_string(player1.score) + " - " + std::to_string(player2.score));
    text.setPosition((window.getSize().x / 2) - (text.getLocalBounds().width / 2), 10);

    // * If we hit the x walls, reset ball to center.
    ball.circleShape.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    ball.vel.x = ball.maxVel;
    ball.vel.y = 0;

    player1.rectShape.setPosition(sf::Vector2f(50, (height / 2) - (player1.rectShape.getSize().y / 2)));
    player2.rectShape.setPosition(sf::Vector2f(width - 50 - 20, (height / 2) - (player2.rectShape.getSize().y / 2)));

    return;
  }

  // * If our ball is colliding with the left-most paddle.
  if (CollisionHandler::collides(ball.circleShape, player1.rectShape))
  {
    // * Reverse the ball's motion, move the ball to the boundary between the objects.
    ball.vel.x *= -1;
    ball.circleShape.setPosition(player1.rectShape.getPosition().x + ball.circleShape.getRadius() + player1.rectShape.getSize().x, ball.circleShape.getPosition().y);

    // * If our player is moving, add some of the player's vertical velocity to the ball.
    if (player1.verticalVel != 0)
    {
      ball.vel.y = ball.vel.y + (player1.verticalVel * 0.9);
      if (ball.vel.y > (ball.maxVel / 2))
      {
        ball.vel.y = (ball.maxVel / 2);
      }
      else if (ball.vel.y < -(ball.maxVel / 2))
      {
        ball.vel.y = -(ball.maxVel / 2);
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
      ball.vel.y = ball.vel.y + (player2.verticalVel * 0.9);
      if (ball.vel.y > (ball.maxVel / 2))
      {
        ball.vel.y = (ball.maxVel / 2);
      }
      else if (ball.vel.y < -(ball.maxVel / 2))
      {
        ball.vel.y = -(ball.maxVel / 2);
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

void clampValue(float& val, float min, float max)
{
  if (val > max)
    val = max;
  else if (val < min)
    val = min;
}

float clampValue(float val, float min, float max)
{
  if (val > max)
    return max;
  else if (val < min)
    return min;

  return val;
}
