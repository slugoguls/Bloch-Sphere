#pragma once

#include <vector>
#include <glm/glm.hpp>

class Sphere {
public:
    Sphere(float radius, unsigned int rings, unsigned int sectors);

    void draw() const;

private:
    void generateVertices(float radius, unsigned int rings, unsigned int sectors);
    void setupMesh();

    unsigned int m_VAO, m_VBO, m_EBO;
    std::vector<glm::vec3> m_positions;
    std::vector<unsigned int> m_indices;
};
