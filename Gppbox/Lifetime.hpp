#pragma once
#include "Component.hpp"

class Lifetime : public Component
{
private:
	float lifetime = 0.0f;

public:
	Lifetime(Entity* entity, float lifetime);
	void preupdate(double dt) override;
};