#pragma once
#include "SFML/System/Vector2.hpp"

class Entity;

class Utils
{
private:
	#define RadToDeg (180.0f / M_PI)
	#define DegToRad (M_PI / 180.0f)

public:
	static bool isFullBody(Entity* entity, int gridx, int gridy);
	template <typename T> static int sign(T value);

	static float toLength(const sf::Vector2i& source);
	static float toLength(const sf::Vector2f& source);
	static float toAngle(const sf::Vector2i& source);
	static float toAngle(const sf::Vector2f& source);
	static sf::Vector2f fromAngle(float angle);

	static sf::Vector2f normalize(const sf::Vector2i& source);
	static sf::Vector2f normalize(const sf::Vector2f& source);
	static sf::Vector2i rotate(const sf::Vector2i& source, float angle);
	static sf::Vector2f rotate(const sf::Vector2f& source, float angle);
};