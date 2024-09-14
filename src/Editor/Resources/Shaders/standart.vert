#version 410 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;

out vec3 v_Position;
out vec4 v_Color;
out vec2 v_TexCoord;

uniform mat4 u_camMatrix;
uniform mat4 u_trsMatrix;

void main()
{
    v_Position = a_Position;
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    
    gl_Position = u_camMatrix * u_trsMatrix * vec4(a_Position, 1.0);
}