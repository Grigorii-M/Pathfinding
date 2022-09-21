#include <vector>
#include <iostream>
#include "Grid.h"

Grid::Grid(int width, int height, int neighborsCount) : width(width), height(height),
                                                         neighborsCount(neighborsCount) {
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

bool Grid::IsCellTraversableFrom(int curr, int next) const {
    bool destinationIsTraversable = data[next];
    if (!destinationIsTraversable) {
        return false;
    }

    int start_i = curr / width;
    int start_j = curr % width;

    int next_i = next / width;
    int next_j = next % width;

    if (neighborsCount == 8) {
        int di = next_i - start_i;
        int dj = next_j - start_j;

        bool isDiagonalPathOpen = data[curr + di * width] || data[curr + dj];

        return isDiagonalPathOpen;
    }

    return true;
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

    if (neighborsCount == 8) {
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

double Grid::Cost(int cell1, int cell2) const {
    int x1 = cell1 % width;
    int y1 = cell1 / width;

    int x2 = cell2 % width;
    int y2 = cell2 / width;

    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

std::tuple<int, int> Grid::GetCellCoordinates(int cell) const {
    return std::make_tuple(cell / width, cell % width);
}
