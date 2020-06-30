#include <SFML/Main.hpp>
#include <math.h>

namespace CollisionHandler
{
void printVector(sf::Vector2f&);

bool collides(sf::CircleShape&, sf::RectangleShape&);

float distance(sf::Vector2f& p1, sf::Vector2f& p2);
}