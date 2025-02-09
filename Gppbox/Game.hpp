#pragma once

#include <vector>
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"
#include "Particle.hpp"
#include "ParticleMan.hpp"

#include "World.hpp"
#include "Entity.hpp"


class Game {
public:
	static Game* singleton;
	static double g_tickTimer;
	static double g_time;

	sf::RenderWindow* win = nullptr;
	bool closing = false;
	
	World* world = nullptr;
	std::vector<Entity*> entities;
	ParticleMan beforeParts;
	ParticleMan afterParts;

	bool wasSpacePressed = false;

	
	~Game();
	Game(sf::RenderWindow * win);
	void initMainChar();
	Entity* getPlayer();

	void preupdate(double dt);
	void fixed(double fdt);
	void update(double dt);
	void draw(sf::RenderWindow& win);
	void imgui();

	void processEvents(sf::Event ev);
	void processInputs(double dt);

	bool isOccupied(int gridx, int gridy);
	bool isPlayer(int gridx, int gridy);
	bool isEnnemy(int gridx, int gridy);

	bool hasCollision(float gridx, float gridy, bool checkBorder = true);
	bool isBorderX(float gridx);
	bool isWall(int cx, int cy);
};