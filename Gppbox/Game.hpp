#pragma once

#include "sys.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

class Entity;
class Environment;
class World;
class MapEditor;
class Camera;

class Game {
public:
	static Game* singleton;
	static double g_tickTimer;
	static double g_time;

	sf::RenderWindow* win = nullptr;
	bool closing = false;
	
	Environment* environment = nullptr;
	World* world = nullptr;
	MapEditor* mapEditor = nullptr;
	Camera* camera = nullptr;

	
	~Game();
	Game(sf::RenderWindow * win);

	void preupdate(double dt);
	void fixed(double fdt);
	void update(double dt);
	void draw(sf::RenderWindow& win);
	void imgui();

	void processEvents(sf::Event ev);

	bool isOccupied(Entity* entity) const;
	bool isOccupied(int gridx, int gridy) const;
	bool isPlayer(int gridx, int gridy) const;
	bool isEnnemy(int gridx, int gridy) const;

	bool hasCollision(float gridx, float gridy, bool checkBorder = true) const;
	bool isBorderX(float gridx) const;
	bool isWall(int cx, int cy) const;
};