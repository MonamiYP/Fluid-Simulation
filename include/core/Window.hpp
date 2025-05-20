#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
    private:
        int window_width = 200.0f;
        int window_height = 100.0f;

        int fb_width;
        int fb_height;

        int simulation_width;
        int simulation_height;

        void setupSimulationWindow(GLFWwindow*& window);

    public:
        int setupWindow(GLFWwindow*& window);
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        int getHeight() { return window_height; }
        int getWidth() { return window_width; }
        int getSimulationHeight() { return simulation_height; }
        int getSimulationWidth() { return simulation_width; }
};