#include "imgui.h"
#include "World.hpp"
#include "Entity.hpp"
#include "Utils.hpp"
#include "M.hpp"
#include "C.hpp"

#include "SpriteOverride.hpp"
#include "EnnemyController.hpp"
#include "PlayerController.hpp"
#include "WeaponController.hpp"
#include "PetDrone.hpp"

#include "Rifle.hpp"
#include "DeathRay.hpp"
#include "MagicMissile.hpp"


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


World::World(sf::RenderWindow* win)
{
	entities = new std::vector<Entity*>();
	ennemies = new std::vector<Entity*>();
	initMainChar();
	initPetDrone();
}

World::~World()
{
	for (Entity* e : *entities) delete e;
	delete entities;
	delete ennemies;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void World::preupdate(double dt)
{
	LOOPF_E(e->preupdate(dt));
}

void World::fixed(double fdt)
{
	LOOPF_E(e->fixed(fdt));
}


void World::update(double dt)
{
	beforeParts.update(dt);
	LOOPF_E(e->update(dt));
	afterParts.update(dt);
}

void World::processDelete()
{
	if (toDelete.size() <= 0) return;
	std::unordered_set<Entity*> cdelete = toDelete;
	toDelete.clear();
	for (Entity* e : cdelete) {
		REMOVE_ITEM(Entity*, entities, e);
		delete e;
	}
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void World::draw(sf::RenderTarget& win)
{
	beforeParts.draw(win);
	LOOPF_E(e->draw(win));
	afterParts.draw(win);
}

void World::imgui()
{
	using namespace ImGui;
	if (TreeNodeEx("Entities", 0)) {
		Indent(5.0f);
		LOOPF_PTR(entities, Entity* e);
		std::string txt = ("Entity " + std::to_string(i));
		if (TreeNodeEx(txt.c_str())) {
			e->imgui();
			TreePop();
		}
		LOOP_END;
		TreePop();
	}
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void World::initMainChar() {
	// Create Player Sprite
	sf::RectangleShape* spr = new sf::RectangleShape({ C::GRID_SIZE * C::S_SCALER_X, C::GRID_SIZE * 2 * C::S_SCALER_Y });
	spr->setFillColor(sf::Color::Magenta);
	spr->setOutlineColor(sf::Color::Red);
	spr->setOutlineThickness(2);
	spr->setOrigin({ C::GRID_SIZE * 0.5f, C::GRID_SIZE * 2 });

	// Create Player with "default" settings
	Entity* e = new Entity(spr);
	e->setCooGrid(3, int(C::RES_Y / C::GRID_SIZE) - 4 + 0.99f);
	e->syncPos();

	// Inject Custom Player Settings
	e->sheight = C::P_HEIGHT;
	e->swidth = C::P_WIDTH;
	e->lifepoints = C::P_LIFEPOINTS;
	e->speed = C::P_SPEED;
	e->jumpforce = C::P_JUMP;
	e->dirx = 1;

	// Add components
	e->addComponent(new PlayerController(e));
	e->addComponent(new SpriteOverride(e, "res/player.png"));

	// Add weapons to player
	WeaponController* wc = new WeaponController(e);
	wc->addWeapon(new Rifle(e, wc));
	wc->addWeapon(new DeathRay(e, wc));
	wc->addWeapon(new MagicMissile(e, wc));
	e->addComponent(wc);

	// Register Player
	entities->push_back(e);
	printf("player added\n");
}

void World::initPetDrone()
{
	constexpr float sizeX = 0.5f;
	constexpr float sizeY = 0.5f;

	// Create Drone Sprite
	sf::RectangleShape* spr = new sf::RectangleShape({ C::GRID_SIZE * sizeX * 1.75f, C::GRID_SIZE * sizeY * 1.75f });
	spr->setFillColor(sf::Color::Green);
	spr->setOutlineColor(sf::Color::Yellow);
	spr->setOutlineThickness(1);

	// Create Drone with "default" settings
	Entity* e = new Entity(spr);
	e->setCooGrid(3, int(C::RES_Y / C::GRID_SIZE) - 4 + 0.99f);
	e->syncPos();

	// Inject Drone Settings
	e->sheight = sizeX;
	e->swidth = sizeY;
	e->speed = C::P_DRONE;
	e->jumpforce = C::P_DRONE;
	e->fry = e->frx;
	e->gravy = 0.0f;

	// Add components
	e->addComponent(new PetDrone(e, getPlayer()));
	e->addComponent(new SpriteOverride(e, "res/drone.png"));
	e->spr->setFillColor(sf::Color::Cyan);

	// Register Drone
	entities->push_back(e);
	printf("drone added\n");
}

Entity* World::initEnnemy(float x, float y)
{
	Entity* e = initEnnemyCore(x, y);
	e->addComponent(new EnnemyController(e));
	entities->push_back(e);
	ennemies->push_back(e);
	return e;
}

Entity* World::initEnnemyCore(float x, float y)
{
	// Create Ennemy Sprite
	sf::RectangleShape* spr = new sf::RectangleShape({ C::GRID_SIZE * C::S_SCALER_X, C::GRID_SIZE * 2 * C::S_SCALER_Y });
	spr->setFillColor(sf::Color::Yellow);
	spr->setOutlineColor(sf::Color::Red);
	spr->setOutlineThickness(2);
	spr->setOrigin({ C::GRID_SIZE * 0.5f, C::GRID_SIZE * 2 });

	// Create Ennemy with "default" settings
	Entity* e = new Entity(spr);
	e->setCooGrid(x, y);
	e->syncPos();

	// Inject Custom Ennemy Settings
	e->sheight = C::P_HEIGHT;
	e->swidth = C::P_WIDTH;
	e->lifepoints = C::E_LIFEPOINTS;
	e->speed = C::P_SPEED;
	e->jumpforce = C::P_JUMP;

	// Add Override Sprite
	e->addComponent(new SpriteOverride(e, "res/ennemy.png"));

	return e;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void World::removeEnnemy(Entity* ennemy)
{
	this->removeEntity(this->ennemies, ennemy);
}

void World::removeEntity(std::vector<Entity*>* quick, Entity* entity)
{
	if (quick != nullptr) { REMOVE_ITEM(Entity*, quick, entity); }
	toDelete.emplace(entity);
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


Entity* World::getPlayer()
{
	if (entities->size()) return entities->operator[](0);
	return nullptr;
}

Entity* World::getEnnemy(int gridx, int gridy)
{
	for (Entity* e : *ennemies) {
		if (Utils::isFullBody(e, gridx, gridy)) return e;
	}
	return nullptr;
}

std::set<Entity*> World::getEnnemies(int gridx, int gridy)
{
	std::set<Entity*> results = std::set<Entity*>();
	for (Entity* e : *ennemies) {
		if (Utils::isFullBody(e, gridx, gridy)) {
			results.emplace(e);
		}
	}
	return results;
}

Entity* World::getClosest(std::vector<Entity*>* vector, sf::Vector2i posPix)
{
	Entity* target = nullptr;
	float distance = INFINITY;

	for (Entity* e : *vector) {
		sf::Vector2i eposPix = e->getPosPixel();
		float dist = Utils::toLength(eposPix - posPix);
		if (dist < distance) {
			distance = dist;
			target = e;
		}
	}

	return target;
}

bool World::isValid(std::vector<Entity*>* vector, Entity* entity)
{
	if (entity == nullptr) return false;
	for (Entity* e : *vector) {
		if (e == entity)
			return true;
	}
	return false;
}