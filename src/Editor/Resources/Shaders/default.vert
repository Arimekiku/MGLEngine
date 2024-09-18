#version 410 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_Normal;

out vec3 v_Position;
out vec4 v_Color;
out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_CamPos;

uniform mat4 u_camMatrix;
uniform mat4 u_trsMatrix;

void main()
{
    v_Position = vec3(u_trsMatrix * vec4(a_Position, 1.0f));
    v_Color = vec4(a_Color, 1.0f);
    v_TexCoord = a_TexCoord;
    v_Normal = a_Normal;
    v_CamPos = vec3(u_camMatrix[0][3], u_camMatrix[1][3], u_camMatrix[2][3]);

    gl_Position = u_camMatrix * u_trsMatrix * vec4(a_Position, 1.0);
}