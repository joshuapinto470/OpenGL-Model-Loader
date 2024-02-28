#version 330 core

out vec4 FragColor;

in vec3 v_normal;
in vec2 TexCoord;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

// uniform sampler2D texture1;
// uniform sampler2D texture2;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

void main()
{
    float ambientStrength = 0.04;
    float specularStrength = 0.5;

    vec3 ambient = ambientStrength * lightColor;
    vec4 objectColor = texture(texture_diffuse1, TexCoord);

    // vec3 norm = normalize(v_normal);
    vec3 norm = texture(texture_normal1, TexCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = vec3(texture(texture_specular1, TexCoord)) * spec * lightColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec4 result = vec4((ambient + diffuse + specular), 1.0) * objectColor;
    FragColor = result;
    // FragColor = vec4(norm, 1.0);
    // FragColor = texture(texture_diffuse1, TexCoord) * vec4(ambient, 1.0);
}


// FragColor = mix(texture(texture1, TexCoord),
//                 texture(texture2,  TexCoord), 0.2);
// FragColor = vec4(v_normal, 1.0);