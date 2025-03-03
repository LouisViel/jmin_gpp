#pragma once
#include "SFML/System/Vector2.hpp"
#include "Component.hpp"

class Entity;

class Missile : public Component
{
private:
	Entity* target;
	float notDelay = 0.2f;
	bool isValid = false;

	#define transf entity->spr
	#define target_valid() if (!isValid) return;
	#define target_invalid() if (isValid) return;
	void initTarget();

public:
	Missile(Entity* entity, float dirAngle);
	
	void preupdate(double dt) override;
	void fixed(double fdt) override;
	void update(double dt) override;
	void draw(sf::RenderTarget& win) override;
};