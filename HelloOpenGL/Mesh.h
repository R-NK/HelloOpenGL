#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include "Vertex.h"
#include "Texture.h"

class Mesh
{

public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
    void Draw(Shader shader);
    ~Mesh();

private:
    uint32_t VBO, VAO, EBO;
    void setupMesh();
};

