#include <glad/glad.h>
#include "shader/shader.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef RENDERER_H
#define RENDERER_H

class Renderer {
    public:
        void render();
        ~Renderer();
        Renderer();

    private:
        GLFWwindow* window;
        bool initialized;

        const unsigned int SCR_WIDTH = 1600;
        const unsigned int SCR_HEIGHT = 900;

        unsigned int texture1;
        unsigned int texture2;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        Shader* s;


        double lastUpdateTime = 0; // track the last update (cycle) time
        double lastFrameTime = 0; // track the last time a frame was displayed
        double fpsLimit = 60.0;

        void drawTriangle();
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height); // callback function to handle window resize
        void processInput(GLFWwindow* window); // handle input
};

#endif