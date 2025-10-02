#include "Math.h"

#include <glm/gtx/matrix_decompose.hpp>

namespace RenderingEngine 
{
    bool Math::Decompose(const glm::mat4& transform, glm::vec3 &position, glm::vec3 &rotation, glm::vec3 &scale) 
    {
		glm::quat quat;
		glm::vec3 skew;
		glm::vec4 perspective;
		
		glm::decompose(transform, scale, quat, position, skew, perspective);

		rotation = glm::eulerAngles(quat);
        return true;
    }
}