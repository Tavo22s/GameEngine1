#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec2 TexCoords;
    vec3 FragPos;
    vec3 Normal;
} fs_in;

struct Material
{
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D aoMap;

    vec3 albedo;
    float metallic;
    float roughness;
    float ao;

    bool aldedoMapEnable;
    bool normalMapEnable;
    bool metallicMapEnable;
    bool roughnessMapEnable;
    bool aoMapEnable;
};

struct Light
{
    vec3 position;
    vec3 direction;
    vec3 color;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;

    int type;
};

uniform Light lights[100];
uniform Material material;
uniform vec3 camPos;

uniform int num_lights;

const float PI = 3.14159265359;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(material.normalMap, fs_in.TexCoords).xyz * 2.0 - 1.0;
    
    vec3 Q1 = dFdx(fs_in.FragPos);
    vec3 Q2 = dFdy(fs_in.FragPos);
    vec2 st1 = dFdx(fs_in.TexCoords);
    vec2 st2 = dFdy(fs_in.TexCoords);

    vec3 N = normalize(fs_in.Normal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), .0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), .0);
    float NdotL = max(dot(N, L), .0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 CalcDirLight(Light dirlight, vec3 N, vec3 V, vec3 albedo, vec3 F0, float roughness, float metallic)
{
    vec3 L = normalize(-dirlight.direction);
    vec3 H = normalize(V + L);
    vec3 radiance = dirlight.color;

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), .0), F0);

    vec3 nominator = NDF * G * F;
    float denominator = 4 * max(dot(N, V), .0f) * max(dot(N, L), .0f) + .0001f;
    vec3 specular = nominator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    float NdotL = max(dot(N, L), .0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 CalcPointLight(Light pointLight, vec3 N, vec3 V, vec3 albedo, vec3 F0, float roughness, float metallic)
{
    vec3 L = normalize(pointLight.position - fs_in.FragPos);
    vec3 H = normalize(V + L);
    float distance = length(pointLight.position - fs_in.FragPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = pointLight.color * attenuation;

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), .0), F0);

    vec3 nominator = NDF * G * F;
    float denominator = 4 * max(dot(N, V), .0f) * max(dot(N, L), .0f) + .0001f;
    vec3 specular = nominator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    float NdotL = max(dot(N, L), .0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 CalcSpotLight(Light spotLight, vec3 N, vec3 V, vec3 albedo, vec3 F0, float roughness, float metallic)
{
    vec3 L = normalize(spotLight.position - fs_in.FragPos);
    vec3 H = normalize(V + L);
    float distance = length(spotLight.position - fs_in.FragPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = spotLight.color * attenuation;

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), .0), F0);

    vec3 nominator = NDF * G * F;
    float denominator = 4 * max(dot(N, V), .0f) * max(dot(N, L), .0f) + .0001f;
    vec3 specular = nominator / denominator;

    float theta = dot(L, normalize(-spotLight.direction));
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, .0f, 1.0f);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    float NdotL = max(dot(N, L), .0);

    return (kD * albedo * intensity / PI + specular * intensity) * radiance * NdotL;
}

void main()
{
    vec3 albedo = pow(texture(material.albedoMap, fs_in.TexCoords).rgb, vec3(2.2));
    float metallic = texture(material.metallicMap, fs_in.TexCoords).r;
    float roughness = texture(material.roughnessMap, fs_in.TexCoords).r;
    float ao = texture(material.aoMap, fs_in.TexCoords).r;

    vec3 N = getNormalFromMap();

    if(!material.aldedoMapEnable)
        albedo = material.albedo;
    if(!material.normalMapEnable)
        N = normalize(fs_in.Normal);
    if(!material.metallicMapEnable)
        metallic = material.metallic;
    if(!material.roughnessMapEnable)
        roughness = material.roughness;
    if(!material.aoMapEnable)
        ao = material.ao;

    vec3 V = normalize(camPos - fs_in.FragPos);

    vec3 F0 = vec3(0.4);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(.0);
    float amconstant = 1.0f;
    for(int i=0; i < num_lights; i++)
    {
        if(lights[i].type == 0)
            Lo += CalcDirLight(lights[i], N, V, albedo, F0, roughness, metallic);
        else if(lights[i].type == 1)
            Lo += CalcPointLight(lights[i], N, V, albedo, F0, roughness, metallic);
        else if(lights[i].type == 2)
            Lo += CalcSpotLight(lights[i], N, V, albedo, F0, roughness, metallic);
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
}