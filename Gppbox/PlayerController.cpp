#include "PlayerController.hpp"
#include "InputHandler.hpp"
#include "Entity.hpp"
#include "M.hpp"
#include "C.hpp"

PlayerController::PlayerController(Entity* entity) : Component(entity) { }


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void PlayerController::preupdate(double dt)
{
	// No logistic Update
	NO_UPDATE(dt);
	processInputs(dt);

	// Update Coyotee Timer
	if (!entity->isGrounded && coyoteeTime > 0.0f) {
		coyoteeTime -= dt;
	}

	// Update Jump Delay Timer
	if (jumpDelay > 0.0f) {
		jumpDelay -= dt;
	}
}

void PlayerController::fixed(double fdt) { }

void PlayerController::update(double dt) { }

void PlayerController::imgui() { }


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void PlayerController::processInputs(double dt)
{
	// Check for game window Focus + Imgui not overriding it
	if (!InputHandler::hasFocus()) return;

	// Get & Apply Horizontal Movement
	sf::Vector2f hor = InputHandler::getHorizontal();
	if (hor.x > 0.5f) entity->setDx(entity->dx - entity->speed * dt * hor.x);
	else if (hor.y > 0.5f) entity->setDx(entity->dx + entity->speed * dt * hor.y);

	// Get & Apply Jump
	if (InputHandler::getJump()) {
		entity->setJumping(true);
	}
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void PlayerController::onGrounded(bool state)
{
	if (state) {
		coyoteeTime = C::P_COYOTEE;
		jumpDelay = C::P_JUMPD;
	}
}

void PlayerController::onJumping(bool state)
{
	if (state) {
		coyoteeTime = 0.0f;
	}
}

bool PlayerController::canJump()
{
	return jumpDelay <= 0.0f && coyoteeTime > 0.0f;
}