#pragma once
#include <SFML/Graphics.hpp>

namespace sf {
	class RenderWindow;
}

class InputHandler
{
private:
	static sf::RenderWindow* window;

public:
	static void setWindow(sf::RenderWindow* window);
	static bool hasFocus();

	static bool canUseMouse();
	static bool canUseKeyboard();
	static bool canUse();

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

private:
	enum JoystickButton
	{
		Down = 0,
		Right = 1,
		Up = 3,
		Left = 2,

		TriggerLeft = 4,
		TriggerRight = 5,

		JoystickLeft = 8,
		JoystickRight = 9,

		MenuLeft = 6, // menu button
		MenuRight = 7, // share button
		MenuMiddle = 11, // upload button
		MenuTop = 10, // xbox button
	};

	enum JoystickAxis
	{
		LeftHorizontal = sf::Joystick::Axis::X, // neg = left, pos = right
		LeftVertical = sf::Joystick::Axis::Y, // neg = bottom, top = up

		RightHorizontal = sf::Joystick::Axis::U, // neg = left, pos = right
		RightVertical = sf::Joystick::Axis::V, // neg = bottom, top = up

		PadHorizontal = sf::Joystick::Axis::PovX, // neg = left, pos = right
		PadVertical = sf::Joystick::Axis::PovY, // neg = bottom, top = up

		ShoulderLeft = sf::Joystick::Axis::Z, // positif
		ShoulderRight = sf::Joystick::Axis::Z, // negative
	};

	#define getJoystickAxis(axis) sf::Joystick::getAxisPosition(0, (sf::Joystick::Axis)axis) / 100.0f;
	#define isJoystickPressed(button) sf::Joystick::isButtonPressed(0, (unsigned int)button);

public:
	static sf::Vector2f getHorizontal();
	static bool getJump();
	static bool getFire();
	static bool getSwitch();
	static bool getDebug();
};