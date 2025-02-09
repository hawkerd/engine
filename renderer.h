#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

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


        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int shaderProgram;


        double lastUpdateTime = 0; // track the last update (cycle) time
        double lastFrameTime = 0; // track the last time a frame was displayed
        double fpsLimit = 60.0;
        const char* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        
        const char* fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()"
            "{\n"
                "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\0";

        void init();
        void drawTriangle();
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height); // callback function to handle window resize
        void processInput(GLFWwindow* window); // handle input
};

#endif