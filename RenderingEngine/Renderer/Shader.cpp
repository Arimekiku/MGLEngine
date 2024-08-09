#include "mxpch.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace RenderingEngine
{
    Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        const uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);

        const char* source = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &source, nullptr);
        glCompileShader(vertexShader);

        int32_t isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            int32_t maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<char> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
            glDeleteShader(vertexShader);

            LOG_CORE_WARN("Shader message: {0}", infoLog.data());
            LOG_CORE_ASSERT(false, "Can't compile vertex shader!")
        }

        const uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        source = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &source, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            int32_t maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<char> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
            glDeleteShader(fragmentShader);
            glDeleteShader(vertexShader);

            LOG_CORE_WARN("Shader message: {0}", infoLog.data());
            LOG_CORE_ASSERT(false, "Can't compile fragment shader!")
        }

        m_RendererID = glCreateProgram();
        glAttachShader(m_RendererID, vertexShader);
        glAttachShader(m_RendererID, fragmentShader);
        glLinkProgram(m_RendererID);

        int32_t isLinked = 0;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            int32_t maxLength = 0;
            glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<char> infoLog(maxLength);
            glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);
            glDeleteProgram(m_RendererID);
            glDeleteShader(fragmentShader);
            glDeleteShader(vertexShader);

            LOG_CORE_WARN("Program message: {0}", infoLog.data());
            LOG_CORE_ASSERT(false, "Can't link shaders!")
        }

        glDetachShader(m_RendererID, vertexShader);
        glDetachShader(m_RendererID, fragmentShader);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_RendererID);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void Shader::Unbind()
    {
        glUseProgram(0);
    }

    void Shader::BindUniformMat4(const std::string& name, const glm::mat4& mat) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
}
