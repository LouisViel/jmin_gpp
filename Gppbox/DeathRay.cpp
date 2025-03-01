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
	graphics = new sf::RectangleShape({ C::GRID_SIZE, C::GRID_SIZE });
	graphics->setFillColor(sf::Color::Red);
}

DeathRay::~DeathRay()
{
	delete graphics;
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
	if (drawDelay > 0.0f) {
		win.draw(*graphics);
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
	graphics->setPosition(origin * (float)C::GRID_SIZE);
	graphics->setRotation(Utils::toAngle(dir));
	graphics->setSize(sf::Vector2f{ Utils::toLength(dir), 0.2f } * (float)C::GRID_SIZE);

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