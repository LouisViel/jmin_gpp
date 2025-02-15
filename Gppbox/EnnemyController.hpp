#pragma once
#include "Component.hpp"

class EnnemyController : public Component
{
public:
	EnnemyController(Entity* entity);

	void preupdate(double dt) override;
	void fixed(double fdt) override;
	void update(double dt) override;

	bool imgui() override;
	bool canJump() override;

private:
	int direction = -1;
};