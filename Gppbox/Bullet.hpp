#pragma once
#include "Component.hpp"

class Bullet : public Component
{
public:
	float damages = 10.0f;
	Bullet(Entity* entity);
	Bullet(Entity* entity, float damages);
	void fixed(double fdt) override;
};