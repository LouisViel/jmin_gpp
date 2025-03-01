#include "Rifle.hpp"
#include "WeaponController.hpp"
#include "Bullet.hpp"
#include "C.hpp"

#include "Game.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "Entity.hpp"


Rifle::Rifle(Entity* entity, WeaponController* controller) : Weapon(entity, controller)
{
	muzzle = new sf::RectangleShape({ C::GRID_SIZE * 0.35f, C::GRID_SIZE * 0.35f });
	muzzle->setFillColor(sf::Color::White);
}

Rifle::~Rifle()
{
	delete muzzle;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Rifle::update(double dt)
{
	// Update muzzle
	if (muzzleDelay > 0.0f) {
		muzzleDelay -= dt;
	}

	// Reset timer if no shooting
	if (!controller->isShooting) {
		cooldown = 0.0f;
		return;
	}

	// Check for timer
	if (cooldown > 0.0f) {
		cooldown -= dt;
		return;
	}

	// Perform Shoot
	cooldown = 0.15f;
	shootBullet();
	shootEffect();
}

void Rifle::draw(sf::RenderTarget& win)
{
	// Draw Rifle Muzzle (flash)
	if (muzzleDelay > 0.0f) {
		sf::Vector2i coo = entity->getPosPixel();
		coo.y -= (entity->sheight * 0.3f) * C::GRID_SIZE;
		muzzle->setPosition(coo.x, coo.y);
		win.draw(*muzzle);
	}
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Rifle::shootBullet()
{
	World* world = Game::singleton->world;
	sf::Vector2i coo = entity->getPosPixel();
	coo.y -= (entity->sheight * 0.3f) * C::GRID_SIZE;

	// Create Bullet Sprite
	sf::RectangleShape* spr = new sf::RectangleShape({ C::GRID_SIZE * 0.25f, C::GRID_SIZE * 0.25f });
	spr->setFillColor(sf::Color::Red);
	//spr->setOrigin({ C::GRID_SIZE * 0.5f, C::GRID_SIZE * 0.5f });

	// Create Bullet
	Entity* bullet = new Entity(spr);
	bullet->addComponent(new Bullet(bullet));
	bullet->swidth = C::GRID_SIZE * 0.25f;
	bullet->sheight = C::GRID_SIZE * 0.25f;

	// Disable Physics settings
	bullet->frx = 1.0f;
	bullet->gravy = 0.0f;
	bullet->usePhysics = false;

	// Set Position & Movement settings
	bullet->setDx(entity->dirx * 100.0f);
	bullet->setCooPixel(coo.x, coo.y);
	bullet->syncPos();

	// Register Bullet
	world->entities->push_back(bullet);
}

void Rifle::shootEffect()
{
	Game::singleton->camera->addShake(0.4f, 0.35f);
	controller->addRecoil(10.0f);
	muzzleDelay = 0.3f;
}
