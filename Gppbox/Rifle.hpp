#pragma once
#include "Weapon.hpp"
#include "Throttle.hpp"

namespace sf {
	class RectangleShape;
}

class Rifle : public Weapon
{
private:
	sf::RectangleShape* muzzle = nullptr;
	float delay = 0.0f;
	float muzzleDelay = 0.0f;

public:
	Rifle(Entity* entity, WeaponController* controller);
	~Rifle();

	void update(double dt) override;
	void draw(sf::RenderTarget& win) override;

private:
	void shootBullet();
	void shootEffect();
};