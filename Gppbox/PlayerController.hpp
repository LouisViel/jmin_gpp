#pragma once
#include "Component.hpp"
#include "Entity.hpp"

class PlayerController : public Component
{
public:
	PlayerController(Entity* entity);

	void preupdate(double dt) override;
	void fixed(double fdt) override;
	void update(double dt) override;
	void draw(sf::RenderWindow& win) override;
	bool imgui() override;
};