#pragma once

namespace C
{
	// App Constants
	static constexpr float F_REF = 60.0f; // Framerate Processing Reference
	static constexpr double F_FIXED = 1.0f / 50.0f; // Framerate for Fixed Update
	static constexpr float F_TARGET = 60.0f; // 60.0f // App Target Framerate
	#define SAVE_FILE "gamemap.save"
	#define SAVE_SPLIT "--\n"

	// Game Constants
	static constexpr int GRID_SIZE = 16;
	static constexpr int RES_X = 1600;
	static constexpr int RES_Y = 900;

	// Camera Constants
	static constexpr float C_CENTER_X = RES_X / 2.0f;
	static constexpr float C_CENTER_Y = RES_Y / 2.0f;
	static constexpr float C_SIZE_X = RES_X / 2.5f;
	static constexpr float C_SIZE_Y = RES_Y / 2.5f;

	// Entities Constants
	static constexpr float G = 9.81f * 11; // (positif = down, negatif = up)
	static constexpr double E_MAX_X = 30.0f; // Max x speed
	static constexpr double E_MAX_Y = 49.0f; // Max y Speed
	static constexpr float E_FR_GROUND = 1.45f; // Friction Ground multiplier
	static constexpr float E_ADJUSTMENT_X = 0.725f; // Process Adjusment on x axis

	// Sprite/Rendering Constants
	static constexpr float S_ADJUSTMENT_X = 5.0f; // Sprite position Adjustment (axis x)
	static constexpr float S_SCALER_X = 1.2f; // Sprite Scale Adjustment (axis x)
	static constexpr float S_ADJUSTMENT_Y = 7.5f; // Sprite position Adjustment (axis y)
	static constexpr float S_SCALER_Y = 1.2f; // Sprite Scale Adjustment (axis y)

	// Player Constants
	static constexpr float P_WIDTH = 0.5f; // Player Width
	static constexpr float P_HEIGHT = 3.0f; // Player height
	static constexpr float P_SPEED = 5.0f * F_REF; // Move Speed
	static constexpr float P_JUMP = 35.0f; // Jump Force
	
	// Player Tolerance Settings
	static constexpr float P_JUMP_TOL = 0.0f; // Jump tolerance when falling
	static constexpr float P_COYOTEE = 0.3f; // Jump coyotee tolerance
	static constexpr float P_JUMPD = 0.075f; // Jump delay between grounded and new jump

	// Life Constants
	static constexpr float P_LIFEPOINTS = 100.0f;
	static constexpr float E_LIFEPOINTS = 50.0f;
}