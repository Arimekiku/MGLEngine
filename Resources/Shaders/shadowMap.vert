#version 450 core
layout (location = 0) in vec3 a_Position;

uniform mat4 u_lightViewProj;
uniform mat4 u_trsMatrix;

void main()
{
    gl_Position = u_lightViewProj * u_trsMatrix * vec4(a_Position, 1.0);
}