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

vec4 areaLight(); 
vec4 directLight();

void main()
{
    vec3 texColor = texture(u_Texture, v_TexCoord).rgb;
    vec3 normal = normalize(v_Normal);
    vec3 lightColor = vec3(1.0);
    // ambient
    vec3 ambient = 0.15 * lightColor;
    // diffuse
    vec3 lightDir = normalize(u_LightPos - v_FragPosLight.xyz);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(v_CamPos - v_FragPosLight.xyz);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    vec3 projCoords = v_FragPosLight.xyz / v_FragPosLight.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(u_DepthMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;     

    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * texColor;    
    
    color = vec4(lighting, 1.0);
    //color = areaLight();
}

vec4 areaLight() 
{
    //Intensity
    //float d = length(u_LightPos - v_Position);
    //float a = 3.0f;
    //float b = 0.7f;
    //float i = 1.0f / (a * d * d + b * d + 1.0f);

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

    //Shadow
    float shadow = 0.0f;
    vec3 lightCoords = v_FragPosLight.xyz / v_FragPosLight.w;
    lightCoords = lightCoords * 0.5f + 0.5f;

    float closestDepth = texture(u_DepthMap, lightCoords.xy).r;
    float currentDepth = lightCoords.z;

    if (currentDepth > closestDepth)
        shadow = 1.0f;

    vec3 ambientColor = u_LightColor * ambient;
    vec3 diffuseColor = u_LightColor * diffuse;
    vec3 specularColor = u_LightColor * specular;

    vec3 lighting = u_Albedo * (ambientColor + (diffuseColor * specularColor) * (1.0f - shadow));
    return texture(u_Texture, v_TexCoord) * vec4(lighting, 1.0f);
}

vec4 directLight() 
{
    //Ambient
    float ambient = 0.2f;

    //Diffuse
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(vec3(0.0f, 0.0f, 1.0f));
    float diffuse = max(dot(normal, lightDir), 0.0f);

    //Specular
    float specular = 0.0f;
    if (diffuse != 0.0f) 
    {
        vec3 viewDir = normalize(v_CamPos - v_Position);
        vec3 reflectDir = reflect(lightDir, normal);
        vec3 halfwayVec = normalize(viewDir + lightDir);
        specular = pow(max(dot(normal, halfwayVec), 0.0f), 16);
    }

    //Shadow
    float shadow = 0.0f;
    vec3 lightCoords = v_FragPosLight.xyz / v_FragPosLight.w;
    if (lightCoords.z <= 1.0f) 
    {
        lightCoords = lightCoords * 0.5f + 0.5f;

        float closestDepth = texture(u_DepthMap, lightCoords.xy).r;
        float currentDepth = lightCoords.z;

        if (currentDepth > closestDepth)
            shadow = 1.0f;
    }

    vec3 ambientColor = u_LightColor * ambient;
    vec3 diffuseColor = u_LightColor * diffuse;
    vec3 specularColor = u_LightColor * specular;

    vec3 lighting = u_Albedo * (ambientColor + (diffuseColor * specularColor) * (1.0f - shadow));
    return texture(u_Texture, v_TexCoord) * vec4(lighting, 1.0f);
}