#version 330 core
in vec3 Normal, FragPos;
uniform vec3 viewPos;
uniform float time;
out vec4 FragColor;
void main() {
    // Accretion disk: glowing orange/red dengan animasi
    float dist = length(FragPos.xz);
    vec3 hotColor = vec3(1.0, 0.9, 0.5);  // Kuning putih (dekat)
    vec3 coolColor = vec3(1.0, 0.3, 0.0); // Merah (jauh)
    
    float t = clamp((dist - 1.0) / 4.0, 0.0, 1.0);
    vec3 objCol = mix(hotColor, coolColor, t);
    
    // Animasi glow
    float glow = 0.8 + 0.2 * sin(time * 2.0 + dist * 3.0);
    objCol *= glow;
    
    FragColor = vec4(objCol, 0.9);
}