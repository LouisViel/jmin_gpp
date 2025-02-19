#pragma once

namespace M
{
	#define NO_UPDATE(dt) if (dt <= 0.0) { return; }

	#define FULL_CHECK(entity, args) \
		/* Verify entity */ \
		if (!entity) return false; \
		\
		/* Prepare full check Variables */ \
		int xposMin(entity->rx - entity->swidth - C::E_ADJUSTMENT_X + entity->cx); \
		int xposMax(entity->rx + entity->swidth + entity->cx); \
		float cry(entity->cy + entity->ry); \
		\
		/* Process full body check */ \
		for (float ypos = cry, ytarget = cry - entity->sheight; ypos > ytarget; --ypos) { \
			for (int xpos = xposMin; xpos <= xposMax; ++xpos) { \
				if (args) return true;\
			} \
		} \
		\
		/* No collision */ \
		return false;
}