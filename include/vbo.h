#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <array>
#include "vertex.h"

class VBO {
    public:
        GLuint id;
        VBO(const std::vector<Vertex>& vertices);

        void bind();
        void unbind();
        void del();
};

#endif