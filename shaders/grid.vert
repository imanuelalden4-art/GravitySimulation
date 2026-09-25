#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model, view, projection;
out vec3 Pos;
void main() {
    Pos = aPos;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}