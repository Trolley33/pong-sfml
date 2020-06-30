#include "CollisionHandler.hpp"

namespace CollisionHandler
{
void printVector(sf::Vector2f& vec)
{
  using namespace std;
  cout << vec.x << "," << vec.y << endl;
}

bool collides(sf::CircleShape& circle, sf::RectangleShape& rect)
{
  sf::Vector2f rectEdge(circle.getPosition());
  sf::Vector2f circleCenter(circle.getPosition());

  if (circleCenter.x < rect.getPosition().x)
    rectEdge.x = rect.getPosition().x;
  else if (circleCenter.x > rect.getPosition().x + rect.getSize().x)
    rectEdge.x = rect.getPosition().x + rect.getSize().x;

  if (circleCenter.y < rect.getPosition().y)
    rectEdge.y = rect.getPosition().y;
  else if (circleCenter.y > rect.getPosition().y + rect.getSize().y)
    rectEdge.y = rect.getPosition().y + rect.getSize().y;

  if (distance(rectEdge, circleCenter) <= circle.getRadius())
  {
    return true;
  }

  return false;
}

float distance(sf::Vector2f& p1, sf::Vector2f& p2)
{
  float dx = p2.x - p1.x;
  float dy = p2.y - p1.y;
  return std::sqrt((dx * dx) + (dy * dy));
}

}