#include <filesystem>
#include <imgui.h>

#include "MapEditor.hpp"
#include "InputHandler.hpp";
#include "Environment.hpp";
#include "World.hpp"
#include "Game.hpp"
#include "Entity.hpp"
#include "M.hpp"
#include "C.hpp"


MapEditor::MapEditor(sf::RenderWindow* win, Environment* environment, World* world)
{
	this->win = win;
	this->environment = environment;
	this->world = world;
	eSpr = world->initEnnemyCore(-100, -100);
}

MapEditor::~MapEditor()
{
	delete eSpr;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void MapEditor::update(double dt)
{
	// Check is active and if game focused
	if (!active) return;
	valid = InputHandler::hasFocus();
	valid &= InputHandler::canUseMouse();
	if (!valid) return;

	// Get & Check if Mouse pos is in window 
	sf::Vector2f pos = win->mapPixelToCoords(sf::Mouse::getPosition(*win));
	if (pos.x < 0.0f || pos.x > C::RES_X || pos.y < 0.0f || pos.y > C::RES_Y) {
		valid = false;
		return;
	}

	// Get, Process, & Normalize Inputs
	mousePos = sf::Vector2i(int(pos.x / C::GRID_SIZE), int(pos.y / C::GRID_SIZE));
	leftButton = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	rightButton = sf::Mouse::isButtonPressed(sf::Mouse::Right);

	// Sync Ennemy processing position
	if (tileType == TileType::Ennemy) {
		eSpr->setCooPixel(pos.x + C::E_ADJUSTMENT_X + 0.25f * C::GRID_SIZE, pos.y + 0.8f * C::GRID_SIZE);
		eSpr->roundCoo();
		eSpr->rx = 0.25f;
		eSpr->ry = 0.99f;
		eSpr->cy -= 1;
		eSpr->syncPos();
	}

	// Check if target is occupied
	Game* g = Game::singleton;
	isOccupied(g);

	// Destroy Object if asked to + occupied
	if (occupied && rightButton) {
		if (!g->isPlayer(mousePos.x, mousePos.y)) {
			removeAny();
			isOccupied(g);
		}
	}

	// Spawn Object if asked to + not occupied 
	if (!occupied && leftButton) {
		if (tileType == TileType::Wall) addWall();
		else if (tileType == TileType::Ennemy) addEnnemy();
		occupied = true;
	}
}

void MapEditor::isOccupied(Game* const g)
{
	if (tileType == TileType::Wall) occupied = g->isOccupied(mousePos.x, mousePos.y);
	else if (tileType == TileType::Ennemy) occupied = g->isOccupied(eSpr);
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void MapEditor::imgui()
{
	using namespace ImGui;
	if (TreeNodeEx("Editor", ImGuiTreeNodeFlags_DefaultOpen)) {

		// Load / Save / Reset Buttons
		ImGui::Text("Save Settings :");
		if (Button("Load Map")) load();
		SameLine();
		ImGui::Text(" --- ");
		SameLine();
		if (Button("Save Map")) save();
		SameLine();
		ImGui::Text(" --- ");
		SameLine();
		if (Button("Reset Map")) loadDefault();

		// Editor Toggle Button
		Dummy(ImVec2(0.0f, 6.0f));
		ImGui::Text("Edit Mode Settings");
		if (active && Button("Go back Playing")) active = false;
		if (!active && Button("Enter Edit Mode")) active = true;

		// Show only in edit mode
		if (active) {

			// Tile Place type
			constexpr int typeSize = 2;
			static const char* tileTypes[typeSize]{ "Wall", "Ennemy" };
			int currentType = (int)tileType;
			Combo("Tiletype", &currentType, tileTypes, typeSize, IM_ARRAYSIZE(tileTypes));
			tileType = (TileType)currentType;

			// Editor Hints
			BulletText("Left click to place");
			BulletText("Right click to remove");

			Value("pcx", world->getPlayer()->cx);
			Value("prx", world->getPlayer()->rx);
			Value("pcy", world->getPlayer()->cy);
			Value("pry", world->getPlayer()->ry);

			Value("cx", eSpr->cx);
			Value("rx", eSpr->rx);
			Value("cy", eSpr->cy);
			Value("ry", eSpr->ry);
		}

		Value("Entities", (int)world->entities->size());

		Dummy(ImVec2(0.0f, 6.0f));
		TreePop();
	}
}

void MapEditor::draw(sf::RenderTarget& win)
{
	if (!(active && valid)) return;
	if (tileType == TileType::Wall) drawWall(win);
	else if (tileType == TileType::Ennemy) drawEnnemy(win);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void MapEditor::drawWall(sf::RenderTarget& win)
{
	// Draw Color coded Cursor Target
	sf::RectangleShape rect(sf::Vector2f(C::GRID_SIZE, C::GRID_SIZE));
	rect.setPosition((float)mousePos.x * C::GRID_SIZE, (float)mousePos.y * C::GRID_SIZE);
	rect.setFillColor(sf::Color(occupied ? sf::Color::Red : sf::Color::Green));
	win.draw(rect);
}

void MapEditor::drawEnnemy(sf::RenderTarget& win)
{
	// Draw Color coded Ennemy Visual Target
	if (occupied) eSpr->spr->setFillColor(sf::Color{ 0xff0000cc });
	else eSpr->spr->setFillColor(sf::Color{ 0x00ff00cc });
	eSpr->draw(win);
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void MapEditor::save()
{
	ofstream outfile(C::SAVE_FILE.data(), ofstream::trunc);
	try {
		saveWall(outfile);
		saveEnnemy(outfile);
	} catch (...) { }
	outfile.close();
}

void MapEditor::load()
{
	if (!std::filesystem::exists(C::SAVE_FILE.data())) {
		loadDefault();
		return;
	}

	ifstream infile(C::SAVE_FILE.data());
	try { loadInternal(infile); }
	catch (...) { loadDefault(); }
	infile.close();
}

void MapEditor::loadInternal(std::ifstream& infile)
{
	clearAll();
	loadWall(infile);
	loadEnnemy(infile);
}

void MapEditor::loadDefault()
{
	clearAll();
	loadWallDefault();
	loadEnnemyDefault();
}

void MapEditor::clearAll()
{
	clearWall();
	clearEnnemy();
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void MapEditor::saveWall(ofstream& outfile)
{
	for (sf::Vector2i w : environment->walls)
		outfile << w.x << " " << w.y << "\n";
	outfile << C::SAVE_SPLIT;
}

void MapEditor::loadWall(ifstream& infile)
{
	int x, y;
	std::string wallLine;
	while (getline(infile, wallLine)) {
		istringstream line(wallLine);
		if (!(line >> x >> y)) break;
		environment->walls.push_back({ x, y });
	}
	environment->cacheWalls();
}

void MapEditor::clearWall()
{
	environment->walls.clear();
	environment->cacheWalls();
}

void MapEditor::loadWallDefault()
{
	using namespace sf;

	int cols = C::RES_X / C::GRID_SIZE;
	int lastLine = C::RES_Y / C::GRID_SIZE - 1;
	std::vector<Vector2i>& walls = environment->walls;

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

	environment->cacheWalls();
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void MapEditor::saveEnnemy(ofstream& outfile)
{
	for (Entity* e : *world->ennemies) {
		outfile << e->cx << " " << e->rx << " "
			<< e->cy << " " << e->ry << "\n";
	}
	outfile << C::SAVE_SPLIT;
}

void MapEditor::loadEnnemy(ifstream& infile)
{
	float cx, cy, rx, ry;
	std::string wallLine;
	while (getline(infile, wallLine)) {
		istringstream line(wallLine);
		if (!(line >> cx >> rx >> cy >> ry)) break;
		world->initEnnemy(cx + rx, cy + ry);
	}
}

void MapEditor::clearEnnemy()
{
	std::vector<Entity*>* ennemies = world->ennemies;
	for (int i = ennemies->size() - 1; i >= 0; --i) world->removeEnnemy(ennemies->operator[](i));
	ennemies->clear();
}

void MapEditor::loadEnnemyDefault()
{
	world->initEnnemy(7, int(C::RES_Y / C::GRID_SIZE) - 4 + 0.99f);
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void MapEditor::addWall()
{
	environment->walls.push_back(mousePos);
	environment->cacheWalls();
}

void MapEditor::addEnnemy()
{
	world->initEnnemy(eSpr->cx + eSpr->rx, eSpr->cy + eSpr->ry);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

inline void MapEditor::removeAny()
{
	Game* g = Game::singleton;
	if (g->isWall(mousePos.x, mousePos.y)) removeWall();
	if (g->isEnnemy(mousePos.x, mousePos.y)) removeEnnemy();
}


void MapEditor::removeWall()
{
	std::vector<sf::Vector2i>* walls = &environment->walls;
	REMOVE_ITEM(sf::Vector2i&, walls, mousePos);
	environment->cacheWalls();
}

void MapEditor::removeEnnemy()
{
	Entity* e = world->getEnnemy(mousePos.x, mousePos.y);
	if (e != nullptr) world->removeEnnemy(e);
}