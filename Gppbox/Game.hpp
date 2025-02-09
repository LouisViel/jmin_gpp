#pragma once

#include <vector>
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"
#include "Particle.hpp"
#include "ParticleMan.hpp"

#include "Entity.hpp"
#include "HotReloadShader.hpp"


class Game {
public:
	static Game* singleton;
	static int cols;
	static int lastLine;
	static double g_tickTimer;
	static double g_time;

	sf::RenderWindow* win = nullptr;
	bool closing = false;

	sf::RectangleShape bgHandle;
	HotReloadShader* bgShader = nullptr;
	sf::Texture	bgTexture;
	
	std::vector<sf::Vector2i> walls;
	std::vector<sf::RectangleShape> wallSprites;
	std::vector<Entity*> entities;

	ParticleMan beforeParts;
	ParticleMan afterParts;

	bool wasSpacePressed = false;

	
	Game(sf::RenderWindow * win);
	
	void initBackground();
	void initMainChar();
	void initWalls();

	void preupdate(double dt);
	void fixed(double fdt);
	void update(double dt);
	void draw(sf::RenderWindow& win);
	void im();

	void processEvents(sf::Event ev);
	void processInputs(double dt);

	Entity* getPlayer();
	void cacheWalls();

	
	bool hasCollision(float gridx, float gridy, bool checkBorder = true);
	bool isBorderX(float gridx);
	bool isWall(int cx, int cy);
};