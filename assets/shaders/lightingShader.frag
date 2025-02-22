#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    // vec3 ambient;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

#define NUM_POINT_LIGHTS 4

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform DirectionalLight directionalLight;
uniform Material material;
uniform vec3 viewPos;

vec3 calculateSpotLight(SpotLight sl, vec3 normalDir, vec3 fragPos, vec3 viewDir);
vec3 calculateDirectionalLight(DirectionalLight dl, vec3 normalDir, vec3 viewDir);
vec3 calculatePointLight(PointLight pl, vec3 normalDir, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 normDir = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = calculateDirectionalLight(directionalLight, normDir, viewDir);
    for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
        result += calculatePointLight(pointLights[i], normDir, FragPos, viewDir);
    }
    result += calculateSpotLight(spotLight, normDir, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 calculateSpotLight(SpotLight sl, vec3 normalDir, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(sl.position - fragPos);
    // diffuse
    float diffuseStrength = max(dot(normalDir, lightDir), 0.0);
    vec3 diffuse = sl.diffuse * diffuseStrength * vec3(texture(material.diffuse, TexCoords));
    // specular
    vec3 reflectDir = reflect(-lightDir, normalDir);
    float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = sl.specular * specularStrength * vec3(texture(material.specular, TexCoords));
    // ambient
    vec3 ambient = sl.ambient * vec3(texture(material.diffuse, TexCoords));
    // angle
    float theta = dot(lightDir, normalize(-sl.direction));
    float epsilon   = sl.cutOff - sl.outerCutOff;
    float intensity = clamp((theta - sl.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;
    // attenuation
    float distance = length(sl.position - fragPos);
    float attenuation = 1.0 / (sl.constant + sl.linear * distance + 
  			     sl.quadratic * (distance * distance)); 
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return(ambient + diffuse + specular);
}

vec3 calculateDirectionalLight(DirectionalLight dl, vec3 normalDir, vec3 viewDir) {
    vec3 lightDir = normalize(-dl.direction);
    // diffuse
    float diffuseStrength = max(dot(normalDir, lightDir), 0.0);
    vec3 diffuse = dl.diffuse * diffuseStrength * vec3(texture(material.diffuse, TexCoords));
    // specular
    vec3 reflectDir = reflect(-lightDir, normalDir);
    float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dl.specular * specularStrength * vec3(texture(material.specular, TexCoords));
    // ambient
    vec3 ambient = dl.ambient * vec3(texture(material.diffuse, TexCoords));

    return(ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight pl, vec3 normalDir, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(pl.position - fragPos);
    // diffuse
    float diffuseStrength = max(dot(normalDir, lightDir), 0.0);
    vec3 diffuse = pl.diffuse * diffuseStrength * vec3(texture(material.diffuse, TexCoords));
    // specular
    vec3 reflectDir = reflect(-lightDir, normalDir);
    float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = pl.specular * specularStrength * vec3(texture(material.specular, TexCoords));
    // ambient
    vec3 ambient = pl.ambient * vec3(texture(material.diffuse, TexCoords));
    // attenuation
    float distance = length(pl.position - fragPos);
    float attenuation = 1.0 / (pl.constant + pl.linear * distance + 
  			     pl.quadratic * (distance * distance)); 
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return(ambient + diffuse + specular);
}