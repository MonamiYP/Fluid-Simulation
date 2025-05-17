#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
    private:
        const float WINDOW_WIDTH = 1200.0f;
        const float WINDOW_HEIGHT = 800.0f;

    public:
        int setupWindow(GLFWwindow*& window);
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};