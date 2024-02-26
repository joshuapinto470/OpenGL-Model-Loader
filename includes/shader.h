#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader {
   public:
    Shader(const char *, const char *);
    void use();
    void setInt(const std::string &, int) const;
    void setBool(const std::string &, bool) const;
    void setFloat(const std::string &, float) const;
    void setVec2(const std::string &, const glm::vec2 &) const;
    void setVec2(const std::string &, float, float) const;
    void setVec3(const std::string &, const glm::vec3 &) const;
    void setVec3(const std::string &, float, float, float) const;
    void setVec4(const std::string &, float, float, float, float) const;
    void setVec4(const std::string &, const glm::vec4 &) const;
    void setMat4(const std::string &, const glm::mat4 &) const;
    void setMat2(const std::string &, const glm::mat2 &) const;
    void setMat3(const std::string &, const glm::mat3 &) const;
    unsigned int ID;
};