#pragma once

bool MainLoop(float timestep, GLFWwindow* window, std::function<bool()> update, std::function<void()> render);

