#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include <iostream>
#include <fstream>
#include <cmath>

#include "ApplicationState.hpp"
#include "Window.hpp"
#include "ImGUI.hpp"
#include "Renderer.hpp"
#include "Input.hpp"
#include "DrawQuad.hpp"
#include "Grid.hpp"
#include "Texture.hpp"
#include "FluidSolver.hpp"

int main() {
    ApplicationState app_state;
    Window window;
    Renderer renderer;
    Input input;

    if (window.setupWindow(app_state.window) != 0) return -1;
    ImGUI imGUI(app_state.window);
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    Grid grid(window.getSimulationWidth(), window.getSimulationHeight());
    DrawQuad quad(renderer);

    Texture densityTex(grid.getWidth(), grid.getHeight());
    densityTex.UploadData(grid.getDensitiesNormalised());

    FluidSolver fluid(&grid, 0.1, 0.1, 0.1);

    while (!glfwWindowShouldClose(app_state.window)) {
        float currentTime = glfwGetTime();
        app_state.deltaTime = currentTime - app_state.lastTime;
        app_state.lastTime = currentTime;
        input.processInput(app_state.window, app_state);

        renderer.Clear();
        quad.draw(densityTex);

        if(app_state.guiEnable) { imGUI.drawGUI(io.DeltaTime, io.Framerate); };

        glfwSwapBuffers(app_state.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}