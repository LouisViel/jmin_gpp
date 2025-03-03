#include "SpriteOverride.hpp"
#include "Entity.hpp"

SpriteOverride::SpriteOverride(Entity* entity, std::string text) : SpriteOverride(entity, "res/simple.vert", "res/simple.frag", text) { }
SpriteOverride::SpriteOverride(Entity* entity, std::string vert, std::string frag, std::string text) : Component(entity)
{
	texture = new sf::Texture();
	shader = new HotReloadShader(vert, frag);
	isValid = texture->loadFromFile(text);
	if (isValid) {
		entity->spr->setFillColor(sf::Color::White);
		entity->spr->setOutlineThickness(0);
		entity->spr->setTexture(texture);
		entity->defaultDraw = false;
	}
}

SpriteOverride::~SpriteOverride()
{
	delete shader;
	delete texture;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void SpriteOverride::update(double dt)
{
	// Update Shader
	if (!isValid) return;
	shader->update(dt);

	// Update draw direction
	int dirx = entity->dirx;
	if (dirx != pdirx) {
		pdirx = dirx;
		sf::Vector2f scale = entity->spr->getScale();
		scale.x = -scale.x;
		entity->spr->setScale(scale);
	}
}

void SpriteOverride::draw(sf::RenderTarget& win)
{
	// Apply Sprite drawing
	if (!isValid) return;
	shader->sh.setUniform("texture", texture);
	win.draw(*entity->spr);
}

void SpriteOverride::imgui()
{
	return;
}