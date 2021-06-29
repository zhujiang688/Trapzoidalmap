#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 Color;
uniform vec3 viewPos;

void main()
{
    float ambient = 0.1f;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specularStrength = 0.5f;
    float specular = pow(max(dot(viewDir, reflectDir), 0.0f), 32) * specularStrength;
    vec3 result = (ambient + diff+specular) * lightColor * Color;
    FragColor = vec4(result,1.0f);
}
