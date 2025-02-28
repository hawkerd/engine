#include "vao.h"

VAO::VAO() {
    glGenVertexArrays(1, &id);
}

void VAO::linkAttribs(VBO& vbo, Shader& shader) {
    bind();
    vbo.bind();

    for (const auto& attr : vbo.attributes) {
        GLint location = glGetAttribLocation(shader.id, attr.name.c_str());
        if (location == -1) {
            continue;
        }

        if (attr.type == GL_INT || attr.type == GL_UNSIGNED_INT) {
            glVertexAttribIPointer(location, attr.size, attr.type, vbo.stride, (void*)attr.offset);
        } else {
            glVertexAttribPointer(location, attr.size, attr.type, attr.normalized, vbo.stride, (void*)attr.offset);
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