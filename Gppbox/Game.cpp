#include <array>
#include <vector>
#include <imgui.h>

#include "C.hpp"
#include "Game.hpp"

Game* Game::singleton = nullptr;
int Game::cols = C::RES_X / C::GRID_SIZE;
int Game::lastLine = C::RES_Y / C::GRID_SIZE - 1;
double Game::g_tickTimer = 0.0;
double Game::g_time = 0.0;

Game::Game(sf::RenderWindow* win)
{
	singleton = this;
	this->win = win;
	initBackground();
	initWalls();
	initMainChar();
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Game::initBackground()
{
	bool isOk = bgTexture.loadFromFile("res/bg_stars.png");
	if (!isOk) printf("ERR : LOAD FAILED\n");
	bgHandle = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));
	bgHandle.setTexture(&bgTexture);
	bgHandle.setSize(sf::Vector2f(C::RES_X, C::RES_Y));
	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");
}

void Game::initMainChar() {
	// Create Player Sprite
	sf::RectangleShape* spr = new sf::RectangleShape({ C::GRID_SIZE, C::GRID_SIZE * 2 });
	spr->setFillColor(sf::Color::Magenta);
	spr->setOutlineColor(sf::Color::Red);
	spr->setOutlineThickness(2);
	//spr->setOrigin({ C::GRID_SIZE, C::GRID_SIZE }); // TODO : Resoudre ce soucis de de-synchro entre sprite et physics (origin/graphics wrong ??)
	//spr->setOrigin({ C::GRID_SIZE * 0.5f, C::GRID_SIZE * 2 });
	//spr->setOrigin({ C::P_WIDTH, C::P_HEIGHT });

	// Create Player with "default" settings
	Entity* e = new Entity(spr);
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

void Game::initWalls()
{
	for (int i = 0; i < C::RES_X / C::GRID_SIZE; ++i)
		walls.push_back(Vector2i(i, lastLine));

	walls.push_back(Vector2i(0, lastLine - 1));
	walls.push_back(Vector2i(0, lastLine - 2));
	walls.push_back(Vector2i(0, lastLine - 3));

	walls.push_back(Vector2i(cols - 1, lastLine - 1));
	walls.push_back(Vector2i(cols - 1, lastLine - 2));
	walls.push_back(Vector2i(cols - 1, lastLine - 3));


	walls.push_back(Vector2i(cols >> 2, lastLine - 1));
	walls.push_back(Vector2i(cols >> 2, lastLine - 2));
	walls.push_back(Vector2i(cols >> 2, lastLine - 3));
	walls.push_back(Vector2i(cols >> 2, lastLine - 4));

	walls.push_back(Vector2i((cols >> 2) + 1, lastLine - 4));
	walls.push_back(Vector2i((cols >> 2) + 1, lastLine - 5));
	walls.push_back(Vector2i((cols >> 2) + 2, lastLine - 4));
	walls.push_back(Vector2i((cols >> 2) + 2, lastLine - 5));

	cacheWalls();
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Game::update(double dt)
{
	processInputs(dt);
	g_time += dt;

	beforeParts.update(dt);
	for (Entity* e : entities) e->update(dt);
	if (bgShader) bgShader->update(dt);
	afterParts.update(dt);
}

 void Game::draw(sf::RenderWindow& win)
 {
	 // No need to run
	if (closing) return;

	// Draw Background
	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader * sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	states.texture = &bgTexture;
	sh->setUniform("texture", bgTexture);
	win.draw(bgHandle, states);

	// Draw Parts, Walls, and Entities
	beforeParts.draw(win);
	for (sf::RectangleShape& r : wallSprites) win.draw(r);
	for (Entity* e : entities) e->draw(win);
	afterParts.draw(win);
}

 void Game::im()
 {
	 // No need to run
	if (closing) return;
	using namespace ImGui;

	// Show Walls for debug
	if (TreeNodeEx("Walls", 0)) {
		for (Vector2i& w : walls) {
			Value("x",w.x);
			Value("y",w.y);
		}
		TreePop();
	}

	// Show Entities for debug
	if (TreeNodeEx("Entities", ImGuiTreeNodeFlags_DefaultOpen)) {
		for (Entity* e : entities) e->im();
		TreePop();
	}
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
			walls.clear();
			cacheWalls();
			return;
		}
	}
}

void Game::processInputs(double dt)
{
	float lateralSpeed = 8.0;
	float maxSpeed = 40.0;

	// Process Left Movement Input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		Entity* mainChar = getPlayer();
		if (mainChar) {
			mainChar->dx -= mainChar->speed;
		}
	}

	// Process Right Movement Input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		Entity* mainChar = getPlayer();
		if (mainChar) {
			mainChar->dx += mainChar->speed;
		}
	}

	// Process Jump Input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		if (!wasSpacePressed) {
			Entity* mainChar = getPlayer();
			if (mainChar) mainChar->setJumping(true);
			wasSpacePressed = true;
		}
	} else {
		wasSpacePressed = false;
	}
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


Entity* Game::getPlayer()
{
	if (entities.size()) return entities[0];
	return nullptr;
}

// Cache Walls to Graphics
void Game::cacheWalls() {
	wallSprites.clear();
	for (Vector2i& w : walls) {
		sf::RectangleShape rect(Vector2f(16, 16));
		rect.setPosition((float)w.x * C::GRID_SIZE, (float)w.y * C::GRID_SIZE);
		rect.setFillColor(sf::Color(0x07ff07ff));
		wallSprites.push_back(rect);
	}
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
	return gridx < 1.5f || gridx >= wallRightX;
}

// Check if there is a Wall (Collision) at this position
bool Game::isWall(int cx, int cy)
{
	for (Vector2i& w : walls) {
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