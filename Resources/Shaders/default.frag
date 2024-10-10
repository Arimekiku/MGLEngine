#version 410 core
out vec4 color;

in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_CamPos;
in vec4 v_FragPosLight;

uniform sampler2D u_Texture;
uniform sampler2D u_DepthMap;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;

uniform vec3 u_Albedo;

void main()
{
    //Ambient
    float ambient = 0.2f;

    //Diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_Position);
    float diffuse = max(dot(normal, lightDir), 0.0f);

    //Specular
    float specular = 0.0f;
    if (diffuse != 0.0f) 
    {
        float specularLight = 0.5f;
        vec3 viewDir = normalize(v_CamPos - v_Position);
        vec3 reflectDir = reflect(lightDir, normal);
        vec3 halfwayVec = normalize(viewDir + lightDir);
        float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
        specular = specAmount * specularLight;
    }

    //Shadow
    float shadow = 0.0f;
    vec3 lightCoords = v_FragPosLight.xyz / v_FragPosLight.w;
    if (lightCoords.z <= 1.0f) 
    {
        lightCoords = (lightCoords + 1.0f) / 2.0f;

        float closestDepth = texture(u_DepthMap, lightCoords.xy).r;
        float currentDepth = lightCoords.z;

        float bias = 0.005f;
        if (currentDepth > closestDepth + bias)
            shadow = 1.0f;
    }

    float lightAmount = ambient + (diffuse + specular) * (1.0f - shadow);

    vec3 resultColor = u_Albedo * u_LightColor;
    color = texture(u_Texture, v_TexCoord) * vec4(resultColor, 1.0f) * lightAmount;
}