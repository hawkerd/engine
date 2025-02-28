#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <array>
#include "vertex.h"
#include <iostream>
#include <cstring>

class VBO {
    public:
        GLuint id;
        std::vector<VertexAttribute> attributes;

        template <typename VertexType>
        VBO(const std::vector<VertexType>& vertices) {
            glGenBuffers(1, &id);
            bind();
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexType), vertices.data(), GL_STATIC_DRAW);
        
            attributes = getVertexAttributes<VertexType>();
        
            unbind();
        }

        void bind() const;
        void unbind() const;
        void del() const;
};

#endif