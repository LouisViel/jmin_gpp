#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class Game;
class Component;
namespace sf {
	class RenderWindow;
}

class Entity {
public:
	std::vector<Component*>* components = nullptr;
	sf::Shape* spr = nullptr; // Pointeur vers le sprite lié
	float sheight = 0.0f; // Sprite height (center at bottom)
	float swidth = 0.0f; // Sprite width (center at middle)

	int	cx = 0; // Position "case" en x
	int	cy = 0; // Position "case" en y
	float rx = 0.5f; // Reste de position en x
	float ry = 0.0f; // Reste de position en y

	float dx = 0.0f; // Entity Movement Speed en x
	float dy = 0.0f; // Entity Movement Speed en y
	float gravy = 1.0f; // Entity Gravity multiplier

	float frx = 0.84f; // Friction en x
	float fry = 1.0f; // Friction en y

	float speed = 1.0f; // Move Speed (x axis)
	float jumpforce = 10.0f; // Jump Force (y axis)

	bool isJumping = false; // Is Entity Jumping
	bool isGrounded = false; // Is Entity Grounded
	int hcollision = 0; // Horizontal Collision : -1 = left, 0 = none, 1 = right


	Entity(sf::Shape* spr); // Entity Constructor, no components
	Entity(sf::Shape* spr, Component** components, int componentCount); // Entity Constructor
	~Entity(); // Entity Destructor

	void addComponent(Component* component); // Add one component
	void addComponents(Component** components, int componentCount); // Add multiple components

	void preupdate(double dt); // Pre Update Entity
	void fixed(double fdt); // Fixed Update Entity
	void update(double dt); // Update Entity
	void draw(sf::RenderTarget& win); // Graphics Drawing
	void imgui(); // Imgui Drawing
	
	inline void processMovement(double fdt); // Process Entity Full Movement
	void processHorizontal(Game& g, float& _rx, const float& _ry); // Internal Process Movement Horizontal
	void processVertical(Game& g, const float& _rx, float& _ry); // Internal Process Movement Vertical

	void setCooPixel(int px, int py); // Set Coordinate (Using Screen as referencial)
	void setCooGrid(float coox, float cooy); // Set Coordinate (Using Grid as referencial)
	void roundCoo(); // Round coordinates

	void setGrounded(bool state); // Grounded callback
	void setJumping(bool state); // Set Jumping state
	bool canJump() const; // Check if can Jump

	void setDx(double dx); // Set dx (using clamp)
	void setDy(double dy); // Set dy (using clamp)
	void syncPos(); // Sync Sprite position with internal position
	sf::Vector2i getPosPixel(); // Get Coordinate (Using Screen as referencial)
};