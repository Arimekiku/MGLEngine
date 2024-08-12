#include "mxpch.h"
#include "Transform.h"

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
        const glm::mat4 translation = glm::translate({1.0f}, Position);

        const glm::mat4 scaling = glm::scale({1.0f}, Scale);

        float rad = glm::radians(Rotation.x);
        const glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), rad, {1, 0, 0});

        rad = glm::radians(Rotation.y);
        const glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), rad, {0, 1, 0});

        rad = glm::radians(Rotation.z);
        const glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), rad, {0, 0, 1});

        const glm::mat4 model = translation * scaling * rotationX * rotationY * rotationZ;
        return model;
    }
}
