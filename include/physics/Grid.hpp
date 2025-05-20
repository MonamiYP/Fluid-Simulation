#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

class Grid {
    private:
        int m_width, m_height;
        std::vector<float> m_velocity_x;
        std::vector<float> m_velocity_y;
        std::vector<float> m_density;
    
    public:
        Grid(int width, int height) : m_width(width), m_height(height), m_velocity_x((width+2) * (height+2), 0.0f), m_velocity_y((width+2) * (height+2), 0.0f), m_density((width+2)*(height+2), 0.0f) {};
        ~Grid() {};

        float getWidth() { return m_width; }
        float getHeight() { return m_height; }
        
        std::vector<float>& getVelocitiesX() { return m_velocity_x; };
        std::vector<float>& getVelocitiesY() { return m_velocity_y; };
        std::vector<float>& getDensities() { return m_density; };
        std::vector<float> getDensitiesWithoutBoundaries();
        // std::vector<float>& getDensitiesNormalised();

        void setVelocitiesX(std::vector<float>& new_velocity_x) { m_velocity_x = new_velocity_x; }
        void setVelocitiesY(std::vector<float>& new_velocity_y) { m_velocity_y = new_velocity_y; }
        void setDensities(std::vector<float>& new_density) { m_density = new_density; }
};