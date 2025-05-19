#include "Grid.hpp"

std::vector<float> Grid::getDensitiesNormalised() {
    std::vector<float> normalized(m_density.size());
    float max_density = 0.0f;
    for (float density : m_density)
        if (density > max_density) max_density = density;
    if (max_density == 0) max_density = 1.0f; // avoid divide by zero

    for (size_t i = 0; i < m_density.size(); ++i) {
        normalized[i] = m_density[i] / max_density;
    }
    return normalized;
}