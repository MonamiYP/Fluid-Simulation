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

    DrawQuad quad(renderer);

    int grid_width = 100;
    int grid_height = 100;
    FluidSolver fluid(grid_width, grid_height, 0.001, 0.001, 0.001);
    Texture densityTex(grid_width, grid_height);

    while (!glfwWindowShouldClose(app_state.window)) {
        float currentTime = glfwGetTime();
        app_state.deltaTime = currentTime - app_state.lastTime;
        app_state.lastTime = currentTime;
        input.processInput(app_state.window, app_state);

        if (input.isMousePressed()) {
            glm::vec2 gridPos = input.getMouseGridPosition(&window, grid_width, grid_height);
            fluid.addDensitySource(gridPos, 3, 1);
            fluid.addVelocitySource(gridPos, input.getMouseDelta(), 1);
        }

        fluid.step();
        densityTex.UploadData(fluid.getDensitiesWithoutBoundaries());

        renderer.Clear();
        quad.draw(densityTex);

        if(app_state.guiEnable) { imGUI.drawGUI(io.DeltaTime, io.Framerate); };

        glfwSwapBuffers(app_state.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}