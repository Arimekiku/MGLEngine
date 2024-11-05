#version 450 core
out vec4 color;

in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_CamPos;
in vec4 v_FragPosLight;

layout (binding = 0) uniform sampler2D u_Texture;
layout (binding = 1) uniform sampler2D u_DepthMap;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;

uniform vec3 u_Albedo;

vec4 areaLight(); 
vec4 directLight();

void main()
{
    color = directLight() + areaLight();
}

vec4 areaLight() 
{
    //Intensity
    float d = length(u_LightPos - v_Position);
    float a = 3.0f;
    float b = 0.7f;
    float i = 1.0f / (a * d * d + b * d + 1.0f);

    //Ambient
    float ambient = 0.2f;

    //Diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_Position);
    float diffuse = max(dot(lightDir, normal), 0.0f);

    //Specular
    float specular = 0.0f;
    if (diffuse != 0.0f) 
    {
        vec3 viewDir = normalize(v_CamPos - v_Position);
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 halfwayVec = normalize(viewDir + lightDir);
        specular = pow(max(dot(normal, halfwayVec), 0.0f), 16.0f);
    }

    vec3 ambientColor = u_LightColor * ambient * i;
    vec3 diffuseColor = u_LightColor * diffuse * i;
    vec3 specularColor = u_LightColor * specular * i;

    vec3 lighting = u_Albedo * ambientColor + (diffuseColor * specularColor);
    return texture(u_Texture, v_TexCoord) * vec4(lighting, 1.0f);
}

vec4 directLight() 
{
    vec3 normal = normalize(v_Normal);
    
    // Ambient
    vec3 ambient = 0.2f * u_LightColor;

    // Diffuse
    vec3 lightDir = normalize(u_LightPos - v_Position);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // Specular
    vec3 viewDir = normalize(v_CamPos - v_Position);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specAmount = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    vec3 specular = specAmount * u_LightColor;    

    // Shadow
    vec3 projCoords = v_FragPosLight.xyz / v_FragPosLight.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(u_DepthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    float biasAmount = 0.0005f;
    float shadow = currentDepth > closestDepth + biasAmount ? 1.0 : 0.0;     

    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * texture(u_Texture, v_TexCoord).rgb;    
    return vec4(lighting, 1.0);
}