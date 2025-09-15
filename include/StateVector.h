#pragma once

#include <glm/glm.hpp>
#include "Shader.h"

class StateVector {
public:
    StateVector();
    void update(float theta, float phi);
    void draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection);
    void storePreviousState();
    void hidePrevious();

private:
    unsigned int m_VAO, m_VBO;
    unsigned int m_prev_VAO, m_prev_VBO;
    glm::vec3 m_currentVector;
    glm::vec3 m_previousVector;
    bool m_drawPrevious;
};
