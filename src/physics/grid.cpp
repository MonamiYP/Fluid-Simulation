#include "Grid.hpp"

// std::vector<float>& Grid::getDensitiesNormalised() {
//     std::vector<float> normalized(m_density.size());
//     float max_density = 0.0f;
//     for (float density : m_density)
//         if (density > max_density) max_density = density;
//     if (max_density == 0) max_density = 1.0f; // avoid divide by zero

//     for (size_t i = 0; i < m_density.size(); ++i) {
//         normalized[i] = m_density[i] / max_density;
//     }
//     return normalized;
// }

std::vector<float> Grid::getDensitiesWithoutBoundaries() {
    const std::vector<float>& fullDensities = getDensities();
    std::vector<float> densities(m_width * m_height);

    for (int j = 0; j < m_height; j++) {
        for (int i = 0; i < m_width; i++) {
            // +1 offset to skip boundary layer
            int fullIndex = (i + 1) + (m_width + 2) * (j + 1);
            int coreIndex = i + m_width * j;
            densities[coreIndex] = fullDensities[fullIndex];
        }
    }
    return densities;
}