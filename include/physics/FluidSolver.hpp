#pragma once

#include <glm/glm.hpp>
#include <vector>

class FluidSolver {
    private:
        std::vector<float> m_source;

        std::vector<float> m_density;
        std::vector<float> m_density_prev;

        std::vector<float> m_velocity_x;
        std::vector<float> m_velocity_y;
        std::vector<float> m_velocity_x_prev;
        std::vector<float> m_velocity_y_prev;

        inline int getIndex(int x, int y) { return x + (N+2) * y; };

        void diffuse(int boundary, std::vector<float>& x, std::vector<float>& prev_x, float dt, float coeff);
        void advect(int boundary, std::vector<float>& d, std::vector<float>& prev_d, std::vector<float>& u, std::vector<float>& v, float dt);

        void linearSolver(int boundary, std::vector<float>& x, std::vector<float>& prev_x, float a, float denom);
        void setBoundary(int boundary, std::vector<float>& x);
        void project(std::vector<float>& u, std::vector<float>& v, std::vector<float>& p, std::vector<float>& div);
        void fade();

        void stepDensity(float dt);
        void stepVelocity(float dt);

    public:
        int N;
        int m_size;

        float m_viscosity;
        float m_diffusion_coeff;

        float m_density_fade_rate;
        float m_velocity_fade_rate;

        float m_density_source_strength;
        float m_velocity_source_strength;

        FluidSolver(int N, float viscosity, float diffusion) : 
            N(N), m_size((N+2) * (N+2)),
            m_viscosity(viscosity), m_diffusion_coeff(diffusion),
            m_velocity_x((N+2) * (N+2), 5.0f), m_velocity_y((N+2) * (N+2), 1.0f),
            m_velocity_x_prev((N+2) * (N+2), 0.0f), m_velocity_y_prev((N+2) * (N+2), 0.0f), 
            m_density((N+2)*(N+2), 0.0f), m_density_prev((N+2)*(N+2), 0.0f),
            m_density_fade_rate(1.0f), m_velocity_fade_rate(1.0f),
            m_density_source_strength(1.0f), m_velocity_source_strength(1.0f) {};

        void addDensitySource(glm::vec2 location, int amount, int radius);
        void addVelocitySource(glm::vec2 location, glm::vec2 amount, int radius);
        void step(float dt) { stepVelocity(dt); stepDensity(dt); };

        float getWidth() { return N; }
        float getHeight() { return N; }
        
        std::vector<float>& getVelocitiesX() { return m_velocity_x; };
        std::vector<float>& getVelocitiesY() { return m_velocity_y; };
        std::vector<float>& getDensities() { return m_density; };
        std::vector<float> getDensitiesWithoutBoundaries();
};