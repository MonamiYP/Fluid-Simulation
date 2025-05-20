#pragma once

#include <glm/glm.hpp>
#include <vector>

class FluidSolver {
    private:
        int m_width;
        int m_height;
        int m_size;

        float m_dt;
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
        FluidSolver(int width, int height, float dt, float viscosity, float diffusion) : m_dt(dt), 
            m_width(width), m_height(height), m_size((width+2) * (height+2)),
            m_viscosity(viscosity), m_diffusion_coeff(diffusion),
            m_velocity_x((width+2) * (height+2), 0.0f), m_velocity_y((width+2) * (height+2), 0.0f),
            m_velocity_x_prev((width+2) * (height+2), 0.0f), m_velocity_y_prev((width+2) * (height+2), 0.0f), 
            m_density((width+2)*(height+2), 0.0f), m_density_prev((width+2)*(height+2), 0.0f) {};

        void addDensitySource(glm::vec2 location, int amount, int radius);
        void addVelocitySource(glm::vec2 location, glm::vec2 amount);
        void step();

        float getWidth() { return m_width; }
        float getHeight() { return m_height; }
        
        std::vector<float>& getVelocitiesX() { return m_velocity_x; };
        std::vector<float>& getVelocitiesY() { return m_velocity_y; };
        std::vector<float>& getDensities() { return m_density; };
        std::vector<float> getDensitiesWithoutBoundaries();
};