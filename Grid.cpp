#include <vector>
#include <iostream>
#include "Grid.h"

Grid::Grid(int width, int height, int neighbors_count) : width(width), height(height),
                                                         neighbors_count(neighbors_count) {
    data = new bool[width * height];
    for (int i = 0; i < width * height; i++) {
        data[i] = true;
    }
}

int Grid::GetWidth() const {
    return width;
}

int Grid::GetHeight() const {
    return height;
}

bool Grid::IsCellTraversable(int cell) const {
    return data[cell];
}

void Grid::SetCell(int i, int j, bool value) {
    data[i * width + j] = value;
}

// Return indices of neighbors of the cell
std::vector<int> Grid::GetNeighbors(int cell) const {
    std::vector<int> neighbors;

    // North
    if (cell - width >= 0) {
        neighbors.push_back(cell - width);
    }

    // South
    if (cell + width < width * height) {
        neighbors.push_back(cell + width);
    }

    // West
    if (cell % width != 0) {
        neighbors.push_back(cell - 1);
    }

    // East
    if ((cell + 1) % width != 0) {
        neighbors.push_back(cell + 1);
    }

    if (neighbors_count == 8) {
        // North-West
        if (cell - width >= 0 && cell % width != 0) {
            neighbors.push_back(cell - width - 1);
        }

        // North-East
        if (cell - width >= 0 && (cell + 1) % width != 0) {
            neighbors.push_back(cell - width + 1);
        }

        // South-West
        if (cell + width < width * height && cell % width != 0) {
            neighbors.push_back(cell + width - 1);
        }

        // South-East
        if (cell + width < width * height && (cell + 1) % width != 0) {
            neighbors.push_back(cell + width + 1);
        }
    }

    return neighbors;
}

int Grid::GetCellIndex(int i, int j) const {
    return i * width + j;
}

void Grid::VisualisePath(const std::vector<int> &path) const {
    for (int i = 0; i < width * height; i++) {
        if (std::find(path.begin(), path.end(), i) != path.end()) {
            std::cout << "X";
        } else if (data[i]) {
            std::cout << "_";
        } else {
            std::cout << "#";
        }
        if (i % width == width - 1) {
            std::cout << std::endl;
        }
    }
}
