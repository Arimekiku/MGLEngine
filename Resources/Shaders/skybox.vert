#version 450 core
out vec3 v_TexCoords;

const vec2[6] positions = vec2[](
    vec2(1.0, 1.0),
    vec2(-1.0, 1.0),
    vec2(-1.0, -1.0),
    vec2(-1.0, -1.0),
    vec2(1.0, -1.0),
    vec2(1.0, 1.0)
);

uniform mat4 u_camMatrix;

void main()
{
    vec2 vertexPos = positions[gl_VertexID];
    v_TexCoords = normalize(vec3(0, 0, 1) + vertexPos.x * vec3(1, 0, 0) + vertexPos.y * vec3(0, 1, 0));

    gl_Position = vec4(vertexPos, 1.0, 1.0);
} 