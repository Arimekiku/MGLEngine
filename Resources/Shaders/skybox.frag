#version 450 core

in vec3 v_TexCoords;

layout (location = 0) out vec4 color;
layout (location = 1) out int index;

layout (binding = 0) uniform samplerCube u_Skybox;

uniform vec3 u_Albedo;
uniform int u_Mode;
uniform int u_ID;

void main()
{
    switch(u_Mode) 
    {
        case 0: color = vec4(u_Albedo, 1.0f); break;
        case 1: color = vec4(u_Albedo, 1.0f) * texture(u_Skybox, v_TexCoords); break;
    }

    index = u_ID;
}