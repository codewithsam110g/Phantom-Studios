#ifndef MODEL_H
#define MODEL_H

#include <assimp/material.h>
#include <unordered_map>

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STBI_ASSERT(x) assert(x)
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <vector>
#include <iostream>

// Function to load textures from files
unsigned int TextureFromFile(const char* path, bool gamma = false);

class Model {
public:
    std::vector<Texture_> textures_loaded;  // Textures loaded to prevent duplicates
    std::vector<Mesh> meshes;
    std::string path;
    std::string directory;
    bool gammaCorrection;

    // Default constructor
    Model() : gammaCorrection(false) {}

    // Constructor with path
    Model(const std::string& path, bool gamma = false)
        : path(path), gammaCorrection(gamma) {
        loadModel();
    }

    // Draw the model
    void Draw(Shader& shader) {
        for (auto& mesh : meshes) {
            mesh.Draw(shader);
        }
    }

private:
    // Load the model using the appropriate loader
    void loadModel() {
#ifdef USE_ASSIMP
        loadModelAssimp();
#else
        loadModelTinyObj();
#endif
    }

    // Load model with Assimp
    void loadModelAssimp() {

        std::cout<<"Using Assimp to Load: "<<path<<std::endl;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_FlipUVs |
            aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
    }

    // Recursive node processing for Assimp
    void processNode(aiNode* node, const aiScene* scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMeshAssimp(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    // Process individual meshes for Assimp
    Mesh processMeshAssimp(aiMesh* mesh, const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture_> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.Position = glm::vec3(
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z);

            vertex.Normal = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z);

            vertex.TexCoords = mesh->mTextureCoords[0]
                ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                : glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            
            auto normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            
            auto heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
            
        }

        return Mesh(vertices, indices, textures);
    }

    // Load material textures for Assimp
    std::vector<Texture_> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) {
        std::vector<Texture_> textures;

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;

            for (auto& loaded : textures_loaded) {
                if (loaded.path == str.C_Str()) {
                    textures.push_back(loaded);
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                Texture_ texture;
                texture.id = TextureFromFile(str.C_Str());
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }

        return textures;
    }

    // Load model with TinyObjLoader
    void loadModelTinyObj() {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        std::cout << "Using TinyObjLoader to Load: " << path << std::endl;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), directory.c_str())) {
            std::cerr << "TinyObjLoader: " << warn << err << std::endl;
            return;
        }

        // Map to store textures for each material
        std::unordered_map<int, std::vector<Texture_>> materialTextures;

        // Preload textures for all materials
        for (size_t i = 0; i < materials.size(); ++i) {
            const auto& mat = materials[i];
            std::vector<Texture_> textures;

            if (!mat.diffuse_texname.empty()) {
                bool found = false;
                for (const auto& loaded : textures_loaded) {
                    if (loaded.path == mat.diffuse_texname) {
                        textures.push_back(loaded);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    Texture_ texture;
                    texture.id = TextureFromFile(mat.diffuse_texname.c_str());
                    texture.type = "texture_diffuse";
                    texture.path = mat.diffuse_texname;
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);
                }
            }

            if (!mat.specular_texname.empty()) {
                bool found = false;
                for (const auto& loaded : textures_loaded) {
                    if (loaded.path == mat.specular_texname) {
                        textures.push_back(loaded);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    Texture_ texture;
                    texture.id = TextureFromFile(mat.specular_texname.c_str());
                    texture.type = "texture_specular";
                    texture.path = mat.specular_texname;
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);
                }
            }
            
            if (!mat.normal_texname.empty()) {
                bool found = false;
                for (const auto& loaded : textures_loaded) {
                    if (loaded.path == mat.normal_texname) {
                        textures.push_back(loaded);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    Texture_ texture;
                    texture.id = TextureFromFile(mat.normal_texname.c_str());
                    texture.type = "texture_normal";
                    texture.path = mat.normal_texname;
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);
                }
            }
            
            if (!mat.bump_texname.empty()) {
                bool found = false;
                for (const auto& loaded : textures_loaded) {
                    if (loaded.path == mat.bump_texname) {
                        textures.push_back(loaded);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    Texture_ texture;
                    texture.id = TextureFromFile(mat.bump_texname.c_str());
                    texture.type = "texture_height";
                    texture.path = mat.bump_texname;
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);
                }
            }

            materialTextures[i] = textures;
        }

        // Process shapes
        for (const auto& shape : shapes) {
            meshes.push_back(processMeshTinyObj(attrib, shape, materialTextures));
        }
    }



    // Process individual meshes for TinyObjLoader
    Mesh processMeshTinyObj(const tinyobj::attrib_t& attrib, const tinyobj::shape_t& shape,
                            const std::unordered_map<int, std::vector<Texture_>>& materialTextures) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        // Process vertices and indices
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex;
            vertex.Position = glm::vec3(
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]);

            if (index.normal_index >= 0) {
                vertex.Normal = glm::vec3(
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]);
            } else {
                vertex.Normal = glm::vec3(0.0f); // Default normal if not present
            }

            if (index.texcoord_index >= 0) {
                vertex.TexCoords = glm::vec2(
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1 - attrib.texcoords[2 * index.texcoord_index + 1]);
            } else {
                vertex.TexCoords = glm::vec2(0.0f); // Default texture coordinates
            }

            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }

        // Retrieve preloaded textures for the material
        std::vector<Texture_> textures;
        if (!shape.mesh.material_ids.empty()) {
            int material_id = shape.mesh.material_ids[0];
            auto it = materialTextures.find(material_id);
            if (it != materialTextures.end()) {
                textures = it->second;
            }
        }

        return Mesh(vertices, indices, textures);
    }




};

// Function to load a texture from a file
unsigned int TextureFromFile(const char* path, bool gamma) {
    std::string filename = path;
    std::cout<<path<<std::endl;
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

    if (data) {
        GLenum format = (nrComponents == 1) ? GL_RED : (nrComponents == 3) ? GL_RGB : GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cerr << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

#endif // MODEL_H
