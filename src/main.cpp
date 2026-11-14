#include "globals.hpp"
#include "main_loop.hpp"
#include "shader.hpp"
#include "shader_sources.hpp"
#include "object.hpp"
#include "world.hpp"

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

    World world;

    MainLoop(
        TIMESTEP,
        window,
        // Fixed
        [&]() {
            world.UpdateModels();
            world.UpdatePhysics();
        },
        // Render
        [&]() {
            glClear(GL_COLOR_BUFFER_BIT);
            world.Render();
        }
    );
}

