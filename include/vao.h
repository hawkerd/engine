#ifndef VAO_H
#define VAO_H


#include <glad/glad.h>
#include <vbo.h>
#include <shader.h>

class VAO {
    public:
        GLuint id;
        VAO();

        void linkAttribs(VBO& vbo, Shader& shader);
        void bind();
        void unbind();
        void del();
};

#endif