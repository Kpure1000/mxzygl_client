varying vec3 fragPos;
varying vec3 fragNor;

struct Material
{
    vec3 albedo;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material _material;

struct PointLight
{
    float intensity;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;

    float _constant;
    float _linear;
    float _quadratic;
};
uniform PointLight _light_1;
uniform PointLight _light_2;
uniform PointLight _light_3;

vec3 shading(in PointLight light)
{
    vec3 normalDir = normalize(fragNor);
    vec3 viewDir = normalize(-fragPos);
    vec3 lightDir = normalize(light.position - fragPos);

    float NdotL = dot(normalDir, lightDir);

    if(NdotL < 0.0) {
        normalDir = -normalDir;
        NdotL = dot(normalDir, lightDir);
    }

    vec3 reflectDir = reflect(-lightDir, normalDir);


    vec3 diffuse = _material.diffuse * light.diffuse * max(NdotL, 0.0);
    vec3 specular = _material.specular * light.specular * pow(max(dot(viewDir, reflectDir), 0.0), _material.shininess);
    vec3 ambient = _material.albedo * light.ambient;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light._constant + light._linear * distance + light._quadratic * (distance * distance));

    return (ambient + diffuse + specular) * light.intensity * attenuation;
}

void main()
{
    vec3 color = shading(_light_1) + shading(_light_2) + shading(_light_3);
    color = min(vec3(1.0), color);
    gl_FragColor = vec4(color, 1.0);
}
