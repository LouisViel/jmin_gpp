#pragma once

#include <vector>
#include "ParticleMan.hpp"

class Entity;
namespace sf {
	class RenderWindow;
}

class World
{
public:
	std::vector<Entity*>* entities = nullptr;
	ParticleMan beforeParts;
	ParticleMan afterParts;

	World(sf::RenderWindow* win);
	~World();

	void initMainChar();
	Entity* initEnnemy(float x, float y);
	Entity* initEnnemyCore(float x, float y);

	void preupdate(double dt);
	void fixed(double fdt);
	void update(double dt);
	void draw(sf::RenderWindow& win);
	void imgui();

	Entity* getPlayer();
	Entity* getEnnemy(int gridx, int gridy);
};