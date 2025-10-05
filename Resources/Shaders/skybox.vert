#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out vec3 v_TexCoords;

uniform mat4 u_camMatrix;

void main()
{
    v_TexCoords = a_Position;

    gl_Position = u_camMatrix * vec4(a_Position, 1.0);
} 