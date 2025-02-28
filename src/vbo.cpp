#include "vbo.h"

VBO::VBO(std::vector<Vertex>& vertices) {
    std::cout << "Generating buffer" << std::endl;
    glGenBuffers(1, &id);

    std::cout << "Binding buffer" << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, id);

    if (vertices.empty()) {
        std::cout << "No vertices" << std::endl;
        return;
    }

    // ensure all vertices have the same size
    // TODO: should they all have the same bitmask?
    std::size_t size = vertices[0].getSize();
    for (const auto& vertex : vertices) {
        if (vertex.getSize() != size) {
            throw std::runtime_error("All vertices must have the same size");
        }
    }
    stride = size;
    std::cout << "Stride: " << stride << std::endl;

    // determine the attributes of the vertex
    bool position = vertices[0].hasPosition();
    std::size_t offset = 0;
    if (position) {
        attributes.push_back({ 3, GL_FLOAT, GL_FALSE, offset, "position" });
        offset += vertices[0].getPositionSize();
    }
    bool normal = vertices[0].hasNormal();
    if (normal) {
        attributes.push_back({ 3, GL_FLOAT, GL_FALSE, offset, "normal" });
        offset += vertices[0].getNormalSize();
    }
    bool texCoord = vertices[0].hasTextureCoordinates();
    if (texCoord) {
        attributes.push_back({ 2, GL_FLOAT, GL_FALSE, offset, "textureCoordinates" });
        offset += vertices[0].getTextureCoordinatesSize();
    }
    bool tangent = vertices[0].hasTangent();
    if (tangent) {
        attributes.push_back({ 3, GL_FLOAT, GL_FALSE, offset, "tangent" });
        offset += vertices[0].getTangentSize();
    }
    bool bitangent = vertices[0].hasBitangent();
    if (bitangent) {
        attributes.push_back({ 3, GL_FLOAT, GL_FALSE, offset, "bitangent" });
        offset += vertices[0].getBitangentSize();
    }
    bool bones = vertices[0].hasBones();
    if (bones) {
        attributes.push_back({ MAX_BONE_INFLUENCE, GL_UNSIGNED_INT, GL_FALSE, offset, "boneIds" });
        offset += vertices[0].getBoneIdsSize();
        attributes.push_back({ MAX_BONE_INFLUENCE, GL_FLOAT, GL_FALSE, offset, "weights" });
        offset += vertices[0].getWeightsSize();
    }
    std::cout << "Attributes: " << attributes.size() << std::endl;
    for (const auto& attribute : attributes) {
        std::cout << "Attribute: " << attribute.name << std::endl;
    }

    // calculate the total size of the buffer and allocate
    std::size_t totalSize = vertices.size() * size;
    std::vector<uint8_t> buffer(totalSize);
    std::size_t totalOffset = 0;
    std::cout << "Total size: " << totalSize << std::endl;

    // copy each vertex into the buffer
    for (auto& vertex : vertices) {
        std::size_t offset = totalOffset;
        if (position) {
            std::memcpy(buffer.data() + offset, vertex.getPosition(), sizeof(glm::vec3));
            offset += vertex.getPositionSize();
        }
        if (normal) {
            std::memcpy(buffer.data() + offset, vertex.getNormal(), sizeof(glm::vec3));
            offset += vertex.getNormalSize();
        }
        if (texCoord) {
            std::memcpy(buffer.data() + offset, vertex.getTextureCoordinates(), sizeof(glm::vec2));
            offset += vertex.getTextureCoordinatesSize();
        }
        if (tangent) {
            std::memcpy(buffer.data() + offset, vertex.getTangent(), sizeof(glm::vec3));
            offset += vertex.getTangentSize();
        }
        if (bitangent) {
            std::memcpy(buffer.data() + offset, vertex.getBitangent(), sizeof(glm::vec3));
            offset += vertex.getBitangentSize();
        }
        if (bones) {
            std::memcpy(buffer.data() + offset, vertex.getBoneIds(), sizeof(int) * MAX_BONE_INFLUENCE);
            offset += vertex.getBoneIdsSize();
            std::memcpy(buffer.data() + offset, vertex.getWeights(), sizeof(float) * MAX_BONE_INFLUENCE);
            offset += vertex.getWeightsSize();
        }
        totalOffset += size;
    }

    glBufferData(GL_ARRAY_BUFFER, totalSize, buffer.data(), GL_STATIC_DRAW);
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::del() {
    unbind();
    glDeleteBuffers(1, &id);
}