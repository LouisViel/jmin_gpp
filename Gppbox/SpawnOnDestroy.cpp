#include "SpawnOnDestroy.hpp"
#include "Game.hpp"
#include "World.hpp"
#include "Entity.hpp"
#include "SpriteOverride.hpp"
#include "Lifetime.hpp"

SpawnOnDestroy::SpawnOnDestroy(Entity* entity) : Component(entity) { }

SpawnOnDestroy::~SpawnOnDestroy()
{
	if (Game::singleton == nullptr) return;

	bool isShape = textureSize != sf::Vector2f{ 0, 0 };
	bool isTexture = texture.length() > 0;
	bool isLifetime = lifetime > 0.0f;

	// Generate Sprite if added
	sf::RectangleShape* spr = nullptr;
	if (isShape) spr = new sf::RectangleShape(textureSize);

	// Generate Entity
	Entity* e = new Entity(spr);
	sf::Vector2i coo = entity->getPosPixel();
	e->setCooPixel(coo.x, coo.y);
	e->syncPos();

	e->sheight = sheight;
	e->swidth = swidth;
	e->lifepoints = lifepoints;

	e->frx = frx;
	e->fry = fry;
	e->usePhysics = usePhysics;
	e->gravy = gravy;

	// Inject Sprite with Texture if added
	if (isShape && isTexture) e->addComponent(new SpriteOverride(e, texture));

	// Inject Lifetime destroyer
	if (isLifetime) e->addComponent(new Lifetime(e, lifetime));

	// Register Entity into pool
	Game::singleton->world->entities->push_back(e);
}