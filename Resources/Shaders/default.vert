#version 450 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out vec3 v_Position;
out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_CamPos;
out vec4 v_FragPosLight;

uniform mat4 u_camMatrix;
uniform mat4 u_trsMatrix;
uniform mat4 u_lightViewProj;

void main()
{
    v_Position = vec3(u_trsMatrix * vec4(a_Position, 1.0f));
    v_TexCoord = a_TexCoord;
    v_Normal = transpose(inverse(mat3(u_trsMatrix))) * a_Normal;
    v_CamPos = vec3(u_camMatrix[0][3], u_camMatrix[1][3], u_camMatrix[2][3]);
    v_FragPosLight = u_lightViewProj * vec4(v_Position, 1.0f);

    gl_Position = u_camMatrix * vec4(v_Position, 1.0f);
}
