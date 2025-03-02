#pragma once

#include <vector>
#include <set>
#include <unordered_set>
#include "ParticleMan.hpp"
#include "M.hpp"

class Entity;
namespace sf {
	class RenderWindow;
}

class World
{
private:
	// Loop Forward on entities
	#define LOOPF_E(action) \
		LOOPF_PTR(entities, Entity* e) \
		action; \
		LOOP_END

	// Loop Backward on entities
	#define LOOPB_E(action) \
		LOOPB_PTR(entities, Entity* e) \
		action; \
		LOOP_END

	// Entity delete set (for nice cleaning)
	std::unordered_set<Entity*> toDelete;

public:
	std::vector<Entity*>* entities = nullptr;
	std::vector<Entity*>* ennemies = nullptr;
	ParticleMan beforeParts;
	ParticleMan afterParts;

	World(sf::RenderWindow* win);
	~World();

	void initMainChar();
	void initPetDrone();
	Entity* initEnnemy(float x, float y);
	Entity* initEnnemyCore(float x, float y);

	void preupdate(double dt);
	void fixed(double fdt);
	void update(double dt);
	void processDelete();
	void draw(sf::RenderTarget& win);
	void imgui();

	void removeEnnemy(Entity* ennemy);
	void removeEntity(std::vector<Entity*>* quick, Entity* e);

	Entity* getPlayer();
	Entity* getEnnemy(int gridx, int gridy);
	std::set<Entity*> getEnnemies(int gridx, int gridy);
	Entity* getClosest(std::vector<Entity*>* vector, sf::Vector2i posPix);
	bool isValid(std::vector<Entity*>* vector, Entity* entity);
};