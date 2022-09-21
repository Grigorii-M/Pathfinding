#pragma once
#include <vector>

class Grid {
private:
    int width; // j
    int height; // i
    bool* data; // true - traversable, false - obstacle
    int neighbors_count;

public:
    Grid(int width, int height, int neighbors_count);
    [[nodiscard]] int GetWidth() const;
    [[nodiscard]] int GetHeight() const;
    [[nodiscard]] bool IsCellTraversable(int cell) const;
    void SetCell(int i, int j, bool value);
    [[nodiscard]] std::vector<int> GetNeighbors(int cell) const;
    [[nodiscard]] int GetCellIndex(int i, int j) const;
    [[nodiscard]] double Cost(int cell1, int cell2) const;
    /// Returns the cell's i and j coordinates
    [[nodiscard]] std::tuple<int, int> GetCellCoordinates(int cell) const;
};
