#pragma once

struct ApplicationState {
    GLFWwindow* window = nullptr;
    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    bool guiEnable = true;
};