#include "StateVector.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

StateVector::StateVector()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Initial data (origin to (0,0,0))
    glm::vec3 vertices[2] = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void StateVector::update(float theta, float phi)
{
    // Convert degrees to radians for calculation
    float radTheta = glm::radians(theta);
    float radPhi = glm::radians(phi);

    // Calculate Cartesian coordinates from spherical coordinates
    // Assuming unit radius for the Bloch sphere
    float x = sin(radTheta) * cos(radPhi);
    float y = cos(radTheta);
    float z = sin(radTheta) * sin(radPhi);

    m_currentVector = glm::vec3(x, y, z);

    // Update VBO data
    glm::vec3 vertices[2] = { glm::vec3(0.0f, 0.0f, 0.0f), m_currentVector };
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void StateVector::draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection) const
{
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);

    // Set color for the state vector (e.g., yellow)
    shader.setVec3("lineColor", glm::vec3(1.0f, 1.0f, 0.0f)); 

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_LINES, 0, 2);

    // Draw a point at the tip of the vector
    glPointSize(10.0f); // Set point size
    glDrawArrays(GL_POINTS, 1, 1); // Draw only the second vertex (the tip)

    glBindVertexArray(0);
}
