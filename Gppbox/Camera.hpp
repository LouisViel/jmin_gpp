#pragma once

#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include "Throttle.hpp"

class World;

class Camera
{
public:
	Camera(World* world, sf::Vector2f center, sf::Vector2f size);

	void update(double dt);
	void imgui();

	void setActive(sf::RenderTarget& win) const;
	void addShake(float duration, float intensity);

private:
	struct Shake
	{
		float duration; // in seconds
		float intensity; // radius, in pixels
	};

	sf::Vector2f processShake(double dt);

	// Camera Settings
	World* world = nullptr;
	sf::View m_view;

	// Screen Shake Settings
	Throttle m_throttle;
	std::vector<Shake> m_shakes;
};