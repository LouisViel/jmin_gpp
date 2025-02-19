#include "Utils.hpp"
#include "Entity.hpp"
#include "M.hpp"
#include "C.hpp"

bool Utils::isFullBody(Entity* entity, int gridx, int gridy)
{
	FULL_CHECK(entity, (xpos == gridx && int(ypos) == gridy));
}