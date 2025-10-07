#version 450 core

in vec3 v_TexCoords;

out vec4 color;

layout (binding = 0) uniform samplerCube u_Skybox;

uniform vec3 u_Albedo;
uniform int u_Mode;

void main()
{
    switch(u_Mode) 
    {
        case 0: color = vec4(u_Albedo, 1.0f); break;
        case 1: color = vec4(u_Albedo, 1.0f) * texture(u_Skybox, v_TexCoords); break;
    }
}