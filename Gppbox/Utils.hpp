#pragma once
#include "SFML/System/Vector2.hpp"

class Entity;

class Utils
{
public:
	static bool isFullBody(Entity* entity, int gridx, int gridy);

	static float toLength(const sf::Vector2f& source);
	static sf::Vector2f normalize(const sf::Vector2f& source);
	static float toAngle(const sf::Vector2f& source);
};