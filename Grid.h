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
    int GetWidth() const;
    int GetHeight() const;
    bool IsCellTraversable(int cell) const;
    void SetCell(int i, int j, bool value);
    std::vector<int> GetNeighbors(int cell) const;
    int GetCellIndex(int i, int j) const;
    void VisualisePath(const std::vector<int> &path) const;
};
