#include "vbo.h"

void VBO::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::del() const {
    unbind();
    glDeleteBuffers(1, &id);
}