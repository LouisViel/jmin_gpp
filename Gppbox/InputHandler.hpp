#pragma once

class InputHandler
{
public:
	InputHandler();
	static bool canUseMouse();
	static bool canUseKeyboard();
	static bool canUse();
};