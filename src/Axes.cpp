#include "Axes.h"
#include <glad/glad.h>
#include <vector>

Axes::Axes(float length) : m_length(length)
{
    std::vector<glm::vec3> vertices = {
        // X-axis
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(m_length, 0.0f, 0.0f),
        // Y-axis
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, m_length, 0.0f),
        // Z-axis
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, m_length)
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Axes::draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection)
{
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);

    glBindVertexArray(m_VAO);
    
    // Draw X-axis (red)
    shader.setVec3("lineColor", glm::vec3(1.0f, 0.0f, 0.0f));
    glDrawArrays(GL_LINES, 0, 2);

    // Draw Y-axis (green)
    shader.setVec3("lineColor", glm::vec3(0.0f, 1.0f, 0.0f));
    glDrawArrays(GL_LINES, 2, 2);

    // Draw Z-axis (blue)
    shader.setVec3("lineColor", glm::vec3(0.0f, 0.0f, 1.0f));
    glDrawArrays(GL_LINES, 4, 2);

    glBindVertexArray(0);
}
