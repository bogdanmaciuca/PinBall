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
}

void DestroyGL() {
    glfwTerminate();
}

int main() {
    InitGL();

    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);

    Shader ball_shader;
    ball_shader.Load(BALL_VS, BALL_FS);

    std::array<Vertex, 6> quad_vertices = {
        Vertex{ .pos = glm::vec2(-0.5f, -0.5f) },
        Vertex{ .pos = glm::vec2(+0.5f, -0.5f) },
        Vertex{ .pos = glm::vec2(+0.5f, +0.5f) },
        Vertex{ .pos = glm::vec2(-0.5f, -0.5f) },
        Vertex{ .pos = glm::vec2(+0.5f, +0.5f) },
        Vertex{ .pos = glm::vec2(-0.5f, +0.5f) },
    };

    Object ball(ball_shader);
    ball.Load(quad_vertices);
    ball.pos = glm::vec2(200.0f, 200.0f);
    ball.size = glm::vec2(300.0f, 150.0f);

    MainLoop(
        TIMESTEP,
        window,
        // Fixed
        [&]() {
        },
        // Render
        [&]() {
            glClear(GL_COLOR_BUFFER_BIT);
            ball.Draw();
        }
    );
}

