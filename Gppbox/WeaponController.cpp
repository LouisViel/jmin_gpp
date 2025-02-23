#include "WeaponController.hpp"
#include "InputHandler.hpp";
#include "Weapon.hpp"
#include "Entity.hpp"
#include "M.hpp"

WeaponController::WeaponController(Entity* entity) : Component(entity)
{
	this->weapons = new std::vector<Weapon*>();
}

WeaponController::~WeaponController()
{
	LOOPF_PTR(weapons, Weapon* w);
	delete w;
	LOOP_END;
	delete weapons;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void WeaponController::addWeapon(Weapon* weapon)
{
	if (weapon == nullptr) return;
	this->weapons->push_back(weapon);
}

void WeaponController::addWeapons(Weapon** weapons, int count)
{
	if (weapons == nullptr || count <= 0) return;
	this->weapons->insert(this->weapons->end(), weapons, weapons + count);
}

void WeaponController::removeWeapon(Weapon* weapon)
{
	LOOPF_PTR(weapons, Weapon* w);
	if (w == weapon) {
		weapons->erase(weapons->begin() + i);
		secureWeapon();
		delete w;
		break;
	}
	LOOP_END;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void WeaponController::preupdate(double dt)
{
	// No logistic Update
	NO_UPDATE(dt);

	// Get Wp size & Update Weapon
	int wpSize = weapons->size();
	if (currentWeapon < wpSize) {
		cweapon->update(dt);
	}

	// Get Inputs
	if (!InputHandler::hasFocus()) return;
	bool shooting = InputHandler::getFire();
	bool switching = InputHandler::getSwitch();

	// Evaluate Shooting
	bool shootingChanged = false;
	if (shooting != isShooting) {
		isShooting = shooting;
		shootingChanged = true;
	}

	// Evaluate Switching
	bool switchChanged = false;
	if (switching != isSwitching) {
		isSwitching = switching;
		switchChanged = true;
	}

	// Get & Validate Weapon
	validateWeapon();
	Weapon* weapon = cweapon;

	// Try Switch current Weapon
	bool trySwitch = switchChanged && switching && wpSize > 1;
	if (trySwitch && weapon->canSwitch()) {
		isShooting = false;
		weapon->cancel();
		currentWeapon = (currentWeapon + 1) % wpSize;
	
	// Try Shooting with weapon
	} else if (shootingChanged) {
		if (shooting) weapon->shoot();
		else weapon->cancel();
	}
}

void WeaponController::draw(sf::RenderTarget& win)
{
	validateWeapon();
	cweapon->draw(win);
}

void WeaponController::imgui()
{
	validateWeapon();
	cweapon->imgui();
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void WeaponController::addRecoil(float force)
{
	addRecoil(sf::Vector2f{ -0.5f, 0.3f }, force);
}

void WeaponController::addRecoil(sf::Vector2f dir, float force)
{
	dir *= force;
	dir.x *= entity->dirx;
	entity->setDx(entity->dx + dir.x);
	entity->setDy(entity->dy + dir.y);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void WeaponController::secureWeapon()
{
	int wsize = weapons->size();
	if (currentWeapon >= wsize && wsize > 0) {
		currentWeapon %= wsize;
	}
}

