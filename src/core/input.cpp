#include "Input.hpp"

void Input::processInput(GLFWwindow* window, ApplicationState& state) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    toggleImGUI(window, state);

    m_mouseXPrev = m_mouseX;
    m_mouseYPrev = m_mouseY;

    glfwGetCursorPos(glfwGetCurrentContext(), &m_mouseX, &m_mouseY);
    m_mousePressed = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT);
}

glm::vec2 Input::getMouseGridPosition(Window* window) {
    int simWidth = window->getSimulationWidth();
    int simHeight = window->getSimulationHeight();
    int winWidth = window->getWidth();
    int winHeight = window->getHeight();

    float relativeX = m_mouseX * 2 - (winWidth - simWidth);
    float relativeY = m_mouseY * 2;

    relativeX = std::clamp(relativeX, 0.0f, float(simWidth));
    relativeY = std::clamp(relativeY, 0.0f, float(simHeight));
    relativeY = float(simHeight) - relativeY;

    return glm::vec2((int)relativeX, (int)relativeY);
}

glm::vec2 Input::getMouseDelta() {
    return glm::vec2((m_mouseX - m_mouseXPrev) * 100, (m_mouseY - m_mouseYPrev) * 100);
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