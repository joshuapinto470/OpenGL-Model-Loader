#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_height1;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 v_normal;
in vec2 TexCoord;
in vec3 FragPos;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main()
{
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoord).rgb;

    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoord).rgb;  

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec3 specular = light.specular * (spec * texture(material.texture_specular1, TexCoord).rgb); 

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}