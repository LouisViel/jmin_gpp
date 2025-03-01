#include "Bullet.hpp"
#include "Game.hpp"
#include "World.hpp"
#include "Entity.hpp"

Bullet::Bullet(Entity* entity) : Component(entity) { }

Bullet::Bullet(Entity* entity, float _damages) : Component(entity), damages(_damages) { }

void Bullet::fixed(double fdt)
{
	Game* g = Game::singleton;
	World* w = g->world;

	// Check if oustide world bounds
	if (g->isBorderX(entity->cx) || g->isBorderY(entity->cy) || g->isWall(entity->cx, entity->cy)) {
		w->removeEntity(nullptr, entity);
		return;
	}

	// Check if hit an ennemy
	Entity* ennemy = w->getEnnemy(entity->cx, entity->cy);
	if (ennemy != nullptr && ennemy->lifepoints > 0.0f) {
		ennemy->lifepoints -= damages;
		if (ennemy->lifepoints <= 0.0f) w->removeEnnemy(ennemy);
		w->removeEntity(nullptr, entity);
		return;
	}
}