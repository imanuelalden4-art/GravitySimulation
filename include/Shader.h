#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    unsigned int ID;
    Shader(const char* vPath, const char* fPath);
    void use() { glUseProgram(ID); }
    void setMat4(const std::string& n, const glm::mat4& m) const;
    void setFloat(const std::string& n, float v) const;
    void setVec3(const std::string& n, const glm::vec3& v) const;
};