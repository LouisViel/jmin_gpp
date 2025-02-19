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

	virtual void preupdate(double dt) { } // Pre Update (called even in "edit mode")
	virtual void fixed(double fdt) { } // Fixed Update (no call in "edit mode")
	virtual void update(double dt) { } // Update (no call in "edit mode")
	virtual void draw(sf::RenderWindow& win) { } // Graphics Drawing
	virtual void imgui() { } // Imgui Drawing

	virtual void onGrounded(bool state) { }
	virtual void onJumping(bool state) { }
	virtual bool canJump() { return true; }
};