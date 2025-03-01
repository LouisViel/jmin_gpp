#pragma once
#include "Component.hpp"

class PetDrone : public Component
{
public:
	PetDrone(Entity* entity);
	~PetDrone();

	void preupdate(double dt) override;
	void fixed(double fdt) override;
	void update(double dt) override;

	void imgui() override;
	void draw(sf::RenderTarget& win) override;
};