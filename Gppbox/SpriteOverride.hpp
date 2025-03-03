#pragma once
#include <string>
#include "Component.hpp"
#include "HotReloadShader.hpp"

class SpriteOverride : public Component
{
private:
	HotReloadShader* shader = nullptr;
	sf::Texture* texture = nullptr;
	bool isValid = false;
	int pdirx = -1; // Same as entity default dirx

public:
	SpriteOverride(Entity* entity, std::string vert, std::string frag, std::string text);
	SpriteOverride(Entity* entity, std::string text);
	~SpriteOverride();

	void update(double dt) override;
	void draw(sf::RenderTarget& win) override;
	void imgui() override;
};