#pragma once

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
};