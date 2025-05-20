#include "FluidSolver.hpp"

#include <iostream>

void FluidSolver::addDensitySource(glm::vec2 location, int amount, int radius) {
    m_density[getIndex(location.x, location.y)] += amount * m_density_source_strength;
}

void FluidSolver::addVelocitySource(glm::vec2 location, glm::vec2 amount, int radius) {
    m_velocity_x[getIndex(location.x, location.y)] += amount.x * m_velocity_source_strength;
    m_velocity_y[getIndex(location.x, location.y)] -= amount.y * m_velocity_source_strength;
}

void FluidSolver::linearSolver(int boundary, std::vector<float>& x, std::vector<float>& prev_x, float a, float denom) {
    const unsigned int iterations = 20;
    for (unsigned int k = 0; k < iterations; k++) {
        for (unsigned int j = 1; j < N+1; j++) {
            for (unsigned int i = 1; i < N+1; i++) {
                x[getIndex(i, j)] = denom * (prev_x[getIndex(i, j)] 
                + a * (x[getIndex(i-1, j)] 
                + x[getIndex(i+1, j)] 
                + x[getIndex(i, j-1)] 
                + x[getIndex(i, j+1)]));
            }
        }

        setBoundary(boundary, x);
    }
}

void FluidSolver::diffuse(int boundary, std::vector<float>& x, std::vector<float>& prev_x, float dt, float coeff) {
    /*
        Uses Gauss-Seidel relaxation to solve the linear system that arises from the backward‑Euler discretisation of the diffusion term
    */
    float a = dt * coeff * N * N;
    linearSolver(boundary, x, prev_x, a, 1.0 / (1.0 + 4.0 * a));
}

void FluidSolver::advect(int boundary, std::vector<float>& d, std::vector<float>& prev_d, std::vector<float>& u, std::vector<float>& v, float dt) {
    /*
        Makes density follow a given velocity field
        Density at each grid cell is updated by tracing backwards through the velocity field to find where the fluid at that cell came from in the previous timestep, then sampling the old density value there
    */

    int i0, j0, i1, j1; 
    float x, y, s0, t0, s1, t1; 

    for (unsigned int i = 1; i < N+1; i++) {
        for (unsigned int j = 1; j < N+1; j++) {
            // Backtracing: move backwards along velocity vector and find grid coordinates
            x = (float)i - dt * (float)N * u[getIndex(i,j)];
            y = (float)j - dt * (float)N * v[getIndex(i,j)];

            // Clamp x and y to stay within grid bounds
            if (x < 0.5) x = 0.5;
            if (x > (float)N + 0.5) x = (float)N + 0.5;
            if (y < 0.5) y = 0.5;
            if (y > (float)N + 0.5) y = (float)N + 0.5;

            // Find index of the grid coordinates surrounding x and y
            i0 = floor(x);
            i1 = i0 + 1;
            j0 = floor(y);
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

        - Boundary = 0: for pressure and density fields, where the boundary value is same as neighbour
        - Boundary = 1: for velocity field in x direction
        - Boundary = 2: for velocity field in y direction

        Reminder that m_width and m_height is the actual fluid size and whole size of vectors including boundaries is (m_width+2) * (m_height+2)
    */

    // Edges
    for (unsigned int i = 1; i <= N; i++) {
        x[getIndex(i, 0)] = boundary == 2 ? -x[getIndex(i, 1)] : x[getIndex(i, 1)];
        x[getIndex(i, N+1)] = boundary == 2 ? -x[getIndex(i, N)] : x[getIndex(i, N)];
        x[getIndex(0, i)] = boundary == 1 ? -x[getIndex(1, i)] : x[getIndex(1, i)];
        x[getIndex(N+1, i)] = boundary == 1 ? -x[getIndex(N, i)] : x[getIndex(N, i)];
    }

    // Corner cases (average of 2 neighbours)
    x[getIndex(0,0)] = 0.5 * (x[getIndex(1,0)] + x[getIndex(0,1)]);
    x[getIndex(0,N+1)] = 0.5 * (x[getIndex(1,N+1)] + x[getIndex(0,N)]);
    x[getIndex(N+1,0)] = 0.5 * (x[getIndex(N,0)] + x[getIndex(N+1,1)]);
    x[getIndex(N+1,N+1)] = 0.5 * (x[getIndex(N+1,N)] + x[getIndex(N,N+1)]);
}

void FluidSolver::project(std::vector<float>& u, std::vector<float>& v, std::vector<float>& p, std::vector<float>& div) {
    /*
        Other steps in solver do not conserve mass
        Use Helmholtz decomposition, where every velocity field is the sum of a mass conserving field and a gradient field
    */
    
    // 1. Compute divergence of provisional velocity field
    for (unsigned int i = 1 ; i < N+1; i++ ) {
        for (unsigned int j = 1 ; j < N+1; j++ ) {
            div[getIndex(i,j)] = (u[getIndex(i-1,j)]-u[getIndex(i+1,j)] + v[getIndex(i,j-1)]-v[getIndex(i,j+1)]) / (2*N);
            p[getIndex(i,j)] = 0;
        }
    }
    setBoundary(0, div);
    setBoundary(0, p);


    // 2. Solve Poisson equation for pressure
    linearSolver(0, p, div, 1, 1/4);
    // for (unsigned int k = 0 ; k < iterations; k++) {
    //     for (unsigned int i = 1 ; i <= m_width; i++ ) {
    //         for (unsigned int j = 1 ; j <= m_height; j++ ) {
    //             p[getIndex(i,j)] = (divergence[getIndex(i,j)]/h + (p[getIndex(i-1,j)]+p[getIndex(i+1,j)])/(h_x*h_x) + (p[getIndex(i,j-1)]+p[getIndex(i,j+1)])/(h_y*h_y)) / (2*h);
    //         }
    //     }
    //     setBoundary(0, p);
    // }

    // 3. Project velocity field by subtracting pressure gradient
    for (unsigned int j = 1 ; j < N+1; j++ ) {
        for (unsigned int i = 1 ; i < N+1; i++ ) {
            u[getIndex(i,j)] -= 0.5 * (p[getIndex(i+1,j)]-p[getIndex(i-1,j)]) * N;
            v[getIndex(i,j)] -= 0.5 * (p[getIndex(i,j+1)]-p[getIndex(i,j-1)]) * N;
        }
    }
    setBoundary(1, u);
    setBoundary(2, v);
}

void FluidSolver::stepDensity(float dt) {
    std::swap(m_density, m_density_prev);
    diffuse(0, m_density, m_density_prev, dt, m_diffusion_coeff);
    std::swap(m_density, m_density_prev);
    advect(0, m_density, m_density_prev, m_velocity_x, m_velocity_y, dt);
    fade();
}

void FluidSolver::stepVelocity(float dt) {
    std::swap(m_velocity_x, m_velocity_x_prev);
    diffuse(1, m_velocity_x, m_velocity_x_prev, dt, m_viscosity);
    std::swap(m_velocity_y, m_velocity_y_prev);
    diffuse(2, m_velocity_y, m_velocity_y_prev, dt, m_viscosity);

    project(m_velocity_x, m_velocity_y, m_velocity_x_prev, m_velocity_y_prev);

    std::swap(m_velocity_x, m_velocity_x_prev);
    std::swap(m_velocity_y, m_velocity_y_prev);
    advect(1, m_velocity_x, m_velocity_x_prev, m_velocity_x_prev, m_velocity_y_prev, dt);
    advect(2, m_velocity_y, m_velocity_y_prev, m_velocity_x_prev, m_velocity_y_prev, dt);
    project(m_velocity_x, m_velocity_y, m_velocity_x_prev, m_velocity_y_prev);
}

void FluidSolver::fade() {
    for (size_t i = 0; i < m_density.size(); ++i) {
        m_density[i] *= m_density_fade_rate;
    }

    for (size_t i = 0; i < m_velocity_x.size(); ++i) {
        m_velocity_x[i] *= m_velocity_fade_rate;
    }
    for (size_t i = 0; i < m_velocity_y.size(); ++i) {
        m_velocity_y[i] *= m_velocity_fade_rate;
    }
}

std::vector<float> FluidSolver::getDensitiesWithoutBoundaries() {
    const std::vector<float>& fullDensities = getDensities();
    std::vector<float> densities(N * N);

    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            // +1 offset to skip boundary layer
            int fullIndex = (i + 1) + (N+2) * (j + 1);
            int coreIndex = i + N * j;
            densities[coreIndex] = fullDensities[fullIndex];
        }
    }
    
    return densities;
}