#include "vertex.h"

std::vector<std::tuple<std::string, int, GLenum, std::size_t>> Vertex::getAttributes() {
    return {
        {"position", 3, GL_FLOAT, offsetof(Vertex, position)},
        {"normal", 3, GL_FLOAT, offsetof(Vertex, normal)},
        {"textureCoordinates", 2, GL_FLOAT, offsetof(Vertex, textureCoordinates)},
        {"tangent", 3, GL_FLOAT, offsetof(Vertex, tangent)},
        {"bitangent", 3, GL_FLOAT, offsetof(Vertex, bitangent)},
        {"boneIds", 4, GL_INT, offsetof(Vertex, boneIds)},
        {"weights", 4, GL_FLOAT, offsetof(Vertex, weights)}
    };
}

Vertex::Vertex()
    : position(0.0f), normal(0.0f), textureCoordinates(0.0f),
      tangent(1.0f, 0.0f, 0.0f), bitangent(0.0f, 1.0f, 0.0f),
      boneIds({0, 0, 0, 0}), weights({1.0f, 0.0f, 0.0f, 0.0f}) {}