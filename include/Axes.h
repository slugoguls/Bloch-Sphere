#pragma once

#include "Shader.h"
#include <glm/glm.hpp>

class Axes {
public:
    Axes(float length);
    void draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection, float thickness);

private:
    unsigned int m_VAO, m_VBO;
    float m_length;
};
