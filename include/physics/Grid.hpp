#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

class Grid {
    private:
        int m_width, m_height;
        std::vector<glm::vec2> m_velocity;
        std::vector<float> m_density;
    
    public:
        Grid(int width, int height) : m_width(width), m_height(height), m_velocity(width * height, {0.0f, 0.0f}), m_density(width*height, 0.0f) {};
        ~Grid() {};

        float getWidth() { return m_width; }
        float getHeight() { return m_height; }
        
        std::vector<glm::vec2> getVelocities() { return m_velocity; };
        std::vector<float> getDensities() { return m_density; };
        std::vector<float> getDensitiesNormalised();
        glm::vec2 getVelocity(int x, int y) { return m_velocity[y * m_width + x]; };
        float getDensity(int x, int y) { return m_density[y * m_width + x]; };
};