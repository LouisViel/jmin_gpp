#include "imgui.h"
#include "PetDrone.hpp"
#include "Bullet.hpp"
#include "Utils.hpp"

#include "Entity.hpp"
#include "Game.hpp"
#include "World.hpp"

PetDrone::PetDrone(Entity* entity, Entity* _player) : Component(entity), player(_player) { }

PetDrone::~PetDrone() { }


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void PetDrone::preupdate(double dt)
{
	if (cooldown > 0.0f) cooldown -= dt;
}

void PetDrone::fixed(double fdt)
{
	// Get Values
	sf::Vector2i ppos = player->getPosPixel();
	ppos.y -= player->sheight * C::GRID_SIZE;
	sf::Vector2i diff = ppos - entity->getPosPixel();
	float distance = Utils::toLength(diff);

	// TP if too far
	if (distance >= maxDist) {
		entity->setCooPixel(ppos.x, ppos.y);
		entity->syncPos();

	// Apply Move Settings
	} else if (distance >= moveDist) {
		sf::Vector2f move = Utils::normalize(diff) * (float)fdt;
		entity->setDx(entity->dx + move.x * entity->speed);
		entity->setDy(entity->dy + move.y * entity->jumpforce);
	}
}

void PetDrone::update(double dt)
{
	// No timer Shoot
	if (cooldown > 0.0f) return;

	// Get Settings & Values
	World* w = Game::singleton->world;
	sf::Vector2i pos = entity->getPosPixel();
	Entity* closest = w->getClosest(w->ennemies, pos);
	if (closest == nullptr) return;

	// Adjust target pos
	sf::Vector2i cpos = closest->getPosPixel();
	cpos.y -= (closest->sheight * 0.75f) * C::GRID_SIZE;

	// Check target in range & Shoot
	sf::Vector2i dir = cpos - pos;
	float dist = Utils::toLength(dir);
	if (dist <= shootDist) {
		cooldown = 3.0f;
		shoot(Utils::normalize(dir));
	}
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void PetDrone::shoot(sf::Vector2f dir)
{
	World* world = Game::singleton->world;
	sf::Vector2i coo = entity->getPosPixel();

	// Create Bullet Sprite
	sf::RectangleShape* spr = new sf::RectangleShape({ C::GRID_SIZE * 0.25f, C::GRID_SIZE * 0.25f });
	spr->setFillColor(sf::Color::Red);
	spr->setRotation(Utils::toAngle(dir));

	// Create Bullet
	Entity* bullet = new Entity(spr);
	bullet->addComponent(new Bullet(bullet));
	bullet->swidth = C::GRID_SIZE * 0.25f;
	bullet->sheight = C::GRID_SIZE * 0.25f;
	bullet->speed = 100.0f;

	// Disable Physics settings
	bullet->frx = 1.0f;
	bullet->gravy = 0.0f;
	bullet->usePhysics = false;

	// Set Position & Movement settings
	bullet->setDx(dir.x * bullet->speed);
	bullet->setDy(dir.y * bullet->speed);
	bullet->setCooPixel(coo.x, coo.y);
	bullet->syncPos();

	// Register Bullet
	world->entities->push_back(bullet);
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void PetDrone::imgui()
{
	return;
}

void PetDrone::draw(sf::RenderTarget& win)
{
	return;
}