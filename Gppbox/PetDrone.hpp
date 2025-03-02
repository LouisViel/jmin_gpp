#pragma once
#include "Component.hpp"
#include "C.hpp"

class PetDrone : public Component
{
private:
	Entity* player;
	float cooldown = 3.0f;

public:
	float moveDist = C::GRID_SIZE * 3;
	float maxDist = C::GRID_SIZE * 18;
	float shootDist = C::GRID_SIZE * 30;

	PetDrone(Entity* entity, Entity* player);
	~PetDrone();

	void preupdate(double dt) override;
	void fixed(double fdt) override;
	void update(double dt) override;

	void imgui() override;
	void draw(sf::RenderTarget& win) override;

	void shoot(sf::Vector2f dir);
};