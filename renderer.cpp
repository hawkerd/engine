#include "renderer.h"
#include <iostream>

Renderer::~Renderer() {
    glfwTerminate();
}

Renderer::Renderer() {
    init();
    render();
}

void Renderer::init() {
    int success;
    char infoLog[512];

    // initialize GLFW
    if (glfwInit() != GL_TRUE) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // specify OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // initialize GLFW window
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "visualizer", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwMakeContextCurrent(window); // make context current
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // register callback resizing function

    // load all OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // create shader object, store ID
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // attach source
    glCompileShader(vertexShader); // compile
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // get status
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // get info log
        glfwTerminate();
        throw std::runtime_error("Failed to compile vertex shader");
    }

    // fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        glfwTerminate();
        throw std::runtime_error("Failed to compile fragment shader");
    }

    // link shaders
    shaderProgram = glCreateProgram(); // create shader program
    glAttachShader(shaderProgram, vertexShader); // attach shaders
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); // link
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); // get status
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        glfwTerminate();
        throw std::runtime_error("Failed to link shader program");
    }

    // delete shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // define vertices
    //float vertices[] = {
    //    -0.5f, -0.5f, 0.0f,
    //    0.5f, -0.5f, 0.0f,
    //    0.0f, 0.5f, 0.0f
    //};
    float vertices[] = {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
       -0.5f,  0.5f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // set up buffers
    glGenVertexArrays(1, &VAO); // generate VAO (stores buffer configuration, that is VBO and EBO bindings)
    glGenBuffers(1, &VBO); // generate VBO (store vertices)
    glGenBuffers(1, &EBO); // generate EBO (store indices)
    glBindVertexArray(VAO); // bind VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // populate VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // populate EBO

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0); // configure vertex attributes
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO. will not affect VAO stored state
    glBindVertexArray(0); // unbind the VAO


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

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3); // draw 3 vertices from the currently bound vertex array
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window); // swap the new buffer to the screen
        glfwPollEvents(); // poll events
    }

    // clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}

void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); // resize the OpenGL viewport
}

void Renderer::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}