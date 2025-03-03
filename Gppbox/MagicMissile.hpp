#pragma once
#include "Weapon.hpp"

class MagicMissile : public Weapon
{
private:
	sf::Texture* weaponTexture = nullptr;
	sf::RectangleShape* weaponHandle = nullptr;
	float cooldown = 0.0f;

public:
	MagicMissile(Entity* entity, WeaponController* controller);
	~MagicMissile();

	void update(double dt) override;
	void draw(sf::RenderTarget& win) override;

	void shoot() override;
	void cancel() override;

private:
	void shootEffect();
	void shootBullets();
	void shootBullet(sf::Vector2i pos, sf::Vector2f dir);
};