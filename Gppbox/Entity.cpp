#include <imgui.h>
#include "Entity.hpp"
#include "Component.hpp"
#include "Game.hpp"
#include "C.hpp"

Entity::Entity(sf::Shape* spr) : Entity(spr, nullptr, 0) { }

Entity::Entity(sf::Shape* _spr, Component** components, int componentCount) : spr(_spr)
{
	if (components == nullptr) this->components = new std::vector<Component*>();
	else this->components = new std::vector<Component*>(components, components + componentCount);
}

Entity::~Entity()
{
	if (spr) delete spr;
	for (Component* c : *components) delete c;
	delete components;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Entity::addComponent(Component* component)
{
	if (component == nullptr) return;
	this->components->push_back(component);
}

void Entity::addComponents(Component** components, int componentCount)
{
	if (components == nullptr || componentCount <= 0) return;
	this->components->insert(this->components->end(), components, components + componentCount);
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Entity::preupdate(double dt)
{
	hcollision = 0; // Reset Horizontal collision tag
	for (Component* c : *components) c->preupdate(dt);
}

void Entity::fixed(double fdt)
{
	for (Component* c : *components) c->fixed(fdt);
	processMovement(fdt);
}

void Entity::update(double dt)
{
	for (Component* c : *components) c->update(dt);
	syncPos();
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Entity::draw(sf::RenderWindow& win)
{
	if (spr) win.draw(*spr);
}

void Entity::imgui()
{
	using namespace ImGui;
	bool chg = false, chgCoo = false;

	Value("jumping", isJumping);
	Value("grounded", isGrounded);
	Value("cx", cx);
	Value("cy", cy);
	Value("rx", rx);
	Value("ry", ry);

	sf::Vector2i pix = getPosPixel();
	chg |= DragInt2("pix x/pix y", &pix.x, 1.0f, -2000, 2000);
	if (chg) setCooPixel(pix.x, pix.y);

	chg |= DragInt2("cx/cy", &cx, 1.0f, -2000, 2000);

	sf::Vector2f coo = { cx + rx, cy + ry };
	chgCoo = DragFloat2("coo grid x/y", &coo.x, 1.0f, -2000, 2000);
	if (chgCoo) setCooGrid(coo.x, coo.y);

	chg |= DragFloat2("dx/dy", &dx, 0.01f, -20, 20);
	chg |= DragFloat2("frx/fry", &frx, 0.001f, 0, 1);
	chg |= DragFloat("gravy/fry", &gravy, 0.001f, -2, 2);

	if (Button("reset")) {
		chg |= true;

		cx = 3;
		cy = 54;
		rx = 0.5f;
		ry = 0.99f;

		dx = dy = 0.0f;
		setGrounded(false);
		setJumping(false);
	}

	//return chg || chgCoo;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


inline void Entity::processMovement(double fdt)
{
	Game& g = *Game::singleton;
	double rate = 1.0 / fdt; // How many times in 1 second (1 second / deltatime)
	double dfr = C::F_REF / rate; // Normalize rate from framerate

	float frxdfr = isGrounded ? dfr * C::E_FR_GROUND : dfr;
	setDy(dy + C::G * gravy * fdt); // Apply Gravity
	dx *= pow(frx, frxdfr); // Apply Friction x
	dy *= pow(fry, dfr); // Apply Friction y

	float _rx = rx + dx * fdt; // Calculate internal movement x
	float _ry = ry + dy * fdt; // Calculate internal movement y
	processHorizontal(g, _rx, _ry);
	processVertical(g, _rx, _ry);
	rx = _rx;
	ry = _ry;
}

void Entity::processHorizontal(Game& g, float& _rx, const float& _ry)
{
	// No Process Needed
	if (dx == 0.0f) return;

	// Pre-Process Variables
	bool isCollision = false;
	int yposMax(ry - sheight);
	float xposMax(_rx + swidth), rxMax(rx + swidth);
	float xposMin(_rx - swidth), rxMin(rx - swidth);

	// Check & Need to process Right
	if (dx > 0.0f) {

		// Check if need to process collisions
		if (int(xposMax) > int(rxMax)) {
			// Check for right collisions // TODO : Ajouter une verif "anti-teleportation" (Normalisée grace au fixed-update)
			for (float xpos = _rx + cx, xtarget = xposMax + 1 + cx; xpos < xtarget && !isCollision; ++xpos) {
				for (float ypos = ry + cy, ytarget = yposMax + cy; ypos > ytarget && !isCollision; --ypos)
					isCollision = g.hasCollision(xpos, ypos);
			}
		}
		

		// Blocked on Right by Collision
		if (isCollision) {
			dx = 0; // Cancel Move Speed
			_rx = rx; // Reset internal position x
			hcollision = 1;

		// Process movement on Right
		} else {
			// Update internal & full position x
			int rxi = int(_rx);
			cx += rxi;
			_rx -= rxi;
		}

		// End Horizontal Process
		return;
	}

	// Check & Need to process Left
	if (dx < 0.0f) {
		
		// Check if need to process collisions
		//if (int(xposMin) < int(rxMin)) {
			// Check for left collisions // TODO : Ajouter une verif "anti-teleportation" (Normalisée grace au fixed-update)
			for (float xpos = _rx + cx, xtarget = xposMin - 1 + cx; xpos > xtarget && !isCollision; --xpos) {
				for (float ypos = ry + cy, ytarget = yposMax + cy; ypos > ytarget && !isCollision; --ypos) {
					isCollision = g.hasCollision(xpos, ypos);
				}
			}
		//}

		// Blocked on Left by Collision
		if (isCollision) {
			dx = 0; // Cancel Move Speed
			_rx = rx; // Reset internal position x
			hcollision = -1;

		// Process movement on Left
		} else if (_rx < 0.0f) {
			// Update internal & full position x
			int rxi(_rx);
			if (_rx - rxi != 0.0f) rxi -= 1;
			cx += rxi;
			_rx -= rxi;
		}

		// End Horizontal Process
		return;
	}
}

void Entity::processVertical(Game& g, const float& _rx, float& _ry)
{
	// No Process Needed
	if (dy == 0.0f) return;

	// Pre-Process Variables
	bool isCollision = false;
	float xposMin(_rx - swidth);
	int xposMax(_rx + swidth + 1);
	float cry(cy + _ry);

	// Check & Need to process Down
	if (dy > 0.0f && _ry > 1.0f) {

		// Check for down collisions // TODO : Ajouter une verif "anti-teleportation" (Normalisée grace au fixed-update)
		for (float xpos = xposMin + cx, target = xposMax + cx;
			xpos < target && !isCollision; ++xpos
		) isCollision = g.hasCollision(xpos, cry, false);

		// Process Is Grounded
		if (isCollision) {
			dy = 0.0f; // Cancel Movement y
			_ry = 0.99f; // Attach ry to ground

		// Process Gravity/Falling
		} else {
			// Update internal & full position y
			int ryi(_ry);
			cy += ryi;
			_ry -= ryi;
		}

		// Furtherprocess is Grounded (allowing bit of tolerance)
		bool _isGrounded = isCollision;
		for (float ypos = cry + 1.0f, ytarget = ypos + C::P_JUMP_TOL; !_isGrounded && ypos < ytarget; ++ypos) {
			for (float xpos = xposMin + cx, target = xposMax + cx; xpos < target && !_isGrounded; ++xpos) 
				_isGrounded = g.hasCollision(xpos, ypos, false);
		}
		setGrounded(_isGrounded);

		// End Vertical Process
		return;
	}

	
	// Check & Need to process Up
	if (dy < 0.0f) {
		// Invoke Grounded Callback
		setGrounded(false);

		// Check if need to process collisions
		if (int(_ry - sheight) != int(ry - sheight)) {
			// Check for up collisions (+ Allow single platform bypass) // TODO : Ajouter une verif "anti-teleportation" (Normalisée grace au fixed-update)
			for (float xpos = xposMin + cx, xtarget = xposMax + cx, collisionCount = 0; xpos < xtarget && !isCollision; ++xpos) {
				for (float ypos = cry, ytarget = cry - sheight - 1; ypos > ytarget && !isCollision; --ypos) {
					if (g.hasCollision(xpos, ypos, false)) {
						if (++collisionCount > 1) isCollision = true;
					} else collisionCount = 0;
				}
			}
		}

		// Process se cogne la tete
		if (isCollision) {
			dy = 0.0f; // Cancel Movement y
			_ry = ry; // Reset ry at frame start state

		// Process Jumping/Flying
		} else if (_ry < 0.0f) {
			// Update internal & full position y
			int ryi(_ry);
			if (_ry - ryi != 0.0f) --ryi;
			cy += ryi;
			_ry -= ryi;
		}

		// End Vertical Process
		return;
	}
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Entity::setCooPixel(int px, int py)
{
	cx = px / C::GRID_SIZE;
	cy = py / C::GRID_SIZE;
	rx = (px - (cx * C::GRID_SIZE)) / (float)C::GRID_SIZE;
	ry = (py - (cy * C::GRID_SIZE)) / (float)C::GRID_SIZE;
	syncPos();
}

void Entity::setCooGrid(float coox, float cooy)
{
	cx = (int)coox;
	rx = coox - cx;
	cy = (int)cooy;
	ry = cooy - cy;
	syncPos();
}

void Entity::roundCoo()
{
	rx = std::round(rx);
	ry = std::round(ry);
	if (rx != 0.0f) {
		cx += rx;
		rx = 0.0f;
	}
	if (ry != 0.0f) {
		cy += ry;
		ry = 0.0f;
	}
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Entity::setGrounded(bool state)
{
	// Entity is Grounded
	if (state) {
		if (isGrounded) return;
		isGrounded = true;
		setJumping(false);

	// Entity isnt Grounded
	} else {
		if (!isGrounded) return;
		isGrounded = false;
	}

	// Call On Jumping Callbacks
	for (Component* c : *components) c->onGrounded(isGrounded);
}

void Entity::setJumping(bool state)
{
	// Entity is Jumping
	if (state) {
		if (!canJump()) return;
		isJumping = true;
		setDy(-jumpforce);

	// Entity isnt Jumping
	} else if (isJumping) {
		isJumping = false;

	// No more processing
	} else return;

	// Call On Jumping Callbacks
	for (Component* c : *components) c->onJumping(isJumping);
}

bool Entity::canJump() const
{
	// Can Jump only if all components allow it
	for (Component* c : *components) {
		if (!c->canJump()) return false;
	}
	return true;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void Entity::setDx(double dx)
{
	this->dx = std::clamp(dx, -C::E_MAX_X, C::E_MAX_X);
}

void Entity::setDy(double dy)
{
	this->dy = std::clamp(dy, -C::E_MAX_Y, C::E_MAX_Y);
}

void Entity::syncPos()
{
	if (spr) spr->setPosition(sf::Vector2f {
		(cx + rx) * C::GRID_SIZE - C::S_ADJUSTMENT_X,
		(cy + ry) * C::GRID_SIZE - C::S_ADJUSTMENT_Y
	});
}

sf::Vector2i Entity::getPosPixel()
{
	return sf::Vector2i((cx + rx) * C::GRID_SIZE, (cy + ry) * C::GRID_SIZE);
}
