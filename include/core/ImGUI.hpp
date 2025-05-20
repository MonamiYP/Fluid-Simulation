#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ApplicationState.hpp"
#include "FluidSolver.hpp"

class ImGUI {
    private:
        GLFWwindow* m_window;
        
    public:
        ImGUI(GLFWwindow* window);
        ~ImGUI();
        
        void drawGUI(ApplicationState* state, FluidSolver* solver);
        void drawFluidSimGUI(ApplicationState* state, FluidSolver* solver);
};