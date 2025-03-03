#pragma once
#include <string>
#include "SFML/System/Vector2.hpp"
#include "Component.hpp"

class SpawnOnDestroy : public Component
{
public:
	float lifetime = 0.0f;
	std::string texture = "";
	sf::Vector2f textureSize = sf::Vector2f{ 0, 0 };

	float sheight = 0.0f;
	float swidth = 0.0f;
	float lifepoints = 0.0f;

	float frx = 0.84f;
	float fry = 1.0f;
	bool usePhysics = true;
	float gravy = 1.0f;

	SpawnOnDestroy(Entity* entity);
	~SpawnOnDestroy();
};