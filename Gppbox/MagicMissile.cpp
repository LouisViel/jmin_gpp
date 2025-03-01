#include "MagicMissile.hpp"
#include "WeaponController.hpp"
#include "Missile.hpp"
#include "Bullet.hpp"
#include "Utils.hpp"
#include "C.hpp"

#include "Game.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "Entity.hpp"


MagicMissile::MagicMissile(Entity* entity, WeaponController* controller) : Weapon(entity, controller) { }

MagicMissile::~MagicMissile() { }


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void MagicMissile::update(double dt)
{
	if (cooldown > 0.0f) cooldown -= dt;
	if (!controller->isShooting || cooldown > 0.0f) return;
	shootBullets();
	shootEffect();
	cooldown = 1.5f;
}

void MagicMissile::draw(sf::RenderTarget& win)
{
	return;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void MagicMissile::shoot() { }

void MagicMissile::cancel() { }


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void MagicMissile::shootEffect()
{
	// Apply feedbacks
	Game::singleton->camera->addShake(0.3f, 0.65f);
	controller->addRecoil(37.5f);
}

void MagicMissile::shootBullets()
{
	sf::Vector2i coo = entity->getPosPixel();
	coo.y -= (entity->sheight * 0.3f) * C::GRID_SIZE;
	shootBullet(sf::Vector2i{ int(coo.x - entity->swidth * 2), coo.y }, sf::Vector2f{ -1, 0 });
	shootBullet(sf::Vector2i{ coo.x, int(coo.y + entity->sheight * 1.5f) }, sf::Vector2f{ 0, -1 });
	shootBullet(sf::Vector2i{ int(coo.x + entity->swidth * 2), coo.y }, sf::Vector2f{ 1, 0 });
}

void MagicMissile::shootBullet(sf::Vector2i pos, sf::Vector2f dir)
{
	// Create Bullet Sprite
	sf::RectangleShape* spr = new sf::RectangleShape({ C::GRID_SIZE * 0.4f, C::GRID_SIZE * 0.4f });
	spr->setFillColor(sf::Color::Red);
	//spr->setOrigin({ C::GRID_SIZE * 0.5f, C::GRID_SIZE * 0.5f });

	// Create Bullet
	Entity* missile = new Entity(spr);
	missile->swidth = C::GRID_SIZE * 0.4f;
	missile->sheight = C::GRID_SIZE * 0.4f;

	// Disable Physics settings
	missile->frx = 1.0f;
	missile->gravy = 0.0f;
	missile->usePhysics = false;

	// Set Position & Movement settings
	constexpr float speed = 7.0f;
	missile->setDx(dir.x * speed);
	missile->setDy(dir.y * speed);
	missile->setCooPixel(pos.x, pos.y);
	missile->syncPos();

	// Add Components
	missile->addComponent(new Missile(missile, Utils::toAngle(dir)));
	missile->addComponent(new Bullet(missile, 50.0f));

	// Register Bullet
	World* world = Game::singleton->world;
	world->entities->push_back(missile);
}