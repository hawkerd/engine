#include "renderer.h"
#include "stb_image.h"
#include <iostream>
#include <fstream>
#include <cmath>

Renderer::~Renderer() {
    delete s;
    glfwTerminate();
}

Renderer::Renderer() {
    // initialize GLFW
    if (glfwInit() != GL_TRUE) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // specify OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // initialize GLFW window
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "visualizer", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwMakeContextCurrent(window); // make context current
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // register callback resizing function

    // load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // build/compile shader
    s = new Shader("/home/hawkerd/projects/engine/src/shader/shader.vert", 
        "/home/hawkerd/projects/engine/src/shader/shader.frag");

    // set up vertex + buffer data
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };

    unsigned int indices[] = {  
        0, 1, 3,
        1, 2, 3 
    };

    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // generate VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // bind VAO
    glBindVertexArray(VAO);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0); // configure vertex attributes
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO. will not affect VAO stored state
    glBindVertexArray(0); // unbind the VAO

    // create texture 1
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // load/generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
    if (!data) {
        throw std::runtime_error("Failed to load texture from file");
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    // create another texture
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (!data) {
        throw std::runtime_error("Failed to load texture from file");
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    // set uniforms
    s->use();
    s->setInt("texture1", 0);
    s->setInt("texture2", 1);

    initialized = true;
}

void Renderer::render() {
    /*
    // while the window has not been closed, render content
    while (!glfwWindowShouldClose(window)) {
        // grab current time
        double now = glfwGetTime();
        lastUpdateTime = now;

        // poll events
        glfwPollEvents();

        // process input
        processInput(window);

        // if 1 frame time has passed, render a new frame
        if ((now - lastFrameTime) >= (1.0 / fpsLimit)) {
            lastFrameTime = now;

            // clear old frame
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // draw new frame
            drawTriangle();

            // swap the back buffer to the front to display the newly rendered frame
            glfwSwapBuffers(window);
        }

    }
    */
    while (!glfwWindowShouldClose(window)) {
        // process input
        processInput(window);

        // clear old frame
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        s->use();

        // create matrix
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime() * 2, glm::vec3(0.0f, 1.0f, 1.0f));

        s->setMat4("transform", trans);

        
        // bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window); // swap the new buffer to the screen
        glfwPollEvents(); // poll events
    }

    // clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    s->end();
    glfwTerminate();
}

void Renderer::framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height); // resize the OpenGL viewport
}

void Renderer::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}