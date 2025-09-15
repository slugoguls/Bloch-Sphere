#include "StateVector.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

StateVector::StateVector() : m_drawPrevious(false)
{
    // Current state vector setup
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glm::vec3 vertices[2] = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Previous state vector setup
    glGenVertexArrays(1, &m_prev_VAO);
    glGenBuffers(1, &m_prev_VBO);
    glBindVertexArray(m_prev_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_prev_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void StateVector::update(float theta, float phi)
{
    float radTheta = glm::radians(theta);
    float radPhi = glm::radians(phi);

    float x = sin(radTheta) * cos(radPhi);
    float y = cos(radTheta);
    float z = sin(radTheta) * sin(radPhi);

    m_currentVector = glm::vec3(x, y, z);

    glm::vec3 vertices[2] = { glm::vec3(0.0f, 0.0f, 0.0f), m_currentVector };
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void StateVector::draw(const Shader& shader, const glm::mat4& view, const glm::mat4& projection)
{
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);

    // Draw previous state vector if flag is set
    if (m_drawPrevious) {
        shader.setVec3("lineColor", glm::vec3(1.0f, 1.0f, 0.0f) * 0.5f); // Dimmed color
        glBindVertexArray(m_prev_VAO);
        glDrawArrays(GL_LINES, 0, 2);
        glPointSize(5.0f);
        glDrawArrays(GL_POINTS, 1, 1);
    }

    // Draw current state vector
    shader.setVec3("lineColor", glm::vec3(1.0f, 1.0f, 0.0f));
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glPointSize(10.0f);
    glDrawArrays(GL_POINTS, 1, 1);

    glBindVertexArray(0);
}

void StateVector::storePreviousState()
{
    m_previousVector = m_currentVector;
    m_drawPrevious = true;

    glm::vec3 vertices[2] = { glm::vec3(0.0f, 0.0f, 0.0f), m_previousVector };
    glBindBuffer(GL_ARRAY_BUFFER, m_prev_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void StateVector::hidePrevious()
{
    m_drawPrevious = false;
}
