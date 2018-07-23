#pragma once
#include <string>
#include <vector>
#include "Mesh.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Shader;

class Model
{
public:
    Model(const std::string& path);
    void Draw(Shader shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(const std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    uint32_t textureFromFile(const char *path, const std::string &directory);
};

