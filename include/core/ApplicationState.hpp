#pragma once

struct ApplicationState {
    GLFWwindow* window = nullptr;
    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    bool guiEnable = true;

    // Simulation specific states
    float viscocity = 0.1f;
};