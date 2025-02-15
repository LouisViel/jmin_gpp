#pragma once

class Entity;
namespace sf {
	class RenderWindow;
}

class Component
{
protected:
	Entity* entity;

public:
	Component() = delete;
	Component(Entity* _entity) : entity(_entity) { }

	virtual void preupdate(double dt) { } // Pre Update
	virtual void fixed(double fdt) { } // Fixed Update
	virtual void update(double dt) { } // Update
	virtual void draw(sf::RenderWindow& win) { } // Graphics Drawing
	virtual bool imgui() { return false; } // Imgui Drawing

	virtual void onGrounded(bool state) { }
	virtual void onJumping(bool state) { }
	virtual bool canJump() { return true; }
};