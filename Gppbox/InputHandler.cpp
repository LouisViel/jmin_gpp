#include <imgui.h>
#include "InputHandler.hpp"


InputHandler::InputHandler()
{
	
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