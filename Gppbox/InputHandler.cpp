#include <imgui.h>
#include "InputHandler.hpp"
#include <SFML/Graphics.hpp>

sf::RenderWindow* InputHandler::window = nullptr;
void InputHandler::setWindow(sf::RenderWindow* window)
{
	InputHandler::window = window;
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


bool InputHandler::hasFocus()
{
	if (!window) return false;
	return window->hasFocus();
}


//////////////////////////////////////////////////////////////////
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