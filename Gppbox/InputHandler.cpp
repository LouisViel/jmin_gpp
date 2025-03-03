#include <imgui.h>
#include "InputHandler.hpp"
#include "M.hpp"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

sf::RenderWindow* InputHandler::window = nullptr;
void InputHandler::setWindow(sf::RenderWindow* window)
{
	InputHandler::window = window;
}

bool InputHandler::hasFocus()
{
	if (!window) return false;
	return window->hasFocus();
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

bool InputHandler::canUseMouse()
{
	ImGuiIO& io = ImGui::GetIO();
	return !io.WantCaptureMouse;
}

bool InputHandler::canUseKeyboard()
{
	ImGuiIO& io = ImGui::GetIO();
	return !io.WantCaptureKeyboard;
}

bool InputHandler::canUse()
{
	ImGuiIO& io = ImGui::GetIO();
	return !(io.WantCaptureMouse || io.WantCaptureKeyboard);
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

sf::Vector2f InputHandler::getHorizontal()
{
	bool leftKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q);
	bool leftPad = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
	bool rightKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
	bool rightPad = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
	float axisJoy = getJoystickAxis(JoystickAxis::LeftHorizontal);
	float padJoy = getJoystickAxis(JoystickAxis::PadHorizontal);

	sf::Vector2f result;
	if (leftKey || leftPad) result.x = 1.0f;
	else { GET_AXIS_MULTIPLE(result.x, < 0.0f, ({ axisJoy, padJoy })); }
	if (rightKey || rightPad) result.y = 1.0f;
	else { GET_AXIS_MULTIPLE(result.y, > 0.0f, ({ axisJoy, padJoy })); }
	return result;
}

bool InputHandler::getJump()
{
	bool spaceKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
	bool upKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z);
	bool upPad = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
	bool jumpJoy = isJoystickPressed(JoystickButton::Down);
	return spaceKey || upKey || upPad || jumpJoy;
}

bool InputHandler::getFire()
{
	bool fireKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F);
	bool fireMouse = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	bool triggerJoy = isJoystickPressed(JoystickButton::TriggerRight);
	bool buttonJoy = isJoystickPressed(JoystickButton::Right);
	return fireKey || fireMouse || triggerJoy || buttonJoy;
}

bool InputHandler::getSwitch()
{
	bool switchKey = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
	bool switchMouse = sf::Mouse::isButtonPressed(sf::Mouse::Right);
	bool triggerJoy = isJoystickPressed(JoystickButton::TriggerLeft);
	bool buttonJoy = isJoystickPressed(JoystickButton::Left);
	return switchKey || switchMouse || triggerJoy || buttonJoy;
}

bool InputHandler::getDebug()
{
	bool debugKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P);
	bool debugPad = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Multiply);
	bool debugJoy = isJoystickPressed(JoystickButton::MenuLeft);
	return debugKey || debugPad || debugJoy;
}
