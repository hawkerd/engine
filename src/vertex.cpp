#include "vertex.h"

template <typename VertexType>
std::vector<VertexAttribute> getVertexAttributes();

template <>
std::vector<VertexAttribute> getVertexAttributes<VertexP>() {
    GLsizei stride = sizeof(VertexP);
    return {
        {"position", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), stride, offsetof(VertexP, position)}
    };
}

template <>
std::vector<VertexAttribute> getVertexAttributes<VertexPN>() {
    GLsizei stride = sizeof(VertexPN);
    return {
        {"position", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), stride, offsetof(VertexPN, position)},
        {"normal", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), stride, offsetof(VertexPN, normal)}
    };
}

template <>
std::vector<VertexAttribute> getVertexAttributes<VertexPT>() {
    GLsizei stride = sizeof(VertexPT);
    return {
        {"position", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), stride, offsetof(VertexPT, position)},
        {"textureCoordinates", 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), stride, offsetof(VertexPT, textureCoordinates)}
    };
}

template <>
std::vector<VertexAttribute> getVertexAttributes<VertexPNT>() {
    GLsizei stride = sizeof(VertexPNT);
    return {
        {"position", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), stride, offsetof(VertexPNT, position)},
        {"normal", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), stride, offsetof(VertexPNT, normal)},
        {"textureCoordinates", 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), stride, offsetof(VertexPNT, textureCoordinates)}
    };
}

template <>
std::vector<VertexAttribute> getVertexAttributes<VertexPNTB>() {
    GLsizei stride = sizeof(VertexPNTB);
    return {
        {"position", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), stride, offsetof(VertexPNTB, position)},
        {"normal", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), stride, offsetof(VertexPNTB, normal)},
        {"textureCoordinates", 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), stride, offsetof(VertexPNTB, textureCoordinates)},
        {"tangent", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), stride, offsetof(VertexPNTB, tangent)},
        {"bitangent", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), stride, offsetof(VertexPNTB, bitangent)}
    };
}

template <>
std::vector<VertexAttribute> getVertexAttributes<VertexPNTBS>() {
    GLsizei stride = sizeof(VertexPNTBS);
    return {
        {"position", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), stride, offsetof(VertexPNTBS, position)},
        {"normal", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), stride, offsetof(VertexPNTBS, normal)},
        {"textureCoordinates", 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), stride, offsetof(VertexPNTBS, textureCoordinates)},
        {"tangent", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), stride, offsetof(VertexPNTBS, tangent)},
        {"bitangent", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), stride, offsetof(VertexPNTBS, bitangent)},
        {"boneIDs", MAX_BONE_INFLUENCE, GL_INT, GL_FALSE, sizeof(int) * MAX_BONE_INFLUENCE, stride, offsetof(VertexPNTBS, boneIDs)},
        {"weights", MAX_BONE_INFLUENCE, GL_FLOAT, GL_FALSE, sizeof(float) * MAX_BONE_INFLUENCE, stride, offsetof(VertexPNTBS, weights)}
    };
}
