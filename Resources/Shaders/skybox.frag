#version 450 core

in vec3 v_TexCoords;

out vec4 color;

layout (binding = 0) uniform samplerCube u_Skybox;

void main()
{
    color = texture(u_Skybox, v_TexCoords);
}