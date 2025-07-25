#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "../PropertyReflection.h"
#include "CGuide.h"

namespace blot {
namespace ecs {

struct CTexture {
	unsigned int textureId = 0; // OpenGL/Vulkan/DirectX/etc. texture or FBO ID
	int width = 0;
	int height = 0;
	bool renderTarget = false; // true if this is a canvas/FBO
	// Optionally: format, filtering, mipmaps, etc.
	// int format = GL_RGBA;
	// int filtering = GL_LINEAR;

	// View state fields (from old CCanvas)
	float zoom = 1.0f;
	glm::vec2 offset = glm::vec2(0.0f, 0.0f);
	bool showRulers = false;
	bool showGuides = false;
	std::vector<CGuide> guides;

	std::vector<sProp> GetProperties() {
		return {{0, "Texture ID", EPT_UINT, &textureId},
				{1, "Width", EPT_INT, &width},
				{2, "Height", EPT_INT, &height},
				{3, "Render Target", EPT_BOOL, &renderTarget},
				{4, "Zoom", EPT_FLOAT, &zoom},
				{5, "Offset X", EPT_FLOAT, &offset.x},
				{6, "Offset Y", EPT_FLOAT, &offset.y},
				{7, "Show Rulers", EPT_BOOL, &showRulers},
				{8, "Show Guides", EPT_BOOL, &showGuides}};
	}
};

} // namespace ecs
} // namespace blot
