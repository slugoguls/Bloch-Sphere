#pragma once

#include <glm/glm.hpp>
#include "Shader.h"

class StateVector {
public:
    StateVector();
    void update(float theta, float phi);
    void draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection) const;

private:
    unsigned int m_VAO, m_VBO;
    glm::vec3 m_currentVector;
};
