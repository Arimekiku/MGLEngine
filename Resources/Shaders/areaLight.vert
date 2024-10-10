#version 410 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_camMatrix;
uniform mat4 u_trsMatrix;

void main()
{
    gl_Position = u_camMatrix * u_trsMatrix * vec4(a_Position, 1.0);
}