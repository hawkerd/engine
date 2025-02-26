#ifndef VERTEX_H
#define VERTEX_H

#define MAX_BONE_INFLUENCE 4

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <array>
#include <vector>
#include <tuple>
#include <string>

class Vertex {
    public:
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textureCoordinates;
        glm::vec3 tangent;
        glm::vec3 bitangent;
        std::array<int, MAX_BONE_INFLUENCE> boneIds;
        std::array<float, MAX_BONE_INFLUENCE> weights;

        Vertex();

        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates, 
            glm::vec3 tangent = glm::vec3(1.0f, 0.0f, 0.0f), 
            glm::vec3 bitangent = glm::vec3(0.0f, 1.0f, 0.0f), 
            std::array<int, MAX_BONE_INFLUENCE> boneIds = {0, 0, 0, 0}, 
            std::array<float, MAX_BONE_INFLUENCE> weights = {1.0f, 0.0f, 0.0f, 0.0f})
         : position(position), normal(normal), textureCoordinates(textureCoordinates), 
           tangent(tangent), bitangent(bitangent), boneIds(boneIds), weights(weights) {}

        static std::vector<std::tuple<std::string, int, GLenum, std::size_t>> getAttributes();
};

#endif