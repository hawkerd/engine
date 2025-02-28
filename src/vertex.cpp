#include "vertex.h"

std::vector<std::tuple<std::string, int, GLenum, std::size_t>> Vertex::getAttributes() const {
    std::vector<std::tuple<std::string, int, GLenum, std::size_t>> attributes;
    if (attributeMask & POSITION) {
        attributes.push_back(std::make_tuple("position", 3, GL_FLOAT, offsetof(Vertex, position)));
    }
    if (attributeMask & NORMAL) {
        attributes.push_back(std::make_tuple("normal", 3, GL_FLOAT, offsetof(Vertex, normal)));
    }
    if (attributeMask & TEXCOORD) {
        attributes.push_back(std::make_tuple("texCoords", 2, GL_FLOAT, offsetof(Vertex, textureCoordinates)));
    }
    if (attributeMask & TANGENT) {
        attributes.push_back(std::make_tuple("tangent", 3, GL_FLOAT, offsetof(Vertex, tangent)));
    }
    if (attributeMask & BITANGENT) {
        attributes.push_back(std::make_tuple("bitangent", 3, GL_FLOAT, offsetof(Vertex, bitangent)));
    }
    if (attributeMask & BONES) {
        attributes.push_back(std::make_tuple("boneIds", 4, GL_INT, offsetof(Vertex, boneIds)));
        attributes.push_back(std::make_tuple("weights", 4, GL_FLOAT, offsetof(Vertex, weights)));
    }
    return attributes;
}