#include "Window.hpp"

int Window::setupWindow(GLFWwindow*& window) {
    // setup GLFW
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(window_width, window_height, "Fluid Sim", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    // setup GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    setupSimulationWindow(window);
    
    return 0;
}

void Window::setupSimulationWindow(GLFWwindow*& window) {
    glfwGetFramebufferSize(window, &window_width, &window_height);
    simulation_width = window_width * 0.2;
    simulation_height = window_height;
    glViewport(simulation_width, 0, window_width - simulation_width, simulation_height);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}