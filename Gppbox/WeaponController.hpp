#pragma once
#include <vector>
#include "Component.hpp"

class Weapon;

class WeaponController : public Component
{
private:
	std::vector<Weapon*>* weapons = nullptr;
	int currentWeapon = 0;

	#define validateWeapon() if (currentWeapon >= weapons->size()) return;
	#define cweapon weapons->operator[](currentWeapon)
	void secureWeapon();

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
};