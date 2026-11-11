#include "globals.hpp"
#include "main_loop.hpp"
#include "shader.hpp"
#include "shader_sources.hpp"
#include "object.hpp"

GLFWwindow* window;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    g_proj = glm::ortho(
        -(float)width / 2.0f, (float)width / 2.0f,
        (float)height / 2.0f, -(float)height / 2.0f
    );
}

void InitGL() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(WND_W, WND_H, "OpenGL App", nullptr, nullptr);
    assert(window != nullptr);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    g_proj = glm::ortho(
        -(float)WND_W / 2.0f, (float)WND_W / 2.0f,
        (float)WND_H / 2.0f, -(float)WND_H / 2.0f
    );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void DestroyGL() {
    glfwTerminate();
}

int main() {
    InitGL();

    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);

    Shader shader;
    shader.Load(BORDER_VS, BORDER_FS);

    std::array vertices = {
        // Flipper / Drain area (now at the top)
        Vertex{ .pos = glm::vec2(-0.23f,  0.5f) },  

        // Left wall
        Vertex{ .pos = glm::vec2(-0.37f,  0.19f) }, 
        Vertex{ .pos = glm::vec2(-0.37f, -0.21f) }, 

        // Arch / Dome (now at the bottom)
        Vertex{ .pos = glm::vec2(-0.26f, -0.41f) }, 
        Vertex{ .pos = glm::vec2( 0.0f,  -0.5f) },  // Bottom apex
        Vertex{ .pos = glm::vec2( 0.26f, -0.41f) }, 

        // Right wall
        Vertex{ .pos = glm::vec2( 0.37f, -0.21f) }, 
        Vertex{ .pos = glm::vec2( 0.37f,  0.19f) }, 

        // Closing back to start
        Vertex{ .pos = glm::vec2( 0.23f,  0.5f) },  
        Vertex{ .pos = glm::vec2(-0.23f,  0.5f) }   
    };

    Object ball(&shader);
    ball.Load(MakeLinePoly(vertices, 0.005f));
    ball.pos = glm::vec2(0.0f);
    ball.size = glm::vec2(WND_H * 0.9f);
    ball.color = glm::vec4(0.6f, 0.5f, 0.3f, 1.0f);

    MainLoop(
        TIMESTEP,
        window,
        // Fixed
        [&]() {
            ball.UpdateModel();
        },
        // Render
        [&]() {
            glClear(GL_COLOR_BUFFER_BIT);
            ball.Draw();
        }
    );
}

