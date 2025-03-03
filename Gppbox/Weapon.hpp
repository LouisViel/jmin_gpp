#pragma once
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

class Entity;
class WeaponController;
namespace sf {
	class Texture;
}

class Weapon
{
protected:
	WeaponController* controller;
	Entity* entity;

public:
	Weapon() = delete;
	Weapon(Entity* _entity, WeaponController* _controller) : entity(_entity), controller(_controller) { }
	virtual ~Weapon() { }
	
	virtual void update(double dt) { }
	virtual void draw(sf::RenderTarget& win) { }
	virtual void imgui() { }

	virtual void shoot() { }
	virtual void cancel() { }
	virtual bool canSwitch() { return true; }
};