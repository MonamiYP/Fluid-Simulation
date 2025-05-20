#include "ImGUI.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

ImGUI::ImGUI(GLFWwindow* window) : m_window(window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");  // match OpenGL context
}

ImGUI::~ImGUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGUI::drawGUI(ApplicationState* state, FluidSolver* solver) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    drawFluidSimGUI(state, solver);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGUI::drawFluidSimGUI(ApplicationState* state, FluidSolver* solver) {
    int window_width, window_height;
    glfwGetFramebufferSize(m_window, &window_width, &window_height);
    float guiWidth = window_width * 0.2;

    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({guiWidth/2, (float)window_height});

    ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    
    ImGui::Text("Control Panel");
    ImGui::Separator();
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Separator();
    
    ImGui::Text("Fluid parameters");
    ImGui::SliderFloat("Viscosity", &solver->m_viscosity, 0.0f, 1.0f);
    ImGui::SliderFloat("Diffusion", &solver->m_diffusion_coeff, 0.0f, 0.0001f, "%.6f");
    ImGui::Separator();

    ImGui::Text("Source");
    if (ImGui::Button(state->addDensitySource ? "Stop" : "Add density")) state->addDensitySource = !state->addDensitySource;
    if (ImGui::Button(state->addVelocitySource ? "Stop" : "Add velocity")) state->addVelocitySource = !state->addVelocitySource;
    ImGui::Text("Source strength");
    ImGui::SliderFloat("Density", &solver->m_density_source_strength, 0.0f, 2.0f);
    ImGui::SliderFloat("Velocity", &solver->m_velocity_source_strength, 0.0f, 100.0f);
    ImGui::Text("Fading rate");
    ImGui::SliderFloat("Fade Density", &solver->m_density_fade_rate, 0.98f, 1.0f);
    ImGui::SliderFloat("Fade Velocity", &solver->m_velocity_fade_rate, 0.98f, 1.0f);

    ImGui::Text("Velocity field");
    ImGui::Checkbox("Draw velocity arrows", &state->arrowsVisible);
    // Todo: pick between different velocity fields

    ImGui::Separator();

    ImGui::ColorEdit3("Color", state->color);

    ImGui::End();
}