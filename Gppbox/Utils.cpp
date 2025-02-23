#define _USE_MATH_DEFINES
#include <math.h>

#include "Utils.hpp"
#include "Entity.hpp"
#include "M.hpp"
#include "C.hpp"

bool Utils::isFullBody(Entity* entity, int gridx, int gridy)
{
	FULL_CHECK(entity, (xpos == gridx && int(ypos) == gridy));
}

float Utils::toLength(const sf::Vector2f& source)
{
    return sqrt((source.x * source.x) + (source.y * source.y));
}

sf::Vector2f Utils::normalize(const sf::Vector2f& source)
{
    float length = toLength(source);
    if (length != 0) return sf::Vector2f(source.x / length, source.y / length);
    else return source;
}

float Utils::toAngle(const sf::Vector2f& source)
{
    return std::acos((source.x * source.x) / (source.x * source.x + source.y * source.y)) * (180 / M_PI);
}