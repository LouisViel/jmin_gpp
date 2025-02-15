#pragma once

#include <vector>
#include "HotReloadShader.hpp"
#include "MapEditor.hpp"

class World
{
public:
	sf::RenderWindow* win;
	MapEditor* mapEditor;

	sf::RectangleShape bgHandle;
	HotReloadShader* bgShader = nullptr;
	sf::Texture	bgTexture;

	std::vector<sf::Vector2i> walls;
	std::vector<sf::RectangleShape> wallSprites;

	~World();
	World(sf::RenderWindow* win);
	void initBackground();

	void update(double dt);
	void draw(sf::RenderWindow& win);
	void imgui();

	void cacheWalls();
	void debug();
};
