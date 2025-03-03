#include "DeathRay.hpp"
#include "WeaponController.hpp"
#include "Bresenham.hpp"
#include "Utils.hpp"
#include "C.hpp"

#include "Game.hpp"
#include "World.hpp"
#include "Camera.hpp"
#include "Entity.hpp"


DeathRay::DeathRay(Entity* entity, WeaponController* controller) : Weapon(entity, controller)
{
	laserGraphics = new sf::RectangleShape({ C::GRID_SIZE, C::GRID_SIZE });
	laserGraphics->setFillColor(sf::Color::Red);

	weaponTexture = new sf::Texture();
	weaponTexture->loadFromFile("res/laser.png");
	weaponHandle = new sf::RectangleShape({ C::GRID_SIZE * 0.75f, C::GRID_SIZE * 0.75f });
	weaponHandle->setTexture(weaponTexture);
}

DeathRay::~DeathRay()
{
	delete laserGraphics;
	delete weaponTexture;
	delete weaponHandle;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void DeathRay::update(double dt)
{
	if (drawDelay > 0.0f) drawDelay -= dt;
	else if (cooldown > 0.0f) cooldown -= dt;
}

void DeathRay::draw(sf::RenderTarget& win)
{
	sf::Vector2i coo = entity->getPosPixel();
	coo.y -= entity->sheight * C::GRID_SIZE * 0.8f;
	coo.x += entity->swidth * 0.2f *- C::GRID_SIZE;
	if (entity->dirx > 0) coo.x -= entity->swidth * C::GRID_SIZE * 2;
	else coo.x += entity->swidth * C::GRID_SIZE;
	weaponHandle->setPosition(coo.x, coo.y);

	sf::Vector2f scale = weaponHandle->getScale();
	if (Utils::sign<float>(scale.x) != Utils::sign<int>(entity->dirx)) {
		scale.x = -scale.x;
		weaponHandle->setScale(scale);
	}

	win.draw(*weaponHandle);
	if (drawDelay > 0.0f) {
		win.draw(*laserGraphics);
	}
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void DeathRay::shoot()
{
	if (cooldown > 0.0f) return;
	shootBullet();
	shootEffect();
	drawDelay = 0.75f;
	cooldown = 1.5f;
}

void DeathRay::cancel() { }


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void DeathRay::shootEffect()
{
	// Set Graphics Settings
	sf::Vector2f dir = (target - origin);
	laserGraphics->setPosition(origin * (float)C::GRID_SIZE);
	laserGraphics->setRotation(Utils::toAngle(dir));
	laserGraphics->setSize(sf::Vector2f{ Utils::toLength(dir), 0.2f } * (float)C::GRID_SIZE);

	// Apply feedbacks
	Game::singleton->camera->addShake(0.5f, 0.75f);
	controller->addRecoil(45.0f);
}

void DeathRay::shootBullet()
{
	// Get Bresenham line coordinates
	std::vector<sf::Vector2i> bresenhamResults = bresenham();

	// Check over line for ennemies
	World* w = Game::singleton->world;
	std::set<Entity*> ennemies = std::set<Entity*>();
	for (sf::Vector2i casePos : bresenhamResults) {
		std::set<Entity*> collisions = w->getEnnemies(casePos.x, casePos.y);
		ennemies.insert(collisions.begin(), collisions.end());
	}

	// Apply damages to ennemies
	for (Entity* ennemy : ennemies) {
		if (ennemy->lifepoints > 0.0f) {
			ennemy->lifepoints -= damages;
			if (ennemy->lifepoints <= 0.0f) w->removeEnnemy(ennemy);
		}
	}
}

std::vector<sf::Vector2i> DeathRay::bresenham()
{
	// Calculate origin
	origin = sf::Vector2f{ (float)entity->cx, (float)entity->cy };
	origin.y -= entity->sheight * 0.3f;

	// Calculate normalized reach
	sf::Vector2f reach;
	if (orientation.x != 0.0f && orientation.y != 0.0f) {
		reach = Utils::normalize(orientation);
		reach.x *= entity->dirx;
	} else reach.x = entity->dirx;

	// Calculate size multiplier x
	float multx = 0.0f;
	if (reach.x != 0.0f) {
		float limitx = reach.x >= 0.0f ? (C::RES_X / C::GRID_SIZE) : 0.0f;
		multx = std::fabs(limitx - entity->cx) / std::fabs(reach.x);
	}

	// Calculate size multiplier y
	float multy = 0.0f;
	if (reach.y != 0.0f) {
		float limity = reach.y >= 0.0f ? (C::RES_Y / C::GRID_SIZE) : 0.0f;
		multy = std::fabs(limity - entity->cy) / std::fabs(reach.y);
	}

	// Extend reach to match map size
	if (multx != 0.0f && multy != 0.0f) reach *= (std::fmin(multx, multy) + 1);
	else reach *= (std::fmax(multx, multy) + 1);

	// Use Bresenham to calculate coordinates to check
	target = origin + reach; //* float((float)C::RES_X / C::GRID_SIZE);
	return bresenhamLine(int(origin.x), int(origin.y), int(target.x), int(target.y));
}