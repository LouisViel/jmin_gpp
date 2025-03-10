#pragma once

#include <vector>
#include "HotReloadShader.hpp"
#include "MapEditor.hpp"

class Environment
{
public:
	sf::RenderWindow* win;

	sf::RectangleShape bgHandle;
	HotReloadShader* bgShader = nullptr;
	sf::Texture	bgTexture;
	sf::Texture wallTexture;

	std::vector<sf::Vector2i> walls;
	std::vector<sf::RectangleShape> wallSprites;

	~Environment();
	Environment(sf::RenderWindow* win);
	void initBackground();

	void update(double dt);
	void drawWorld(sf::RenderTarget& win);
	void drawCamera(sf::RenderTarget& win);
	void imgui();

	void cacheWalls();
	void debug();
};
