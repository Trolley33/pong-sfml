#include "Application.hpp"

App::App(int width, int height, sf::String title)
{
  // Create render window.
  window.create(sf::VideoMode(width, height), title);
  window.setFramerateLimit(30);

  // Create Players
  player1.setPosition(sf::Vector2f(10, 10));              // create at top left
  player2.setPosition(sf::Vector2f(width - 10 - 20, 10)); // create at top right

  player1.setSize(sf::Vector2f(20, 60));
  player2.setSize(sf::Vector2f(20, 60));

  player1.setFillColor(sf::Color::White);
  player2.setFillColor(sf::Color::White);

  // Create ball
  ball.circleShape = sf::CircleShape(15.0f); // initial radius
  ball.circleShape.setFillColor(sf::Color::White);
  ball.circleShape.setOrigin(sf::Vector2f(ball.circleShape.getRadius(), ball.circleShape.getRadius()));
  ball.circleShape.setPosition(sf::Vector2f(width / 2, height / 2));
  ball.vel = sf::Vector2f(1, 0);

  // Init timer variables.
  frameDelta = 0;

  sf::Clock clock;

  while (window.isOpen())
  {
    processEvents();
    processInput();

    frameDelta = clock.restart().asSeconds();
    render();
  }
}

void App::render()
{
  window.clear();

  window.draw(player1);
  window.draw(player2);
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
  const int pixelsPerSeconds = 150;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
  {
    player1.move(0, frameDelta * pixelsPerSeconds);
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
  {
    player1.move(0, frameDelta * -pixelsPerSeconds);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
  {
    player2.move(0, frameDelta * pixelsPerSeconds);
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
  {
    player2.move(0, frameDelta * -pixelsPerSeconds);
  }
}