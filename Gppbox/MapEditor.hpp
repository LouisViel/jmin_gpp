#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

class World;

class MapEditor
{
public:
	enum TileType {
		Wall = 0,
		Ennemy = 1
	};


	sf::RenderWindow* win;
	World* world;

	bool active = false;
	bool valid = false;

	TileType tileType = TileType::Wall;
	bool occupied = false;

	bool leftButton = false;
	bool rightButton = false;
	sf::Vector2i mousePos;
	

	MapEditor(sf::RenderWindow* win, World* world);

	void update(double dt);
	void draw(sf::RenderWindow& win);
	void imgui();

	void save();
	void load();


private:
	void loadInternal(std::ifstream& infile);
	void loadDefault();
	void clearAll();

	void saveWall(std::ofstream& outfile);
	void loadWall(std::ifstream& infile);
	void loadWallDefault();
	void clearWall();

	void saveEnnemy(std::ofstream& outfile);
	void loadEnnemy(std::ifstream& infile);
	void loadEnnemyDefault();
	void clearEnnemy();


	void addWall();
	void addEnnemy();

	inline void removeAny();
	void removeWall();
	void removeEnnemy();
};