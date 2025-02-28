#include "vao.h"

VAO::VAO() {
    glGenVertexArrays(1, &id);
}

void VAO::linkAttribs(const VBO& vbo, const Shader& shader) {
    bind();
    vbo.bind();

    for (const auto& attr : vbo.attributes) {
        GLint location = glGetAttribLocation(shader.id, attr.name.c_str());
        if (location == -1) {
            std::cerr << "Warning: attribute " << attr.name << " not found in shader" << std::endl;
            continue;
        }

        if (attr.type == GL_INT || attr.type == GL_UNSIGNED_INT) {
            glVertexAttribIPointer(location, attr.numComponents, attr.type, attr.stride, (void*)attr.offset);
        } else {
            glVertexAttribPointer(location, attr.numComponents, attr.type, attr.normalized, attr.stride, (void*)attr.offset);
        }

        glEnableVertexAttribArray(location);

    }

    vbo.unbind();
    unbind();
}

void VAO::bind() {
    glBindVertexArray(id);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

void VAO::del() {
    glDeleteVertexArrays(1, &id);
}