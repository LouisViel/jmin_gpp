#pragma once

namespace C
{
	// Game Constants
	static constexpr float FRAMERATE = 60.0f;
	static constexpr int GRID_SIZE = 16;
	static constexpr int RES_X = 1600;
	static constexpr int RES_Y = 900;

	// Entities Constants
	static constexpr float G = 9.81f * 10; // (positif = down, negatif = up)

	// Player Constants
	static constexpr float P_WIDTH = 0.5f;
	static constexpr float P_HEIGHT = 3.0f;
	static constexpr float P_SPEED = 5.0f;
	static constexpr float P_JUMP = 40.0f;
}
