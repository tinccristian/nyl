#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <glm/glm.hpp>

namespace nyl
{
    /**
     * @brief Uniform-grid spatial hash for broad-phase collision culling.
     *
     * Insert axis-aligned boxes keyed by an integer id, then query() returns the
     * ids whose cells overlap a query box. This replaces O(n^2) all-pairs scans:
     * a query only narrow-phase-tests the handful of nearby candidates.
     *
     * Header-only and inline so both the engine and game can use it without DLL
     * export concerns. Tile colliders (Phase 7) feed the same structure.
     */
    class Broadphase
    {
    public:
        explicit Broadphase(float cellSize = 128.0f) : m_cellSize(cellSize) {}

        void setCellSize(float cellSize)
        {
            m_cellSize = (cellSize > 0.0f) ? cellSize : 1.0f;
        }

        void clear() { m_cells.clear(); }

        void insert(int id, const glm::vec2& min, const glm::vec2& max)
        {
            const int x0 = cellCoord(min.x);
            const int y0 = cellCoord(min.y);
            const int x1 = cellCoord(max.x);
            const int y1 = cellCoord(max.y);
            for (int y = y0; y <= y1; ++y)
                for (int x = x0; x <= x1; ++x)
                    m_cells[key(x, y)].push_back(id);
        }

        /// Unique ids whose cells overlap the query box.
        std::vector<int> query(const glm::vec2& min, const glm::vec2& max) const
        {
            std::vector<int> result;
            const int x0 = cellCoord(min.x);
            const int y0 = cellCoord(min.y);
            const int x1 = cellCoord(max.x);
            const int y1 = cellCoord(max.y);
            for (int y = y0; y <= y1; ++y)
            {
                for (int x = x0; x <= x1; ++x)
                {
                    auto it = m_cells.find(key(x, y));
                    if (it == m_cells.end()) continue;
                    result.insert(result.end(), it->second.begin(), it->second.end());
                }
            }
            std::sort(result.begin(), result.end());
            result.erase(std::unique(result.begin(), result.end()), result.end());
            return result;
        }

    private:
        int cellCoord(float v) const
        {
            return static_cast<int>(std::floor(v / m_cellSize));
        }
        static std::int64_t key(int x, int y)
        {
            return (static_cast<std::int64_t>(static_cast<std::uint32_t>(x)) << 32)
                 | static_cast<std::uint32_t>(y);
        }

        float m_cellSize;
        std::unordered_map<std::int64_t, std::vector<int>> m_cells;
    };

} // namespace nyl
