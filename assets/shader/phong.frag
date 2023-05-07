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

struct DirectionLight
{
    float intensity;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
};
uniform DirectionLight _light;

void main()
{
    vec3 normalDir = normalize(fragNor);
    vec3 viewDir = normalize(-fragPos);
    vec3 lightDir = normalize (_light.direction);
    vec3 reflectDir = reflect(-lightDir, normalDir);

    float NdotL = dot(normalDir, lightDir);

    vec3 diffuse = _material.diffuse * max(NdotL, 0.0);
    vec3 specular = (NdotL < 0.0) ? vec3(0.0) : _material.specular * pow(max(dot(viewDir, reflectDir), 0.0), _material.shininess);
    vec3 ambient = _material.albedo * _light.ambient;

    gl_FragColor = vec4((ambient + diffuse + specular) * _light.intensity, 1.0);
}
