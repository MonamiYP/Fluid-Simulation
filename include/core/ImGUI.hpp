#pragma once

#include <GLFW/glfw3.h>

class ImGUI {
    private:
        
    public:
        ImGUI(GLFWwindow* window);
        ~ImGUI();
        
        void drawGUI(float deltaTime, float fps);
};