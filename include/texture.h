#ifndef TEXTURE_H
#define TEXTURE_H

#include <stb/stb_image.h>
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

class Texture {
    public:
        GLuint id;
        GLenum format;
        std::string type;
        std::string path;

        Texture(const std::string& path) : path(path), id(0) {
            spdlog::info("loading texture from path: {}", path);
            loadTexture(path);
            if (id == 0) {
                spdlog::error("Failed to load texture");
            }
        }
         
        void bind() const {
            glBindTexture(GL_TEXTURE_2D, id);
        }

        void del() {
            if (id) {
                glDeleteTextures(1, &id);
                id = 0; // Prevents accidental reuse
            }
        }

        void unBind() const {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

    private:
        void loadTexture(const std::string path) {
            glGenTextures(1, &id);

            int width, height, nrComponents;
            unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
            if (!data) {
                throw std::runtime_error("Failed to load texture");
            }

            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }

};

#endif