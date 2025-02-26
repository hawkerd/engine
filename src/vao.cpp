#include "vao.h"

VAO::VAO() {
    glGenVertexArrays(1, &id);
}

void VAO::linkAttribs(VBO& vbo, Shader& shader) {
    bind();
    vbo.bind();

    for (const auto& [name, size, type, offset] : Vertex::getAttributes()) {
        GLint location = glGetAttribLocation(shader.id, name.c_str());
        if (location == -1) {
            continue;
        }

        if (type == GL_INT) {
            glVertexAttribIPointer(location, size, type, sizeof(Vertex), (void*)offset);
        } else {
            glVertexAttribPointer(location, size, type, GL_FALSE, sizeof(Vertex), (void*)offset);
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