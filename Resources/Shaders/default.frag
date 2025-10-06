#version 460 core

layout (location = 0) out vec4 color;
layout (location = 1) out int index;

in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_CamPos;
in vec4 v_FragPosLight;

layout (binding = 0) uniform sampler2D u_Texture;
layout (binding = 1) uniform sampler2D u_DepthMap;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform int u_ID;

uniform vec3 u_Albedo;

float ShadowCalculation(vec4 fragPosLightSpace) 
{
    // Transform from clip space to [0,1] texture coordinates
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // Depth of current fragment from light's view
    float currentDepth = projCoords.z;

    // Get closest depth stored in shadow map
    float closestDepth = texture(u_DepthMap, projCoords.xy).r;

    // Basic comparison
    vec3 lightDir = normalize(u_LightPos - v_Position);
    vec3 normal = normalize(v_Normal);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
    float shadow = currentDepth > closestDepth + bias ? 1.0 : 0.0;

    // If outside the light’s frustum, ignore
    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{
    vec3 albedo = texture(u_Texture, v_TexCoord).rgb;
    vec3 normal = normalize(v_Normal);

    // Simple directional light
    vec3 lightDir = normalize(u_LightPos - v_Position);
    vec3 lightDirNorm = normalize(lightDir);
    float diff = max(dot(normal, lightDirNorm), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // Compute shadow
    float shadow = ShadowCalculation(v_FragPosLight);

    // Combine lighting
    vec3 lighting = (1.0 - shadow) * diffuse * albedo;

    color = vec4(lighting, 1.0);
    index = u_ID;
}