#version 330 core
in vec3 Normal, FragPos;
uniform vec3 viewPos;
out vec4 FragColor;
void main() {
    vec3 objCol = vec3(0.9, 0.95, 1.0);
    vec3 lightCol = vec3(1.0, 0.9, 0.8);
    vec3 lightPos = vec3(15.0, 15.0, 15.0);
    
    vec3 ambient = 0.3 * lightCol;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 diffuse = max(dot(norm, lightDir), 0.0) * lightCol;
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 specular = 0.6 * pow(max(dot(viewDir, reflectDir), 0.0), 32) * lightCol;
    
    vec3 glow = vec3(0.6, 0.3, 0.1) * (1.0 - abs(norm.z)); // Edge glow
    
    FragColor = vec4((ambient + diffuse + specular) * objCol + glow, 1.0);
}