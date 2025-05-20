#pragma once

#include <GLFW/glfw3.h>

struct ApplicationState {
    GLFWwindow* window = nullptr;
    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    bool guiEnable = true;

    // Simulation specific states
    bool simulationPaused = false;
    bool addDensitySource = false;
    bool addVelocitySource = false;
    bool arrowsVisible = false;
    float color[3] = {1.0f, 1.0f, 1.0f};
};