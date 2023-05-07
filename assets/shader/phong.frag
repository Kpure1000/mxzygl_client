varying vec3 fragPos;
varying vec3 fragNor;

struct Material
{
    vec3 albedo;
    vec3 diffuse;
    vec3 specular;
    int shininess;
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
    if(dot(normalDir, viewDir) < 0.0)
        normalDir = -normalDir;
    vec3 lightDir = normalize (_light.direction);
    vec3 reflectDir = reflect(-lightDir, normalDir);

    vec4 diffuse = vec4(_material.diffuse * max(dot(normalDir, lightDir), 0.0), 1.0);
    vec4 specular = vec4(_material.specular * pow(max(dot(viewDir, reflectDir), 0.0), float(_material.shininess)), 1.0);
    vec4 ambient = vec4(_material.albedo, 1.0);

    gl_FragColor = (ambient + diffuse + specular) * _light.intensity;
}
