#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class Window {
    public:
        int width, height;
        GLFWwindow* window;

        Window(int width = 1920, int height = 1080, std::string title = "window") : width(width), height(height), title(title) {
            if (!glfwInit()) {
                throw std::runtime_error("Failed to initialize GLFW");
            }
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            #ifdef __APPLE__
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            #endif

            window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
            if (!window) {
                glfwTerminate();
                throw std::runtime_error("Failed to create window");
            }

            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        }

        void setCursorPosCallback(GLFWcursorposfun callback) {
            glfwSetCursorPosCallback(window, callback);
        }

        void setScrollCallback(GLFWscrollfun callback) {
            glfwSetScrollCallback(window, callback);
        }

        bool shouldClose() {
            return glfwWindowShouldClose(window);
        }

        void swapBuffers() {
            glfwSwapBuffers(window);
        }

        void pollEvents() {
            glfwPollEvents();
        }

        void setClearColor(float r, float g, float b, float a) {
            glClearColor(r, g, b, a);
        }

        void clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void close() {
            glfwSetWindowShouldClose(window, true);
        }

        void terminate() {
            glfwTerminate();
        }

    private:
        std::string title;
        
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        }
};


#endif