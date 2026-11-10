#include "main_loop.hpp"

void MainLoop(float fixed_timestep, GLFWwindow* window, std::function<void()> update, std::function<void()> render) {
    using clock = std::chrono::high_resolution_clock;

    auto previous_time = clock::now();
    float accumulator = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        auto current_time = clock::now();
        std::chrono::duration<float> frame_duration = current_time - previous_time;
        previous_time = current_time;

        float frame_time = frame_duration.count();

        if (frame_time > 0.25f) {
            frame_time = 0.25f;
        }

        accumulator += frame_time;

        glfwPollEvents();
        while (accumulator >= fixed_timestep) {
            update();
            accumulator -= fixed_timestep;
        }

        // Render frame
        render();
        glfwSwapBuffers(window);
    }
}

