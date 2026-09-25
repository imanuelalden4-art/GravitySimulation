#version 330 core
in vec3 Normal, FragPos;
uniform vec3 viewPos;
out vec4 FragColor;
void main() {
    // Event horizon: hitam pekat dengan sedikit glow di tepi
    vec3 objCol = vec3(0.0, 0.0, 0.0);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Fresnel effect untuk edge glow
    float fresnel = pow(1.0 - max(dot(norm, viewDir), 0.0), 3.0);
    vec3 edgeGlow = vec3(0.3, 0.1, 0.5) * fresnel;
    
    FragColor = vec4(objCol + edgeGlow, 1.0);
}