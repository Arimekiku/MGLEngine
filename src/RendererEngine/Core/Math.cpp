#include "Math.h"
#include "glm/ext/scalar_constants.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace RenderingEngine 
{
    bool Math::Decompose(glm::mat4 &transform, glm::vec3 &position, glm::vec3 &rotation, glm::vec3 &scale) 
    {
        auto epsilon = glm::epsilon<float>();

		glm::mat4 LocalMatrix = glm::mat4(transform);

		// Normalize the matrix.
		if (glm::epsilonEqual(LocalMatrix[3][3], 0.0f, epsilon))
			return false;

		// First, isolate perspective.  This is the messiest.
		if (glm::epsilonNotEqual(LocalMatrix[0][3], 0.0f, epsilon) ||
			glm::epsilonNotEqual(LocalMatrix[1][3], 0.0f, epsilon) ||
			glm::epsilonNotEqual(LocalMatrix[2][3], 0.0f, epsilon))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = 0.0f;
			LocalMatrix[3][3] = 1.0f;
		}

		// Next take care of translation (easy).
		position = glm::vec3(LocalMatrix[3]);
		LocalMatrix[3] = glm::vec4(0, 0, 0, LocalMatrix[3].w);

		glm::vec3 Row[3], Pdum3;

		// Now get scale and shear.
		for (glm::length_t i = 0; i < 3; ++i)
			for (glm::length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		scale.x = length(Row[0]);
		Row[0] = glm::detail::scale(Row[0], 1.0f);
		scale.y = length(Row[1]);
		Row[1] = glm::detail::scale(Row[1], 1.0f);
		scale.z = length(Row[2]);
		Row[2] = glm::detail::scale(Row[2], 1.0f);

		rotation.y = asin(-Row[0][2]);
		if (cos(rotation.y) != 0) 
        {
			rotation.x = atan2(Row[1][2], Row[2][2]);
			rotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else 
        {
			rotation.x = atan2(-Row[2][0], Row[1][1]);
			rotation.z = 0;
		}

		return true;
    }
}