#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ApplicationState.hpp"
#include "Window.hpp"

class Input {
    private:
        double m_mouseX, m_mouseY;
        double m_mouseXPrev, m_mouseYPrev;
        bool m_mousePressed = false;
    public:
        void processInput(GLFWwindow* window, ApplicationState& state);
        void toggleImGUI(GLFWwindow* window, ApplicationState& state);

        glm::vec2 getMouseGridPosition(Window* window, int grid_width, int grid_height);
        glm::vec2 getMouseDelta();
        bool isMousePressed() { return m_mousePressed; };
};