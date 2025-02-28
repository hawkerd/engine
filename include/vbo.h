#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <array>
#include "vertex.h"
#include <iostream>
#include <cstring>

struct Attribute {
    GLint size;
    GLenum type;
    GLboolean normalized;
    std::size_t offset;
    std::string name;
};

class VBO {
    public:
        GLuint id;
        GLsizei stride;
        std::vector<Attribute> attributes;

        VBO(std::vector<Vertex>& vertices);

        void bind();
        void unbind();
        void del();
};

#endif