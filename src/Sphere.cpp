#include "Sphere.h"
#include <glad/glad.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Sphere::Sphere(float radius, unsigned int rings, unsigned int sectors)
{
    generateVertices(radius, rings, sectors);
    setupMesh();
}

void Sphere::draw() const
{
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Sphere::generateVertices(float radius, unsigned int rings, unsigned int sectors)
{
    m_positions.clear();
    m_indices.clear();

    for (unsigned int r = 0; r <= rings; ++r) {
        for (unsigned int s = 0; s <= sectors; ++s) {
            float theta = r * M_PI / rings;
            float phi = s * 2 * M_PI / sectors;

            float x = radius * sin(theta) * cos(phi);
            float y = radius * cos(theta);
            float z = radius * sin(theta) * sin(phi);

            m_positions.push_back(glm::vec3(x, y, z));
        }
    }

    for (unsigned int r = 0; r < rings; ++r) {
        for (unsigned int s = 0; s < sectors; ++s) {
            m_indices.push_back(r * (sectors + 1) + s);
            m_indices.push_back((r + 1) * (sectors + 1) + s);
            m_indices.push_back(r * (sectors + 1) + (s + 1));

            m_indices.push_back(r * (sectors + 1) + (s + 1));
            m_indices.push_back((r + 1) * (sectors + 1) + s);
            m_indices.push_back((r + 1) * (sectors + 1) + (s + 1));
        }
    }
}

void Sphere::setupMesh()
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(glm::vec3), &m_positions[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}
