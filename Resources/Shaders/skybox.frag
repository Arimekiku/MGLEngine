#version 450 core
out vec4 color;

in vec3 v_TexCoords;

layout (binding = 0) uniform samplerCube u_Skybox;

void main()
{    
    color = texture(u_Skybox, v_TexCoords);
}