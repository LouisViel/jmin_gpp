#include "MagicMissile.hpp"
#include "WeaponController.hpp"
#include "SpriteOverride.hpp"
#include "Missile.hpp"
#include "Bullet.hpp"
#include "SpawnOnDestroy.hpp"
#include "Utils.hpp"
#include "C.hpp"

#include "Game.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "Entity.hpp"


MagicMissile::MagicMissile(Entity* entity, WeaponController* controller) : Weapon(entity, controller)
{
	weaponTexture = new sf::Texture();
	weaponTexture->loadFromFile("res/magicMissile.png");
	weaponHandle = new sf::RectangleShape({ C::GRID_SIZE * 0.75f, C::GRID_SIZE * 0.75f });
	weaponHandle->setTexture(weaponTexture);
}

MagicMissile::~MagicMissile()
{
	delete weaponTexture;
	delete weaponHandle;
}


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
	sf::Vector2i coo = entity->getPosPixel();
	coo.y -= entity->sheight * C::GRID_SIZE * 0.8f;
	coo.x += entity->swidth * 0.2f * -C::GRID_SIZE;
	if (entity->dirx > 0) coo.x -= entity->swidth * C::GRID_SIZE * 2;
	else coo.x += entity->swidth * C::GRID_SIZE * 1.5f;
	weaponHandle->setPosition(coo.x, coo.y);

	sf::Vector2f scale = weaponHandle->getScale();
	if (Utils::sign<float>(scale.x) != Utils::sign<int>(entity->dirx)) {
		scale.x = -scale.x;
		weaponHandle->setScale(scale);
	}
	
	win.draw(*weaponHandle);
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
	coo.y -= entity->sheight * C::GRID_SIZE;
	shootBullet(sf::Vector2i{ int(coo.x - (entity->swidth * 4 + entity->dirx) * C::GRID_SIZE), coo.y }, sf::Vector2f{ -1, 0 });
	shootBullet(sf::Vector2i{ int(coo.x + (entity->swidth * 3 - entity->dirx) * C::GRID_SIZE), coo.y }, sf::Vector2f{ 0, -1 });
	shootBullet(sf::Vector2i{ int(coo.x + (entity->swidth * 4 - entity->dirx) * C::GRID_SIZE), int(coo.y + entity->sheight * C::GRID_SIZE) }, sf::Vector2f{ 1, 0 });
}

void MagicMissile::shootBullet(sf::Vector2i pos, sf::Vector2f dir)
{
	// Create Bullet Sprite
	sf::RectangleShape* spr = new sf::RectangleShape({ C::GRID_SIZE, C::GRID_SIZE });
	spr->setFillColor(sf::Color::Red);
	spr->setOrigin({ C::GRID_SIZE * 0.5f, C::GRID_SIZE * 2.0f });

	// Create Bullet
	Entity* missile = new Entity(spr);
	missile->swidth = C::GRID_SIZE * 0.4f;
	missile->sheight = C::GRID_SIZE * 0.4f;

	// Disable Physics settings
	missile->frx = 1.0f;
	missile->frx = 1.0f;
	missile->gravy = 0.0f;
	missile->usePhysics = false;

	// Set Position & Movement settings
	missile->speed = 15.0f;
	missile->setDx(dir.x * missile->speed);
	missile->setDy(dir.y * missile->speed);
	missile->setCooPixel(pos.x, pos.y);
	missile->syncPos();

	// Add Components
	missile->addComponent(new SpriteOverride(missile, "res/missile.png"));
	missile->addComponent(new Missile(missile, Utils::toAngle(dir)));
	missile->addComponent(new Bullet(missile, 50.0f));

	SpawnOnDestroy* sod = new SpawnOnDestroy(missile);
	sod->textureSize = sf::Vector2f{ C::GRID_SIZE, C::GRID_SIZE };
	sod->texture = "res/explosion.png";
	sod->lifetime = 1.0f;
	sod->frx = 1.0f;
	sod->fry = 1.0f;
	sod->usePhysics = false;
	sod->gravy = 0.0f;
	missile->addComponent(sod);

	// Register Bullet
	World* world = Game::singleton->world;
	world->entities->push_back(missile);
}