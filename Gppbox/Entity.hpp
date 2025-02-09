#pragma once

#include <SFML/Graphics.hpp>

class Game;
namespace sf {
	class RenderWindow;
}

class Entity{
public:
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
	float coyoteeTime = 0.0f;
	float jumpDelay = 0.0f;


	Entity(sf::Shape* spr); // Entity Constructor

	void preupdate(double dt); // Pre Update Entity
	void fixed(double fdt); // Fixed Update Entity
	void update(double dt); // Update Entity
	void draw(sf::RenderWindow& win); // Graphics Drawing
	bool im(); // Imgui Drawing
	
	void processHorizontal(Game& g, float& _rx, const float& _ry);
	void processVertical(Game& g, const float& _rx, float& _ry);

	void setCooPixel(int px, int py); // Set Coordinate (Using Screen as referencial)
	void setCooGrid(float coox, float cooy); // Set Coordinate (Using Grid as referencial)

	void setGrounded(bool state); // Grounded callback
	void setJumping(bool state); // Set Jumping state
	inline bool canJump() const;

	void setDx(double dx); // Set dx (using clamp)
	void setDy(double dy); // Set dy (using clamp)
	void syncPos(); // Sync Sprite position with internal position
	sf::Vector2i getPosPixel(); // Get Coordinate (Using Screen as referencial)
};