#include "FluidSolver.hpp"

#include <iostream>

FluidSolver::FluidSolver(Grid* grid, float dt, float viscosity, float diffusion) : m_grid(grid), m_dt(dt) {
    m_viscosity = viscosity;
    m_diffusion_coeff = diffusion;

    m_width = m_grid->getWidth();
    m_height = m_grid->getHeight();
    m_size = (m_width+2) * (m_height+2);

    m_density = m_grid->getDensities();
    m_density_prev = std::vector<float>(m_density);
    m_source = std::vector<float>(m_size, 0.0f);  // Tempprary, add source dynamically in the future
    m_velocity_x = m_grid->getVelocitiesX();
    m_velocity_y = m_grid->getVelocitiesY();
    m_velocity_x_prev = m_velocity_x;
    m_velocity_y_prev = m_velocity_y;
}

void FluidSolver::addDensitySource(glm::vec2 location, int amount, int radius) {
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            int xPos = location.x + dx;
            int yPos = location.y + dy;

            if (xPos >= 0 && xPos <= m_width+1 && yPos >= 0 && yPos <= m_height+1) {
                float dist = sqrt(dx*dx + dy*dy);
                if (dist <= radius) {
                    int index = getIndex(xPos, yPos);
                    float falloff = 1.0f - (dist / radius);
                    m_density[index] += amount * falloff;
                }
            }
        }
    }
}

void FluidSolver::addVelocitySource(glm::vec2 location, glm::vec2 amount) {
    int radius = 4;
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            int xPos = location.x + dx;
            int yPos = location.y + dy;

            if (xPos >= 0 && xPos <= m_width+1 && yPos >= 0 && yPos <= m_height+1) {
                float dist = sqrt(dx*dx + dy*dy);
                if (dist <= radius) {
                    int index = getIndex(xPos, yPos);
                    float falloff = 1.0f - (dist / radius);
                    m_velocity_x[index] += amount.x * falloff;
                    m_velocity_y[index] += amount.y * falloff;
                }
            }
        }
    }
    
}

void FluidSolver::diffuse(int boundary, std::vector<float>& x, std::vector<float>& prev_x) {
    /*
        Uses Gauss-Seidel relaxation to solve the linear system that arises from the backward‑Euler discretisation of the diffusion term
    */
    float a = m_dt * m_diffusion_coeff * m_width * m_height;
    const unsigned int iterations = 5;
    float div = 1.0 / (1.0 + 4.0 * a);

    for (unsigned int k = 0; k < iterations; k++) {
        for (unsigned int i = 1; i <= m_width; i++) {
            for (unsigned int j = 1; j <= m_height; j++) {
                x[getIndex(i, j)] = div * (prev_x[getIndex(i, j)] + a * (x[getIndex(i-1, j)] + x[getIndex(i+1, j)] + x[getIndex(i, j-1)] + x[getIndex(i, j+1)]));
            }
        }

        setBoundary(boundary, x);
    }
}

void FluidSolver::advect(int boundary, std::vector<float>& d, std::vector<float>& prev_d, std::vector<float>& u, std::vector<float>& v) {
    /*
        Makes density follow a given velocity field
        Density at each grid cell is updated by tracing backwards through the velocity field to find where the fluid at that cell came from in the previous timestep, then sampling the old density value there
    */

    int i0, j0, i1, j1; 
    float x, y, s0, t0, s1, t1; 

    // Convert velocity units to grid cell units
    float dtx = m_dt * m_width;
    float dty = m_dt * m_height;

    for (unsigned int i = 1 ; i <= m_width ; i++) {
        for (unsigned int j = 1 ; j <= m_height ; j++) {
            // Backtracing: move backwards along velocity vector and find grid coordinates
            x = i - dtx * u[getIndex(i,j)];
            y = j - dty * v[getIndex(i,j)];

            // Clamp x and y to stay within grid bounds
            if (x < 0.5) x = 0.5;
            if (x > m_width + 0.5) x = m_width + 0.5;
            if (y < 0.5) y = 0.5;
            if (y > m_height + 0.5) y = m_height + 0.5;

            // Find index of the grid coordinates surrounding x and y
            i0 = (int)x;
            i1 = i0 + 1;
            j0 = (int)y;
            j1 = j0 + 1;

            // Interpolation weights
            s1 = x - i0;
            s0 = 1 - s1;
            t1 = y - j0;
            t0 = 1 - t1;

            // Interpolation
            d[getIndex(i,j)] = s0 * (t0 * prev_d[getIndex(i0,j0)] + t1 * prev_d[getIndex(i0,j1)])
                             + s1 * (t0 * prev_d[getIndex(i1,j0)] + t1 * prev_d[getIndex(i1,j1)]);

        }
    }

    setBoundary(boundary, d);
}

void FluidSolver::setBoundary(int boundary, std::vector<float>& x) {
    /*
        Keep fluid from leaking out of box, mirror every velocity in cells in the layer next to outer layer
    */

    // Edges
    for (unsigned int j = 1 ; j <= m_height ; j++) {
        x[getIndex(0,j)] = boundary==1 ? -x[getIndex(1,j)] : x[getIndex(1,j)];
        x[getIndex(m_width+1,j)] = boundary==1 ? -x[getIndex(m_width,j)] : x[getIndex(m_width,j)];
    }
    for (unsigned int i = 1 ; i <= m_width ; i++) {
        x[getIndex(i,0)] = boundary==2 ? -x[getIndex(i,1)] : x[getIndex(i,1)];
        x[getIndex(i, m_height+1)] = boundary==2 ? -x[getIndex(i,m_height)] : x[getIndex(i,m_height)];
    }

    // Corner cases (average of 2 neighbours)
    x[getIndex(0,0)] = 0.5 * (x[getIndex(1,0)] + x[getIndex(0,1)]);
    x[getIndex(0,m_height+1)] = 0.5 * (x[getIndex(1,m_height+1)] + x[getIndex(0,m_height)]);
    x[getIndex(m_width+1,0)] = 0.5 * (x[getIndex(m_width,0)] + x[getIndex(m_width+1,1)]);
    x[getIndex(m_width+1,m_height+1)] = 0.5 * (x[getIndex(m_width,m_height+1)] + x[getIndex(m_width+1,m_height)]);
}

void FluidSolver::project(std::vector<float>& u, std::vector<float>& v) {
    /*
        Other steps in solver do not conserve mass
        Use Helmholtz decomposition, where every velocity field is the sum of a mass conserving field and a gradient field
    */

    float h_x = 1.0/m_width;
    float h_y = 1.0/m_height; 

    std::vector<float> div(m_size, 0.0f);
    std::vector<float> p(m_size, 0.0f);
    
    // 1. Compute divergence of provisional velocity field
    for (unsigned int i = 1 ; i <= m_width; i++ ) {
        for (unsigned int j = 1 ; j <= m_height; j++ ) {
            div[getIndex(i,j)] = (u[getIndex(i-1,j)]-u[getIndex(i+1,j)])/(2*h_x) + (v[getIndex(i,j-1)]-v[getIndex(i,j+1)])/(2*h_y);
            p[getIndex(i,j)] = 0;
        }
    }
    setBoundary(0, div);
    setBoundary(0, p);


    // 2. Solve Poisson equation for pressure
    float h = 1/(h_x*h_x) + 1/(h_y*h_y);
    const unsigned int iterations = 5;
    for (unsigned int k = 0 ; k < iterations; k++) {
        for (unsigned int i = 1 ; i <= m_width; i++ ) {
            for (unsigned int j = 1 ; j <= m_height; j++ ) {
                p[getIndex(i,j)] = (div[getIndex(i,j)]/h + (p[getIndex(i-1,j)]+p[getIndex(i+1,j)])/(h_x*h_x) + (p[getIndex(i,j-1)]+p[getIndex(i,j+1)])/(h_y*h_y)) / (2*h);
            }
        }
        setBoundary(0, p);
    }

    // 3. Project velocity field by subtracting pressure gradient
    for (unsigned int i = 1 ; i <= m_width; i++ ) {
        for (unsigned int j = 1 ; j <= m_height; j++ ) {
            u[getIndex(i,j)] -= 0.5 * (p[getIndex(i+1,j)]-p[getIndex(i-1,j)])/ h_x;
            v[getIndex(i,j)] -= 0.5 * (p[getIndex(i,j+1)]-p[getIndex(i,j-1)])/ h_y;
        }
    }
    setBoundary(1, u);
    setBoundary(2, v);
}

void FluidSolver::step() {
    stepDensity();
    stepVelocity();
    m_grid->setDensities(m_density);
}

void FluidSolver::stepDensity() {
    std::swap(m_density, m_density_prev);
    diffuse(0, m_density, m_density_prev);
    std::swap(m_density, m_density_prev);
    advect(0, m_density, m_density_prev, m_velocity_x, m_velocity_y);
}

void FluidSolver::stepVelocity() {
    std::swap(m_velocity_x, m_velocity_x_prev);
    std::swap(m_velocity_y, m_velocity_y_prev);
    diffuse(1, m_velocity_x, m_velocity_x_prev);
    diffuse(2, m_velocity_y, m_velocity_y_prev);
    std::swap(m_velocity_x, m_velocity_x_prev);
    std::swap(m_velocity_y, m_velocity_y_prev);
    project(m_velocity_x, m_velocity_y);
    std::swap(m_velocity_x, m_velocity_x_prev);
    std::swap(m_velocity_y, m_velocity_y_prev);
    advect(1, m_velocity_x, m_velocity_x_prev, m_velocity_x_prev, m_velocity_y_prev);
    advect(2, m_velocity_y, m_velocity_y_prev, m_velocity_x_prev, m_velocity_y_prev);
    project(m_velocity_x, m_velocity_y);
}