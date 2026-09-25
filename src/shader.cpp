#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vPath, const char* fPath) {
    std::ifstream vf(vPath), ff(fPath);
    std::stringstream vs, fs;
    vs << vf.rdbuf(); fs << ff.rdbuf();
    std::string vcs = vs.str(), fcs = fs.str();
    const char* vc = vcs.c_str(), *fc = fcs.c_str();

    unsigned int v = glCreateShader(GL_VERTEX_SHADER), f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(v, 1, &vc, NULL); glCompileShader(v);
    glShaderSource(f, 1, &fc, NULL); glCompileShader(f);

    ID = glCreateProgram();
    glAttachShader(ID, v); glAttachShader(ID, f); glLinkProgram(ID);
    glDeleteShader(v); glDeleteShader(f);
}

void Shader::setMat4(const std::string& n, const glm::mat4& m) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, n.c_str()), 1, GL_FALSE, &m[0][0]);
}

void Shader::setFloat(const std::string& n, float v) const {
    glUniform1f(glGetUniformLocation(ID, n.c_str()), v);
}

void Shader::setVec3(const std::string& n, const glm::vec3& v) const {
    glUniform3f(glGetUniformLocation(ID, n.c_str()), v.x, v.y, v.z);
}