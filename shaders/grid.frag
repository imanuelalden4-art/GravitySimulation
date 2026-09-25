#version 330 core
in vec3 Pos;
out vec4 FragColor;
void main() {
    float dist = length(Pos.xy);
    vec3 color = mix(vec3(0.2, 0.6, 1.0), vec3(0.0, 0.1, 0.4), dist / 25.0);
    FragColor = vec4(color, 0.85);
}