#include "main_loop.hpp"
#include "shader.hpp"
#include "shader_sources.hpp"
#include "vao.hpp"

constexpr auto WND_W = 1200.0f;
constexpr auto WND_H = 800.0f;
constexpr auto TIMESTEP = 1.0f / 120.0f;

GLFWwindow* window;
glm::mat4 proj = glm::ortho(0.0f, WND_W, WND_H, 0.0f);

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
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

    VAO quad;
    quad.Load(quad_vertices);

    MainLoop(
        TIMESTEP,
        window,
        // Fixed
        [&]() {
        },
        // Render
        [&]() {
            glClear(GL_COLOR_BUFFER_BIT);
            ball_shader.Bind();
            // ball_shader.Set("uProj", glm::scale(glm::mat4(1.0f), glm::vec3(100.0f)) * proj);
            ball_shader.Set("uProj", glm::mat4(1));
            ball_shader.Set("uColor", glm::vec4(1.0f, 0.0f, 0.2f, 1.0f));
            quad.Draw();
        }
    );
}

