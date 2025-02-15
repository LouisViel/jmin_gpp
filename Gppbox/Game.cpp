#include <imgui.h>
#include "Game.hpp"
#include "PlayerController.hpp"
#include "C.hpp"

Game* Game::singleton = nullptr;
double Game::g_tickTimer = 0.0;
double Game::g_time = 0.0;

Game::Game(sf::RenderWindow* win)
{
	singleton = this;
	this->win = win;
	world = new World(win);
	initMainChar();
}

Game::~Game()
{
	delete world;
	for (Entity* e : entities) delete e;
	entities.clear();
}

Entity* Game::getPlayer()
{
	if (entities.size()) return entities[0];
	return nullptr;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Game::initMainChar() {
	// Create Player Sprite
	sf::RectangleShape* spr = new sf::RectangleShape({ C::GRID_SIZE * C::S_SCALER_X, C::GRID_SIZE * 2 * C::S_SCALER_Y });
	spr->setFillColor(sf::Color::Magenta);
	spr->setOutlineColor(sf::Color::Red);
	spr->setOutlineThickness(2);
	spr->setOrigin({ C::GRID_SIZE * 0.5f, C::GRID_SIZE * 2 });

	// Create Player with "default" settings
	Entity* e = new Entity(spr);
	e->addComponent(new PlayerController(e));
	e->setCooGrid(3, int(C::RES_Y / C::GRID_SIZE) - 4);
	e->ry = 0.99f;
	e->syncPos();

	// Inject Custom Player Settings
	e->sheight = C::P_HEIGHT;
	e->swidth = C::P_WIDTH;
	e->speed = C::P_SPEED;
	e->jumpforce = C::P_JUMP;

	entities.push_back(e);
	printf("player added\n");
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Game::preupdate(double dt)
{
	g_time += dt;
	for (Entity* e : entities) e->preupdate(dt);
	processInputs(dt);
}

void Game::fixed(double fdt)
{
	for (Entity* e : entities) e->fixed(fdt);
}

void Game::update(double dt)
{
	beforeParts.update(dt);
	world->update(dt);
	for (Entity* e : entities) e->update(dt);
	afterParts.update(dt);
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Game::draw(sf::RenderWindow& win)
{
	// Draw Parts and Entities
	beforeParts.draw(win);
	world->draw(win);
	for (Entity* e : entities) e->draw(win);
	afterParts.draw(win);
}

 void Game::imgui()
 {
	using namespace ImGui;
	
	// Show Entities for debug
	if (CollapsingHeader("Entities")) {
		for (Entity* e : entities) e->imgui();
	}

	// World Imgui
	world->imgui();
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
			world->debug();
			return;
		}
	}
}

void Game::processInputs(double dt)
{
	// Process Left Movement Input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		Entity* mainChar = getPlayer();
		if (mainChar) {
			mainChar->setDx(mainChar->dx - mainChar->speed * dt);
		}
	}

	// Process Right Movement Input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		Entity* mainChar = getPlayer();
		if (mainChar) {
			mainChar->setDx(mainChar->dx + mainChar->speed * dt);
		}
	}

	// Process Jump Input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		//if (!wasSpacePressed) {
			Entity* mainChar = getPlayer();
			if (mainChar) mainChar->setJumping(true);
			wasSpacePressed = true;
		//}
	} else {
		wasSpacePressed = false;
	}
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


// Full check for Occupied Space
bool Game::isOccupied(int gridx, int gridy)
{
	return isPlayer(gridx, gridy) ||
		isEnnemy(gridx, gridy) ||
		isWall(gridx, gridy);
}

// Check is player is at coordinates
bool Game::isPlayer(int gridx, int gridy)
{
	// Get Player
	Entity* player = getPlayer();
	if (!player) return false;

	// Prepare full check Variables
	float xposMin(player->rx - player->swidth + player->cx);
	float xposMax(player->rx + player->swidth + 1 + player->cx);
	float cry(player->cy + player->ry);

	// Process full body check
	for (float ypos = cry, ytarget = cry - player->sheight; ypos > ytarget; --ypos) {
		for (float xpos = xposMin; xpos < xposMax; ++xpos) {
			if (int(xpos) == gridx && int(ypos) == gridy) return true;
		}
	}

	// Player was not here
	return false;
}

// Check if there is an ennemy at coordinates
bool Game::isEnnemy(int gridx, int gridy)
{
	// TODO : Implement
	return false;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


// Full check for Collisions at given position
bool Game::hasCollision(float gridx, float gridy, bool checkBorder)
{
	if (checkBorder && isBorderX(gridx)) return true;
	return isWall(int(gridx), int(gridy));
}

// Check if outside Border (Collision) on X axis
bool Game::isBorderX(float gridx)
{
	int wallRightX = (C::RES_X / C::GRID_SIZE) - 1;
	return gridx < 1.0f || gridx >= wallRightX;
}

// Check if there is a Wall (Collision) at this position
bool Game::isWall(int cx, int cy)
{
	for (Vector2i& w : world->walls) {
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