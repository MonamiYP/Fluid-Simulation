#include "Input.hpp"

void Input::processInput(GLFWwindow* window, ApplicationState& state) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    toggleImGUI(window, state);
}

void Input::toggleImGUI(GLFWwindow* window, ApplicationState& state) {
    static bool g_pressed_prev = false;
    bool g_pressed = glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS;

    if (g_pressed && !g_pressed_prev) {
        state.guiEnable = !state.guiEnable;
        if (state.guiEnable) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    g_pressed_prev = g_pressed;
}