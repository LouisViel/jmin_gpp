#pragma once

class Entity;
class Component
{
protected:
	Entity* entity;

public:
	Component() = delete;
	Component(Entity* _entity) : entity(_entity) { }

	virtual void preupdate(double dt) = 0; // Pre Update
	virtual void fixed(double fdt) = 0; // Fixed Update
	virtual void update(double dt) = 0; // Update
	virtual void draw(sf::RenderWindow& win) = 0; // Graphics Drawing
	virtual bool imgui() = 0; // Imgui Drawing
};