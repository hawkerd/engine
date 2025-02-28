#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include <string>
#include <iostream>

#define MAX_BONE_INFLUENCE 4

class VertexP {
    public:
        glm::vec3 position;
        VertexP() = default;
        VertexP(glm::vec3 position) : position(position) {}
};

class VertexPN {
    public:
        glm::vec3 position;
        glm::vec3 normal;
        VertexPN() = default;
        VertexPN(glm::vec3 position, glm::vec3 normal) : position(position), normal(normal) {}
};

class VertexPT {
    public:
        glm::vec3 position;
        glm::vec2 textureCoordinates;
        VertexPT() = default;
        VertexPT(glm::vec3 position, glm::vec2 textureCoordinates) : position(position), textureCoordinates(textureCoordinates) {}
};

class VertexPNT {
    public:
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoordinates;
        VertexPNT() = default;
        VertexPNT(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates) : position(position), normal(normal), textureCoordinates(textureCoordinates) {}
};

class VertexPNTB {
    public:
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoordinates;
        glm::vec3 tangent;
        glm::vec3 bitangent;
        VertexPNTB() = default;
        VertexPNTB(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates, glm::vec3 tangent, glm::vec3 bitangent) : position(position), normal(normal), textureCoordinates(textureCoordinates), tangent(tangent), bitangent(bitangent) {}
};

class VertexPNTBS {
    public:
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoordinates;
        glm::vec3 tangent;
        glm::vec3 bitangent;
        int boneIDs[MAX_BONE_INFLUENCE];
        float weights[MAX_BONE_INFLUENCE];
        VertexPNTBS() = default;
        VertexPNTBS(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates, glm::vec3 tangent, glm::vec3 bitangent, int* boneIDs, float* weights) : position(position), normal(normal), textureCoordinates(textureCoordinates), tangent(tangent), bitangent(bitangent) {
            for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
                this->boneIDs[i] = boneIDs[i];
                this->weights[i] = weights[i];
            }
        }
};

struct VertexAttribute {
    std::string name;
    GLuint numComponents;
    GLenum type;
    bool normalized;
    GLsizei size;
    GLsizei stride;
    uintptr_t offset;
};

template <typename VertexType>
std::vector<VertexAttribute> getVertexAttributes();

#endif