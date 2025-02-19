#pragma once
#include "Component.hpp"

class PlayerController : public Component
{
public:
	PlayerController(Entity* entity);

	void preupdate(double dt) override;
	void fixed(double fdt) override;
	void update(double dt) override;
	void imgui() override;

	void onGrounded(bool state) override;
	void onJumping(bool state) override;
	bool canJump() override;

private:
	float coyoteeTime = 0.0f;
	float jumpDelay = 0.0f;
};