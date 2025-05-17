#pragma once

#include <GLFW/glfw3.h>
#include "ApplicationState.hpp"

class Input {
    private:

    public:
        void processInput(GLFWwindow* window, ApplicationState& state);
        void toggleImGUI(GLFWwindow* window, ApplicationState& state);
};