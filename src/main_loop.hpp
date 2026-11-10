#pragma once

void MainLoop(float timestep, GLFWwindow* window, std::function<void()> update, std::function<void()> render);

