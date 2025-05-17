#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include <iostream>
#include <fstream>
#include <cmath>

#include "Shader.hpp"
#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "ImGUI.hpp"
#include "ApplicationState.hpp"
#include "Input.hpp"
#include "Window.hpp"

int main() {
    ApplicationState state;
    Window window;
    if (window.setupWindow(state.window) != 0) return -1;

    ImGUI imGUI(state.window);
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    Renderer renderer;
    Input input;

    while (!glfwWindowShouldClose(state.window)) {
        float currentTime = glfwGetTime();
        state.deltaTime = currentTime - state.lastTime;
        state.lastTime = currentTime;

        input.processInput(state.window, state);
        renderer.Clear();

        if(state.guiEnable) { imGUI.drawGUI(io.DeltaTime, io.Framerate); };

        glfwSwapBuffers(state.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}