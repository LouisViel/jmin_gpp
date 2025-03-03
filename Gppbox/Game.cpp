#include <imgui.h>
#include "Game.hpp"
#include "Utils.hpp"
#include "M.hpp"
#include "C.hpp"

#include "Entity.hpp"
#include "Camera.hpp"
#include "Environment.hpp"
#include "World.hpp"
#include "MapEditor.hpp"

#include "InputHandler.hpp"



Game* Game::singleton = nullptr;
double Game::g_tickTimer = 0.0;
double Game::g_time = 0.0;

Game::Game(sf::RenderWindow* win)
{
	singleton = this;
	this->win = win;

	// Create Managers
	environment = new Environment(win);
	world = new World(win);
	mapEditor = new MapEditor(win, environment, world);
	mapEditor->load();

	// Create Camera
	camera = new Camera(world, { C::C_CENTER_X, C::C_CENTER_Y }, { C::C_SIZE_X, C::C_SIZE_Y });
}

Game::~Game()
{
	singleton = nullptr;
	delete environment;
	delete world;
	delete mapEditor;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Game::preupdate(double dt)
{
	g_time += dt;
	g_tickTimer = dt;

	mapEditor->update(dt);
	double adt = mapEditor->active ? 0.0 : dt;

	world->preupdate(adt);
	if (InputHandler::getDebug()) {
		camera->addShake(0.5f, 1.0f);
	}
}

void Game::fixed(double fdt)
{
	if (!mapEditor->active) {
		world->fixed(fdt);
	}
}

void Game::update(double dt)
{
	if (!mapEditor->active) {
		environment->update(dt);
		world->update(dt);
		camera->update(dt);
	}
	world->processDelete();
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Game::draw(sf::RenderWindow& win)
{
	// Get Settings
	sf::RenderTarget* target = &win;
	sf::View defaultView = target->getView();

	// Draw World Renderings
	//environment->drawWorld(*target);

	// Enable Camera Drawing
	if (!mapEditor->active) {
		camera->setActive(*target);
	}

	// Draw Background
	environment->drawWorld(*target);

	// Draw Camera Renderings
	environment->drawCamera(*target);
	world->draw(*target);
	mapEditor->draw(*target);

	// Set Back target view
	target->setView(defaultView);
}

void Game::imgui()
{
	// Show Game Controls
	using namespace ImGui;
	if (CollapsingHeader("Controls", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (TreeNodeEx("Clavier-Souris")) {
			Indent(1.0f);
			BulletText("Left : [Q] OU [Left arrow]");
			BulletText("Right : [D] OU [Right arrow]");
			BulletText("Up : [Z] OU [Space] OU [Up arrow]");
			BulletText("Fire Weapon : [F] OU [Left click]");
			BulletText("Switch Weapon : [E] OU [Right click]");
			BulletText("Debug Key : [P] OU [Numpad *]");
			TreePop();
		}

		if (TreeNodeEx("Manette")) {
			Indent(1.0f);
			BulletText("Left : [Left joystick] OU [Left Pad]");
			BulletText("Right : [Left joystick] OU [Left Pad]");
			BulletText("Up : [Bottom Button]");
			BulletText("Fire Weapon : [Right Button] OU [Right Trigger]");
			BulletText("Switch Weapon : [Left Button] OU [Left Trigger]");
			BulletText("Debug Key : [Menu Left]");
			TreePop();
		}
	}

	// Propagate Imgui
	environment->imgui();
	world->imgui();
	mapEditor->imgui();
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Game::processEvents(sf::Event ev)
{
	// Process window beeing closed
	if (ev.type == sf::Event::Closed) {
		win->close();
		closing = true;
		return;
	}
	
	// Process Key being released
	if (ev.type == sf::Event::KeyReleased) {

		// Key [K] for walls reset debug
		if (ev.key.code == Keyboard::K) {
			environment->debug();
			return;
		}
	}
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


// Full check for Occupied Space
bool Game::isOccupied(Entity* entity) const
{
	FULL_CHECK(entity, this->isOccupied(xpos, ypos));
}

// Full check for Occupied Space
bool Game::isOccupied(int gridx, int gridy) const
{
	return this->isPlayer(gridx, gridy) ||
		this->isEnnemy(gridx, gridy) ||
		this->isWall(gridx, gridy);
}

// Check is player is at coordinates
bool Game::isPlayer(int gridx, int gridy) const
{
	return Utils::isFullBody(this->world->getPlayer(), gridx, gridy);
}

// Check if there is an ennemy at coordinates
bool Game::isEnnemy(int gridx, int gridy) const
{
	return this->world->getEnnemy(gridx, gridy) != nullptr;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


// Full check for Collisions at given position
bool Game::hasCollision(float gridx, float gridy, bool checkBorder) const
{
	if (checkBorder && isBorderX(gridx)) return true;
	return isWall(int(gridx), int(gridy));
}

// Check if outside Border (Collision) on X axis
bool Game::isBorderX(float gridx) const
{
	int wallRightX = (C::RES_X / C::GRID_SIZE) - 1;
	return gridx < 1.0f || gridx >= wallRightX;
}

// Check if outside Border (Collision) on Y axis
bool Game::isBorderY(float gridy) const
{
	int wallRightY = (C::RES_Y / C::GRID_SIZE) - 1;
	return gridy < 1.0f || gridy >= wallRightY;
}

// Check if there is a Wall (Collision) at this position
bool Game::isWall(int cx, int cy) const
{
	for (Vector2i& w : this->environment->walls) {
		if (w.x == cx && w.y == cy)
			return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


int blendModeIndex(sf::BlendMode bm) {
	if (bm == sf::BlendAlpha) return 0;
	if (bm == sf::BlendAdd) return 1;
	if (bm == sf::BlendNone) return 2;
	if (bm == sf::BlendMultiply) return 3;
	return 4;
};