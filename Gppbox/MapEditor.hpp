#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

class Game;
class Environment;
class World;
class Entity;

class MapEditor
{
public:
	enum TileType {
		Wall = 0,
		Ennemy = 1
	};


	sf::RenderWindow* win;
	Environment* environment;
	World* world;

	// States
	bool active = false;
	bool valid = false;

	// Process values
	TileType tileType = TileType::Wall;
	bool occupied = false;
	
	// Inputs
	bool leftButton = false;
	bool rightButton = false;
	sf::Vector2i mousePos;

	// Draw Utils
	Entity* eSpr = nullptr;
	

	MapEditor(sf::RenderWindow* win, Environment* environment, World* world);
	~MapEditor();
	
	// Processing
	void update(double dt);
	void draw(sf::RenderTarget& win);
	void imgui();

	// Save / Load
	void save();
	void load();


private:
	// Load Internal Global
	void loadInternal(std::ifstream& infile);
	void loadDefault();
	void clearAll();

	// Save / Load Walls
	void saveWall(std::ofstream& outfile);
	void loadWall(std::ifstream& infile);
	void loadWallDefault();
	void clearWall();

	// Save / Load Ennemies
	void saveEnnemy(std::ofstream& outfile);
	void loadEnnemy(std::ifstream& infile);
	void loadEnnemyDefault();
	void clearEnnemy();


	// Add to Map
	void addWall();
	void addEnnemy();

	// Remove from Map
	inline void removeAny();
	void removeWall();
	void removeEnnemy();


	// Utils Methods
	void isOccupied(Game* const g);
	void drawWall(sf::RenderTarget& win);
	void drawEnnemy(sf::RenderTarget& win);
};