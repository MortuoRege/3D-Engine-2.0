#ifndef MESH_H
#define MESH_H
#include <stdexcept>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

class Mesh {

public:
//layout for the data [pos | normal | uv | color]
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec4 color;
};

    Mesh(const std::string& filePath);
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh(){
        if (EBO) glDeleteBuffers(1, &EBO); // harmless if zero, even if unused
        if (VBO) glDeleteBuffers(1, &VBO);
        if (VAO) glDeleteVertexArrays(1, &VAO);
    }

    void createMesh()
    {
        if (vertices.empty()) {
                throw std::runtime_error("Mesh::createMesh() called with no vertices.");
            }

        if(!VBO) glGenBuffers(1, &VBO);
        if(!EBO) glGenBuffers(1, &EBO);
        if(!VAO) glGenVertexArrays(1, &VAO);

        // EBO (if indices)
            if (!indices.empty()) {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
            }

        glBindVertexArray(VAO);


        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // OK to leave EBO bound to V

    }
    void bind() const
    {
        glBindVertexArray(VAO);
    };
    void draw() const
    {
        if (!VAO) {return; }

        glBindVertexArray(VAO);
        if(!indices.empty())
        {
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        }
        else if (!vertices.empty()) {
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }

    }


private:
   unsigned int VAO = 0;
   unsigned int VBO = 0;
   unsigned int EBO = 0;

   std::vector<Vertex> vertices;
   std::vector<unsigned int> indices;
};

#endif // MESH_H
