#pragma once
#include <vector>
#include "SFML/System/Vector2.hpp"
#include "Component.hpp"

class Weapon;

class WeaponController : public Component
{
private:
	std::vector<Weapon*>* weapons = nullptr;
	int currentWeapon = 0;

	#define validateWeapon() if (currentWeapon >= weapons->size()) return;
	#define cweapon weapons->operator[](currentWeapon)

public:
	bool isShooting = false;
	bool isSwitching = false;

	WeaponController(Entity* entity);
	~WeaponController();

	void addWeapon(Weapon* weapon);
	void addWeapons(Weapon** weapons, int count);
	void removeWeapon(Weapon* weapon);

	void preupdate(double dt) override;
	void draw(sf::RenderTarget& win) override;
	void imgui() override;

	void addRecoil(float force);
	void addRecoil(sf::Vector2f dir, float force);
private:
	void secureWeapon();
};