#include <imgui.h>
#include "Environment.hpp"
#include "C.hpp"


Environment::Environment(sf::RenderWindow* win)
{
	this->win = win;
	initBackground();
}

Environment::~Environment()
{
	delete bgShader;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Environment::initBackground()
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


void Environment::update(double dt)
{
	if (bgShader) bgShader->update(dt);
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Environment::draw(sf::RenderWindow& win)
{
	// Draw Background
	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader* sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	states.texture = &bgTexture;
	sh->setUniform("texture", bgTexture);
	win.draw(bgHandle, states);

	// Draw Walls
	for (sf::RectangleShape& r : wallSprites) win.draw(r);
}

void Environment::imgui()
{
	using namespace ImGui;
	if (CollapsingHeader("World", ImGuiTreeNodeFlags_DefaultOpen)) {

		// Draw Debug Walls
		if (TreeNodeEx("Walls")) {
			for (sf::Vector2i& w : walls) {
				Value("x", w.x);
				Value("y", w.y);
			}
			TreePop();
		}
	}
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


// Cache Walls to Graphics
void Environment::cacheWalls()
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

void Environment::debug()
{
	walls.clear();
	cacheWalls();
}