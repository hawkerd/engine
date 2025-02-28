#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include <tuple>
#include <string>
#include <array>

#define MAX_BONE_INFLUENCE 4

enum VertexAttributes {
    POSITION   = 1 << 0,  // 000001
    NORMAL     = 1 << 1,  // 000010
    TEXCOORD   = 1 << 2,  // 000100
    TANGENT    = 1 << 3,  // 001000
    BITANGENT  = 1 << 4,  // 010000
    BONES      = 1 << 5   // 100000
};

class Vertex {
    public:
        // constructors for different combinations of attributes
        Vertex() = default;
        Vertex(glm::vec3 pos) : position(pos), attributeMask(POSITION) {}
        Vertex(glm::vec3 pos, glm::vec3 norm) : position(pos), normal(norm), attributeMask(POSITION | NORMAL) {}
        Vertex(glm::vec3 pos, glm::vec2 texCoords) : position(pos), textureCoordinates(texCoords), attributeMask(POSITION | TEXCOORD) {}
        Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texCoords) : position(pos), normal(norm), textureCoordinates(texCoords), attributeMask(POSITION | NORMAL | TEXCOORD) {}
        Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texCoords, glm::vec3 tan, glm::vec3 bitan, std::array<int, MAX_BONE_INFLUENCE> boneIds, std::array<float, MAX_BONE_INFLUENCE> weights) 
            : position(pos), normal(norm), textureCoordinates(texCoords), tangent(tan), bitangent(bitan), boneIds(boneIds), weights(weights), attributeMask(POSITION | NORMAL | TEXCOORD | TANGENT | BITANGENT | BONES) {}

        // destructor
        ~Vertex() = default;

        // get the attributes of the vertex (name, size, type, and offset)
        std::vector<std::tuple<std::string, int, GLenum, std::size_t>> getAttributes() const;

        // set attributes
        void setPosition(glm::vec3 pos) { position = pos; attributeMask |= POSITION; }
        void setNormal(glm::vec3 norm) { normal = norm; attributeMask |= NORMAL; }
        void setTextureCoordinates(glm::vec2 texCoords) { textureCoordinates = texCoords; attributeMask |= TEXCOORD; }
        void setTangent(glm::vec3 tan) { tangent = tan; attributeMask |= TANGENT; }
        void setBitangent(glm::vec3 bitan) { bitangent = bitan; attributeMask |= BITANGENT; }
        void setBoneIds(std::array<int, MAX_BONE_INFLUENCE> ids) { boneIds = ids; attributeMask |= BONES; }
        void setWeights(std::array<float, MAX_BONE_INFLUENCE> w) { weights = w; attributeMask |= BONES; }

        // get attribute info
        bool hasPosition() const { return attributeMask & POSITION; }
        bool hasNormal() const { return attributeMask & NORMAL; }
        bool hasTextureCoordinates() const { return attributeMask & TEXCOORD; }
        bool hasTangent() const { return attributeMask & TANGENT; }
        bool hasBitangent() const { return attributeMask & BITANGENT; }
        bool hasBones() const { return attributeMask & BONES; }

        // get attribute locations
        glm::vec3* getPosition() { return &position; }
        glm::vec3* getNormal() { return hasNormal() ? &normal : nullptr; }
        glm::vec2* getTextureCoordinates() { return hasTextureCoordinates() ? &textureCoordinates : nullptr; }
        glm::vec3* getTangent() { return hasTangent() ? &tangent : nullptr; }
        glm::vec3* getBitangent() { return hasBitangent() ? &bitangent : nullptr; }
        std::array<int, MAX_BONE_INFLUENCE>* getBoneIds() { return hasBones() ? &boneIds : nullptr; }
        std::array<float, MAX_BONE_INFLUENCE>* getWeights() { return hasBones() ? &weights : nullptr; }

        // get attribute sizes
        static std::size_t getPositionSize() { return sizeof(glm::vec3); }
        static std::size_t getNormalSize() { return sizeof(glm::vec3); }
        static std::size_t getTextureCoordinatesSize() { return sizeof(glm::vec2); }
        static std::size_t getTangentSize() { return sizeof(glm::vec3); }
        static std::size_t getBitangentSize() { return sizeof(glm::vec3); }
        static std::size_t getBoneIdsSize() { return sizeof(int) * MAX_BONE_INFLUENCE; }
        static std::size_t getWeightsSize() { return sizeof(float) * MAX_BONE_INFLUENCE; }

        // get vertex size
        std::size_t getSize() const {
            std::size_t size = 0;
            if (attributeMask & POSITION) size += getPositionSize();
            if (attributeMask & NORMAL) size += getNormalSize();
            if (attributeMask & TEXCOORD) size += getTextureCoordinatesSize();
            if (attributeMask & TANGENT) size += getTangentSize();
            if (attributeMask & BITANGENT) size += getBitangentSize();
            if (attributeMask & BONES) {
                size += getBoneIdsSize();
                size += getWeightsSize();
            }
            return size;
        }


    private:
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 normal = glm::vec3(0.0f);
        glm::vec2 textureCoordinates = glm::vec2(0.0f);
        glm::vec3 tangent = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 bitangent = glm::vec3(0.0f, 1.0f, 0.0f);
        std::array<int, MAX_BONE_INFLUENCE> boneIds = {0, 0, 0, 0};
        std::array<float, MAX_BONE_INFLUENCE> weights = {1.0f, 0.0f, 0.0f, 0.0f};
        uint8_t attributeMask = 0;
};


#endif