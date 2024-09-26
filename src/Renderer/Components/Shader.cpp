#include "mxpch.h"

#include "Shader.h"

#include <filesystem>
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace RenderingEngine
{
    std::string Shader::GetContent(const std::string& path)
    {
        std::string result;
        if (auto in = std::ifstream(path, std::ios::in | std::ios::binary))
        {
            in.seekg(0, std::ios::end);
            if (const auto size = in.tellg(); size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);

                return result;
            }

            LOG_CORE_ASSERT(false, "Could not read from file '{0}'", path)
        }

        LOG_CORE_ERROR("Could not open file '{0}'", path);
    }

    Shader::Shader(const std::string& vertPath, const std::string& fragPath)
    {
        const uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);

        auto content = GetContent(vertPath);
        const char* source = content.c_str();

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
            LOG_CORE_ASSERT(false, "Can't compile vertex shader!");
        }

        const uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        content = GetContent(fragPath);
        source = content.c_str();

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
            LOG_CORE_ASSERT(false, "Can't compile fragment shader!");
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

    void Shader::BindUniformMat3(const std::string& name, const glm::mat3& mat) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }

    void Shader::BindUniformFloat4(const std::string& name, const glm::f32vec4& vec) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
    }

    void Shader::BindUniformFloat3(const std::string& name, const glm::f32vec3& vec) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, vec.x, vec.y, vec.z);
    }

    void Shader::BindUniformFloat2(const std::string& name, const glm::f32vec2& vec) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, vec.x, vec.y);
    }

    void Shader::BindUniformFloat1(const std::string& name, const float& val) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, val);
    }

    void Shader::BindUniformInt4(const std::string& name, const glm::i32vec4& vec) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4i(location, vec.x, vec.y, vec.z, vec.w);
    }

    void Shader::BindUniformInt3(const std::string& name, const glm::i32vec3& vec) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3i(location, vec.x, vec.y, vec.z);
    }

    void Shader::BindUniformInt2(const std::string& name, const glm::i32vec2& vec) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2i(location, vec.x, vec.y);
    }

    void Shader::BindUniformInt1(const std::string& name, const int& val) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, val);
    }
}
