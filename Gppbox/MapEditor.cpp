#include <filesystem>
#include <imgui.h>

#include "MapEditor.hpp"
#include "InputHandler.hpp";
#include "World.hpp";
#include "Game.hpp"
#include "C.hpp"


MapEditor::MapEditor(sf::RenderWindow* win, World* world)
{
	this->win = win;
	this->world = world;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void MapEditor::update(double dt)
{
	// Check is active and if game focused
	if (!active) return;
	valid = InputHandler::canUseMouse();
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

	// Check if target is occupied
	Game* g = Game::singleton;
	occupied = g->isOccupied(mousePos.x, mousePos.y);

	// Destroy Object if asked to + occupied
	if (occupied && rightButton) {
		if (!g->isPlayer(mousePos.x, mousePos.y)) {
			removeAny();
			occupied = false;
		}
	}

	// Spawn Object if asked to + not occupied 
	if (!occupied && leftButton) {
		if (tileType == TileType::Wall) addWall();
		else if (tileType == TileType::Ennemy) addEnnemy();
		occupied = true;
	}
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void MapEditor::draw(sf::RenderWindow& win)
{
	if (!(active && valid)) return;

	// Draw Color coded Cursor Target
	sf::RectangleShape rect(sf::Vector2f(C::GRID_SIZE, C::GRID_SIZE));
	rect.setPosition((float)mousePos.x * C::GRID_SIZE, (float)mousePos.y * C::GRID_SIZE);
	rect.setFillColor(sf::Color(occupied ? sf::Color::Red : sf::Color::Green));
	win.draw(rect);
}

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
		}

		Dummy(ImVec2(0.0f, 6.0f));
		TreePop();
	}
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
	auto& walls = world->walls;
	for (Vector2i w : walls) outfile << w.x << " " << w.y << "\n";
	outfile << C::SAVE_SPLIT;
}

void MapEditor::loadWall(ifstream& infile)
{
	auto& walls = world->walls;

	int x, y;
	std::string wallLine;
	while (getline(infile, wallLine)) {
		istringstream line(wallLine);
		if (!(line >> x >> y)) break;
		walls.push_back({ x, y });
	}

	world->cacheWalls();
}

void MapEditor::clearWall()
{
	world->walls.clear();
	world->cacheWalls();
}

void MapEditor::loadWallDefault()
{
	int cols = C::RES_X / C::GRID_SIZE;
	int lastLine = C::RES_Y / C::GRID_SIZE - 1;
	auto& walls = world->walls;

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

	world->cacheWalls();
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void MapEditor::saveEnnemy(ofstream& outfile)
{
	// TODO : Implement
}

void MapEditor::loadEnnemy(ifstream& infile)
{
	// TODO : Implement
}

void MapEditor::clearEnnemy()
{
	// TODO : Implement
}

void MapEditor::loadEnnemyDefault()
{
	// TODO : Implement
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void MapEditor::addWall()
{
	world->walls.push_back(mousePos);
	world->cacheWalls();
}

void MapEditor::addEnnemy()
{
	// TODO : Implement
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
	// Init Variables
	auto& walls = world->walls;
	int index = -1;

	// Search for Wall Index
	for (int i = 0; i < walls.size(); ++i) {
		if (walls[i] == mousePos) {
			index = i;
			break;
		}
	}

	// Erase if founded
	if (index != -1) {
		walls.erase(walls.begin() + index);
		world->cacheWalls();
	}
}

void MapEditor::removeEnnemy()
{
	// TODO : Implement
}