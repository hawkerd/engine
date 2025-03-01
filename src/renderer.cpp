#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "window.h"
#include "texture.h"
#include "vertex.h"

#include <iostream>

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH  / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    Window window(SCR_WIDTH, SCR_HEIGHT, "DAN WINDOW");

    // initialize glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    window.setCursorPosCallback(mouse_callback);
    window.setScrollCallback(scroll_callback);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile shaders
    // -------------------------
    Shader shader("assets/shaders/lightingShader.vert", "assets/shaders/lightingShader.frag");
    Shader screenShader("assets/shaders/screenShader.vert", "assets/shaders/screenShader.frag");
    Shader skyboxShader("assets/shaders/skyboxShader.vert", "assets/shaders/skyboxShader.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<VertexPT> cubeVertices = {
        // Back face
        VertexPT({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}),
        VertexPT({ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}),
        VertexPT({ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}),
        VertexPT({ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}),
        VertexPT({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}),
        VertexPT({-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}),
        // Front face
        VertexPT({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}),
        VertexPT({ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}),
        VertexPT({ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}),
        VertexPT({ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}),
        VertexPT({-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}),
        VertexPT({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}),
        // Left face
        VertexPT({-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}),
        VertexPT({-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}),
        VertexPT({-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}),
        VertexPT({-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}),
        VertexPT({-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}),
        VertexPT({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}),
        // Right face
        VertexPT({ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}),
        VertexPT({ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}),
        VertexPT({ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}),
        VertexPT({ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}),
        VertexPT({ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}),
        VertexPT({ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}),
        // Bottom face
        VertexPT({-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}),
        VertexPT({ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}),
        VertexPT({ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}),
        VertexPT({ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}),
        VertexPT({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}),
        VertexPT({-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}),
        // Top face
        VertexPT({-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}),
        VertexPT({ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}),
        VertexPT({ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}),
        VertexPT({ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}),
        VertexPT({-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}),
        VertexPT({-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f})
    };

    vector<VertexPT> planeVertices = {
        VertexPT({ 5.0f, -0.5f,  5.0f}, {2.0f, 0.0f}),
        VertexPT({-5.0f, -0.5f,  5.0f}, {0.0f, 0.0f}),
        VertexPT({-5.0f, -0.5f, -5.0f}, {0.0f, 2.0f}),
        VertexPT({ 5.0f, -0.5f,  5.0f}, {2.0f, 0.0f}),
        VertexPT({-5.0f, -0.5f, -5.0f}, {0.0f, 2.0f}),
        VertexPT({ 5.0f, -0.5f, -5.0f}, {2.0f, 2.0f})
    };
    
    vector<VertexPT> quadVertices = {
        VertexPT({-1.0f,  1.0f,  0.0f}, {0.0f, 1.0f}),
        VertexPT({-1.0f, -1.0f,  0.0f}, {0.0f, 0.0f}),
        VertexPT({ 1.0f, -1.0f,  0.0f}, {1.0f, 0.0f}),
        VertexPT({-1.0f,  1.0f,  0.0f}, {0.0f, 1.0f}),
        VertexPT({ 1.0f, -1.0f,  0.0f}, {1.0f, 0.0f}),
        VertexPT({ 1.0f,  1.0f,  0.0f}, {1.0f, 1.0f})
    };

    vector<VertexP> skyboxVertices = {
        VertexP({-1.0f,  1.0f, -1.0f}),
        VertexP({-1.0f, -1.0f, -1.0f}),
        VertexP({ 1.0f, -1.0f, -1.0f}),
        VertexP({ 1.0f, -1.0f, -1.0f}),
        VertexP({ 1.0f,  1.0f, -1.0f}),
        VertexP({-1.0f,  1.0f, -1.0f}),

        VertexP({-1.0f, -1.0f,  1.0f}),
        VertexP({-1.0f, -1.0f, -1.0f}),
        VertexP({-1.0f,  1.0f, -1.0f}),
        VertexP({-1.0f,  1.0f, -1.0f}),
        VertexP({-1.0f,  1.0f,  1.0f}),
        VertexP({-1.0f, -1.0f,  1.0f}),

        VertexP({ 1.0f, -1.0f, -1.0f}),
        VertexP({ 1.0f, -1.0f,  1.0f}),
        VertexP({ 1.0f,  1.0f,  1.0f}),
        VertexP({ 1.0f,  1.0f,  1.0f}),
        VertexP({ 1.0f,  1.0f, -1.0f}),
        VertexP({ 1.0f, -1.0f, -1.0f}),

        VertexP({-1.0f, -1.0f,  1.0f}),
        VertexP({-1.0f,  1.0f,  1.0f}),
        VertexP({ 1.0f,  1.0f,  1.0f}),
        VertexP({ 1.0f,  1.0f,  1.0f}),
        VertexP({ 1.0f, -1.0f,  1.0f}),
        VertexP({-1.0f, -1.0f,  1.0f}),

        VertexP({-1.0f,  1.0f, -1.0f}),
        VertexP({ 1.0f,  1.0f, -1.0f}),
        VertexP({ 1.0f,  1.0f,  1.0f}),
        VertexP({ 1.0f,  1.0f,  1.0f}),
        VertexP({-1.0f,  1.0f,  1.0f}),
        VertexP({-1.0f,  1.0f, -1.0f}),

        VertexP({-1.0f, -1.0f, -1.0f}),
        VertexP({-1.0f, -1.0f,  1.0f}),
        VertexP({ 1.0f, -1.0f, -1.0f}),
        VertexP({ 1.0f, -1.0f, -1.0f}),
        VertexP({-1.0f, -1.0f,  1.0f}),
        VertexP({ 1.0f, -1.0f,  1.0f})
    };

    VAO cubeVAO;
    VBO cubeVBO(cubeVertices);
    cubeVAO.linkAttribs(cubeVBO, shader);

    VAO planeVAO;
    VBO planeVBO(planeVertices);
    planeVAO.linkAttribs(planeVBO, shader);

    VAO quadVAO;
    VBO quadVBO(quadVertices);
    quadVAO.linkAttribs(quadVBO, shader);

    VAO skyboxVAO;
    VBO skyboxVBO(skyboxVertices);
    skyboxVAO.linkAttribs(skyboxVBO, shader);

    Texture cubeTex("assets/textures/container.jpg");
    Texture floorTex("assets/textures/wall.jpg");

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);\

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    vector<std::string> faces = {
        "assets/textures/skybox/right.jpg",
        "assets/textures/skybox/left.jpg",
        "assets/textures/skybox/top.jpg",
        "assets/textures/skybox/bottom.jpg",
        "assets/textures/skybox/front.jpg",
        "assets/textures/skybox/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);



    // render loop
    // -----------
    while(!window.shouldClose())
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window.window);

        // render
        // ------
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST);

        window.setClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        window.clear();

        glDepthMask(GL_FALSE);
        skyboxShader.use();
        skyboxShader.setMat4("view", glm::mat4(glm::mat3(camera.getViewMatrix())));
        skyboxShader.setMat4("projection", glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f));
        skyboxVAO.bind();
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);




        shader.use();
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        // cubes
        cubeVAO.bind();
        glActiveTexture(GL_TEXTURE0);
        cubeTex.bind();	
        glm::mat4 model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // floor
        planeVAO.bind();
        floorTex.bind();
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);


        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        window.setClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        window.clear();

        screenShader.use();
        quadVAO.bind();
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        window.swapBuffers();
        window.pollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    cubeVAO.del();
    cubeVBO.del();
    planeVAO.del();
    planeVBO.del();
    quadVAO.del();
    quadVBO.del();
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &framebuffer);

    glfwTerminate();
    return 0;
}

unsigned int loadCubemap(vector<std::string> faces) {
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            stbi_image_free(data);
            throw std::runtime_error("Failed to load cubmap texture");
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureId;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScroll(static_cast<float>(yoffset));
}