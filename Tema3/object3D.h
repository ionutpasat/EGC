#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object3D
{
    Mesh* CreateTrack(std::vector<glm::vec3> in, std::vector<glm::vec3> ext, glm::vec3 color);
}
