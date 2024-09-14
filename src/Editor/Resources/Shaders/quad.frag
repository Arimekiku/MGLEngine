#version 410 core

out vec4 color;

in vec3 v_Position;
in vec4 v_Color;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_CamPos;

uniform sampler2D u_Texture;
uniform vec3 u_LightPos;
uniform vec4 u_LightColor;

void main()
{
    //ambient
    float ambient = 0.2f;

    //diffuse
    vec3 normalized = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_Position);
    float diffuse = max(dot(v_Normal, lightDir), 0.0f);

    //specular
    float specularValue = 0.5f;
    vec3 viewDir = normalize(v_CamPos - v_Position);
    vec3 reflectDir = reflect(-lightDir, normalized);
    float specAmount = pow(max(dot(viewDir, reflectDir), 0.0f), 8);
    float specular = specAmount * specularValue;

    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor * v_Color * u_LightColor * (diffuse + ambient + specular);
    color = vec4(color.x, color.y, color.z, 1f);
}