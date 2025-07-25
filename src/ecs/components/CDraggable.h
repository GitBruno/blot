#pragma once
#include <glm/glm.hpp>
#include "../PropertyReflection.h"

namespace blot {
namespace ecs {

struct CDraggable {
	bool draggable = true;
	bool dragging = false;
	glm::vec2 dragOffset = glm::vec2(
		0.0f, 0.0f); // Offset from entity position to mouse when dragging

	std::vector<sProp> GetProperties() {
		return {{0, "Draggable", EPT_BOOL, &draggable},
				{1, "Dragging", EPT_BOOL, &dragging},
				{2, "Drag Offset X", EPT_FLOAT, &dragOffset.x},
				{3, "Drag Offset Y", EPT_FLOAT, &dragOffset.y}};
	}
};

} // namespace ecs
} // namespace blot
