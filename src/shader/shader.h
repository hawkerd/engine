#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
    public:
        unsigned int id; // program ID

        Shader(const char* vertexPath, const char* fragmentPath) {
            // 1. retrieve vertex/fragment source code
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;

            // ensure filestream objects can throw exceptions
            vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

            try {
                // open files
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);

                // read buffer contents into streams
                std::stringstream vShaderStream, fShaderStream;
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();

                // close files
                vShaderFile.close();
                fShaderFile.close();

                // convert to string
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            } catch (std::ifstream::failure& e) {
                throw std::runtime_error("Issue loading shader file\n");
            }

            // convert
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            // 2. compile shaders
            unsigned int vertex, fragment;

            // vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            checkCompileErrors(vertex, "VERTEX");
            
            // fragment shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            checkCompileErrors(fragment, "FRAGMENT");

            // shader program
            id = glCreateProgram();
            glAttachShader(id, vertex);
            glAttachShader(id, fragment);
            glLinkProgram(id);
            checkCompileErrors(id, "PROGRAM");

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void use() {
            // use this shader program
            glUseProgram(id);
        }

        void end() {
            glDeleteProgram(id);
        }

    // utility uniform functions
    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); 
    }
    void setInt(const std::string &name, int value) const { 
        glUniform1i(glGetUniformLocation(id, name.c_str()), value); 
    }
    void setFloat(const std::string &name, float value) const { 
        glUniform1f(glGetUniformLocation(id, name.c_str()), value); 
    }
    void setVec2(const std::string &name, const glm::vec2 &value) const { 
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const { 
        glUniform2f(glGetUniformLocation(id, name.c_str()), x, y); 
    }
    void setVec3(const std::string &name, const glm::vec3 &value) const { 
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const { 
        glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z); 
    }
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) const { 
        glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w); 
    }
    void setMat2(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setMat3(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    private:
        void checkCompileErrors(unsigned int shader, std::string type) {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM") {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success) {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            } else {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
        }
};

#endif