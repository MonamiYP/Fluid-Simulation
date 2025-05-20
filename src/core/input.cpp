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

glm::vec2 Input::getMouseGridPosition(Window* window, int grid_width, int grid_height) {
    int simWidth = window->getSimulationWidth();
    int simHeight = window->getSimulationHeight();
    int fbWidth = window->getWidth();

    // Multiply by 2 (because framebuffer was 2 times larger than window, remove hard code in future)
    float mouseXGrid = 2 * m_mouseX;
    float mouseYGrid = 2 * m_mouseY;

    // Convert to simulation coordinates (y is flipped)
    mouseXGrid = mouseXGrid - (fbWidth - simWidth);
    mouseYGrid = simHeight - mouseYGrid;

    // Convert to grid coordinates
    mouseXGrid = mouseXGrid * grid_width / simWidth;
    mouseYGrid = mouseYGrid * grid_height / simHeight;

    // Clamp to screen
    mouseXGrid = std::clamp(mouseXGrid, 0.0f, (float)grid_width);
    mouseYGrid = std::clamp(mouseYGrid, 0.0f, (float)grid_height);

    return glm::vec2((int)mouseXGrid, (int)mouseYGrid);
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