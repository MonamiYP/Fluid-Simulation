#pragma once

#include <vector>

#include "Grid.hpp"

class FluidSolver {
    private:
        Grid* m_grid;
        int m_width;
        int m_height;
        float m_dt;
        int m_size;

        float m_viscosity;
        float m_diffusion_coeff;

        std::vector<float> m_source;
        std::vector<float> m_density;
        std::vector<float> m_density_prev;

        std::vector<float> m_velocity_x;
        std::vector<float> m_velocity_y;
        std::vector<float> m_velocity_x_prev;
        std::vector<float> m_velocity_y_prev;

        inline int getIndex(int x, int y) { return x + (m_width+2) * y; };

        void diffuse(int boundary, std::vector<float>& x, std::vector<float>& prev_x);
        void advect(int boundary, std::vector<float>& d, std::vector<float>& prev_d, std::vector<float>& u, std::vector<float>& v);

        void setBoundary(int boundary, std::vector<float>& x);
        void project(std::vector<float>& u, std::vector<float>& v);

        void stepDensity();
        void stepVelocity();

    public:
        void addDensitySource(glm::vec2 location, int amount, int radius);
        void addVelocitySource(glm::vec2 location, glm::vec2 amount);
        FluidSolver(Grid* grid, float dt, float viscosity, float diffusion);
        void step();
};