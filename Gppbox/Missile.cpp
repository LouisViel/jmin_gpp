#define _USE_MATH_DEFINES
#include <cmath>

#include "Missile.hpp"
#include "Tween.hpp"
#include "Utils.hpp"
#include "C.hpp"

#include "Game.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "Entity.hpp"


Missile::Missile(Entity* entity, float dirAngle) : Component(entity)
{
	transf->setRotation(dirAngle);
	initTarget();
}

void Missile::initTarget()
{
	// Get Settings
	World* world = Game::singleton->world;
	sf::Vector2i coo = entity->getPosPixel();
	Entity* closest = world->getClosest(world->ennemies, coo);
	if (closest == nullptr) return;

	// Apply max distance
	sf::Vector2i ecoo = closest->getPosPixel();
	float distance = Utils::toLength(ecoo - coo);
	if (distance > 20.0f * C::GRID_SIZE) return;

	// Register target
	target = closest;
	isValid = true;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Missile::preupdate(double dt)
{
	World* w = Game::singleton->world;
	isValid = w->isValid(w->ennemies, target);
	if (!isValid) {
		initTarget();
		if (!isValid) {
			entity->setDx(0.0f);
			entity->setDy(0.0f);
		}
	}
}

void Missile::fixed(double fdt)
{
	// Ensure target
	target_valid();

	// Apply rotation
	sf::Vector2i dir = target->getPosPixel() - entity->getPosPixel();
	float rot = transf->getRotation(), maxRot = C::F_FIXED * 20.0f, angle = Utils::toAngle(dir);
	float res = Tween<float>::From(rot).To(angle).For(maxRot).SetEase(Ease::Linear).Update(fdt);
	transf->setRotation(res);

	// Apply movement
	sf::Vector2f move = Utils::fromAngle(res);
	entity->dx = move.x * speed;
	entity->dy = move.y * speed;
}

void Missile::update(double dt)
{
	target_invalid();
	notDelay -= dt;
	if (notDelay <= 0.0f) {
		World* world = Game::singleton->world;
		world->removeEntity(nullptr, entity);
	}
}

void Missile::draw(sf::RenderTarget& win)
{
	target_invalid();
	entity->spr->setFillColor(sf::Color::Blue);
	entity->spr->setScale(1.5f, 1.5f);
}