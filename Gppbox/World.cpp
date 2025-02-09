#include <imgui.h>
#include "World.hpp"
#include "Game.hpp";
#include "C.hpp"


World::World(sf::RenderWindow* win)
{
	this->win = win;
	mapEditor = new MapEditor(win, this);
	mapEditor->load();
	initBackground();
}

World::~World()
{
	delete bgShader;
	delete mapEditor;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void World::initBackground()
{
	bool isOk = bgTexture.loadFromFile("res/bg_stars.png");
	if (!isOk) printf("ERR : LOAD FAILED\n");
	bgHandle = sf::RectangleShape(sf::Vector2f((float)win->getSize().x, (float)win->getSize().y));
	bgHandle.setTexture(&bgTexture);
	bgHandle.setSize(sf::Vector2f(C::RES_X, C::RES_Y));
	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void World::update(double dt)
{
	if (bgShader) bgShader->update(dt);
	mapEditor->update(dt);
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void World::draw(sf::RenderWindow& win)
{
	// Draw Background
	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader* sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	states.texture = &bgTexture;
	sh->setUniform("texture", bgTexture);
	win.draw(bgHandle, states);

	for (sf::RectangleShape& r : wallSprites) win.draw(r);
	mapEditor->draw(win);
}

void World::imgui()
{
	using namespace ImGui;
	if (CollapsingHeader("World", ImGuiTreeNodeFlags_DefaultOpen)) {

		// Draw Debug Walls
		if (TreeNodeEx("Walls")) {
			for (Vector2i& w : walls) {
				Value("x", w.x);
				Value("y", w.y);
			}
			TreePop();
		}

		// Draw Editor Imgui
		mapEditor->imgui();
	}
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


// Cache Walls to Graphics
void World::cacheWalls()
{
	wallSprites.clear();
	for (sf::Vector2i& w : walls) {
		sf::RectangleShape rect(sf::Vector2f(C::GRID_SIZE, C::GRID_SIZE));
		rect.setPosition((float)w.x * C::GRID_SIZE, (float)w.y * C::GRID_SIZE);
		//rect.setFillColor(sf::Color(0x07ff07ff));
		rect.setFillColor(sf::Color::Cyan);
		wallSprites.push_back(rect);
	}
}

void World::debug()
{
	walls.clear();
	cacheWalls();
}