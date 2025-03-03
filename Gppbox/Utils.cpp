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



float Utils::toLength(const sf::Vector2i& source)
{
    return sqrt((source.x * source.x) + (source.y * source.y));
}

float Utils::toLength(const sf::Vector2f& source)
{
    return sqrt((source.x * source.x) + (source.y * source.y));
}

float Utils::toAngle(const sf::Vector2i& source)
{
    float angle = atan2(source.y, source.x);
    if (angle < 0.0f) angle += (M_PI + M_PI);
    return angle * RadToDeg;
}

float Utils::toAngle(const sf::Vector2f& source)
{
    float angle = atan2(source.y, source.x);
    if (angle < 0.0f) angle += (M_PI + M_PI);
    return angle * RadToDeg;
}

sf::Vector2f Utils::fromAngle(float angle)
{
    float rad = angle * DegToRad;
    return normalize(sf::Vector2f {
        cos(rad),
        sin(rad)
    });
}




sf::Vector2f Utils::normalize(const sf::Vector2i& source)
{
    float length = toLength(source);
    if (length != 0) return sf::Vector2f(source.x / length, source.y / length);
    else return sf::Vector2f(source.x, source.y);
}

sf::Vector2f Utils::normalize(const sf::Vector2f& source)
{
    float length = toLength(source);
    if (length != 0) return sf::Vector2f(source.x / length, source.y / length);
    else return source;
}

sf::Vector2i Utils::rotate(const sf::Vector2i& source, float angle)
{
    float length = toLength(source);
    float target = fmod(toAngle(source) + angle, 360.0f);
    sf::Vector2f res = fromAngle(target) * length;
    return sf::Vector2i{ int(round(res.x)), int(round(res.y)) };
}

sf::Vector2f Utils::rotate(const sf::Vector2f& source, float angle)
{
    float length = toLength(source);
    float target = fmod(toAngle(source) + angle, 360.0f);
    return fromAngle(target) * length;
}