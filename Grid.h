#pragma once
#include <vector>

class Grid {
private:
    int width; // j
    int height; // i
    bool* data; // true - traversable, false - obstacle
    int neighbors_count; // how many neighbors each cell has

public:
    Grid(int width, int height, int neighbors_count);
    [[nodiscard]] int GetWidth() const;
    [[nodiscard]] int GetHeight() const;
    [[nodiscard]] bool IsCellTraversable(int cell) const;
    void SetCell(int i, int j, bool value);
    [[nodiscard]] std::vector<int> GetNeighbors(int cell) const;
    /// Returns the index of the cell in the data array
    [[nodiscard]] int GetCellIndex(int i, int j) const;
    /// Returns cost of movement between two cells
    [[nodiscard]] double Cost(int cell1, int cell2) const;
    /// Returns the cell's i and j coordinates
    [[nodiscard]] std::tuple<int, int> GetCellCoordinates(int cell) const;
};
