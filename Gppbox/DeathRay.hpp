#pragma once
#include "Weapon.hpp"

namespace sf {
	class RectangleShape;
}

class DeathRay : public Weapon
{
private:
	sf::Texture* weaponTexture = nullptr;
	sf::RectangleShape* weaponHandle = nullptr;
	sf::RectangleShape* laserGraphics = nullptr;
	sf::Vector2f orientation = sf::Vector2f(1.0f, 0.0f);
	float cooldown = 0.0f;
	float drawDelay = 0.0f;

	sf::Vector2f origin;
	sf::Vector2f target;
	float damages = 45.0f;

public:
	DeathRay(Entity* entity, WeaponController* controller);
	~DeathRay();

	void update(double dt) override;
	void draw(sf::RenderTarget& win) override;

	void shoot() override;
	void cancel() override;

private:
	void shootEffect();
	void shootBullet();
	std::vector<sf::Vector2i> bresenham();
};