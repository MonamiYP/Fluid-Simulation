#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ImGUI {
    private:
        GLFWwindow* m_window;
        
    public:
        ImGUI(GLFWwindow* window);
        ~ImGUI();
        
        void drawGUI(float deltaTime, float fps);
        void drawSideWindow();
};