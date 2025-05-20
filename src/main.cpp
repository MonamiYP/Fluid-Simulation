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

    Texture densityTex(window.getSimulationWidth(), window.getSimulationHeight());

    FluidSolver fluid(window.getSimulationWidth(), window.getSimulationHeight(), 0.001, 0.001, 0.1);

    while (!glfwWindowShouldClose(app_state.window)) {
        float currentTime = glfwGetTime();
        app_state.deltaTime = currentTime - app_state.lastTime;
        app_state.lastTime = currentTime;
        input.processInput(app_state.window, app_state);
        if (input.isMousePressed()) {
            glm::vec2 gridPos = input.getMouseGridPosition(&window);
            fluid.addDensitySource(gridPos, 5, 4);
            fluid.addVelocitySource(gridPos, input.getMouseDelta());
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