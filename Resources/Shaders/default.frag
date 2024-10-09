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
uniform float u_Roughness;
uniform float u_Metallic;
uniform float u_AO;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main()
{
    //calculate maps
    vec3 albedo = pow(texture(u_Texture, v_TexCoord).rgb * u_Albedo, vec3(2.2));

    vec3 N = normalize(v_Normal);
    vec3 V = normalize(v_CamPos - v_Position);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, u_Albedo, u_Metallic);

    // reflectance equation
    vec3 L = normalize(u_LightPos - v_Position);
    vec3 H = normalize(V + L);

    float distance = length(u_LightPos - v_Position);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = u_LightColor * attenuation;

    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, u_Roughness);
    float G = GeometrySmith(N, V, L, u_Roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - u_Metallic;

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);
    vec3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;

    //shadow
    float shadow = 0.0f;
    vec3 lightCoords = v_FragPosLight.xyz / v_FragPosLight.w;
    if (lightCoords.z <= 1.0f) 
    {
        lightCoords = (lightCoords + 1.0f) / 2.0f;
        
        float closestDepth = texture(u_ShadowMap, lightCoords.xy).r;
        float currentDepth = lightCoords.z;

        if (currentDepth > closestDepth) 
            shadow = 1.0f;
    }

    //ambient
    vec3 ambient = vec3(0.03) * albedo * u_AO;
    vec3 result = ambient + Lo * (1 - shadow);

    result = result / (result + vec3(1.0));
    result = pow(result, vec3(1.0/2.2));

    color = vec4(result, 1.0);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}