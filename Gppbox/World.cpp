#include "World.hpp"
#include "Entity.hpp"
#include "Utils.hpp"
#include "C.hpp"

#include "PlayerController.hpp"
#include "EnnemyController.hpp"


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


World::World(sf::RenderWindow* win)
{
	entities = new std::vector<Entity*>();
	initMainChar();
}

World::~World()
{
	for (Entity* e : *entities) delete e;
	delete entities;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void World::preupdate(double dt)
{
	for (Entity* e : *entities) e->preupdate(dt);
}

void World::fixed(double fdt)
{
	for (Entity* e : *entities) e->fixed(fdt);
}


void World::update(double dt)
{
	beforeParts.update(dt);
	for (Entity* e : *entities) e->update(dt);
	afterParts.update(dt);
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void World::draw(sf::RenderWindow& win)
{
	beforeParts.draw(win);
	for (Entity* e : *entities) e->draw(win);
	afterParts.draw(win);
}

void World::imgui()
{
	return;
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
	e->addComponent(new PlayerController(e));
	e->setCooGrid(3, int(C::RES_Y / C::GRID_SIZE) - 4 + 0.99f);
	e->syncPos();

	// Inject Custom Player Settings
	e->sheight = C::P_HEIGHT;
	e->swidth = C::P_WIDTH;
	e->speed = C::P_SPEED;
	e->jumpforce = C::P_JUMP;

	entities->push_back(e);
	printf("player added\n");
}

Entity* World::initEnnemy(float x, float y)
{
	Entity* e = initEnnemyCore(x, y);
	e->addComponent(new EnnemyController(e));
	entities->push_back(e);
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
	e->speed = C::P_SPEED;
	e->jumpforce = C::P_JUMP;

	return e;
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
	for (int i = 1, target = entities->size(); i < target; ++i) {
		Entity* e = entities->operator[](i);
		if (Utils::isFullBody(e, gridx, gridy)) return e;
	}
	return nullptr;
}