#include "Transform.h"
#include "glm/trigonometric.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace RenderingEngine
{
    Transform::Transform(const glm::vec3& initPos, const glm::vec3& initRot, const glm::vec3& initScale)
    {
        Position = initPos;
        Rotation = initRot;
        Scale = initScale;
    }

    glm::mat4 Transform::GetTRSMatrix() const
    {
        const glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
        const glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(Rotation)));
        const glm::mat4 scaling = glm::scale(glm::mat4(1.0f), Scale);

        return translation * rotation * scaling;
    }
}
