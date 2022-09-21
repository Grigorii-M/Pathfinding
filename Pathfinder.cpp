#include <queue>
#include <utility>
#include "Pathfinder.h"
#include <functional>

Pathfinder::Pathfinder(TaskConfiguration taskConfiguration, const Grid &grid) : grid(grid), taskConfiguration(
        std::move(taskConfiguration)) {}

std::vector<int> Pathfinder::FindPath() {
    auto parents = TraverseGrid();
    return ReconstructPath(parents);
}

std::map<int, int> Pathfinder::TraverseGrid() {
    int start = grid.GetCellIndex(taskConfiguration.start_i, taskConfiguration.start_j);
    int goal = grid.GetCellIndex(taskConfiguration.goal_i, taskConfiguration.goal_j);

    // Determine heuristic function
    std::function<double(int, int)> heuristicFunction;
    if (taskConfiguration.metricType == "Euclidean") {
        heuristicFunction = [&grid = this->grid](int cell1, int cell2) {
            auto [i1, j1] = grid.GetCellCoordinates(cell1);
            auto [i2, j2] = grid.GetCellCoordinates(cell2);

            return sqrt(pow(i1 - i2, 2) + pow(j1 - j2, 2));
        };
    } else if (taskConfiguration.metricType == "Octile") {
        heuristicFunction = [&grid = this->grid](int cell1, int cell2) {
            auto [i1, j1] = grid.GetCellCoordinates(cell1);
            auto [i2, j2] = grid.GetCellCoordinates(cell2);

            return std::max(abs(i1 - i2), abs(j1 - j2)) + (sqrt(2) - 1) * std::min(abs(i1 - i2), abs(j1 - j2));
        };
    } else { // Manhattan
        heuristicFunction = [&grid = this->grid](int cell1, int cell2) {
            auto [i1, j1] = grid.GetCellCoordinates(cell1);
            auto [i2, j2] = grid.GetCellCoordinates(cell2);

            return std::abs(i1 - i2) + std::abs(j1 - j2);
        };
    }

    // Determine the priority function
    int bfsOrder = 0;
    std::map<int, double> cellCosts;
    std::function<double(int)> priorityFunction;
    if (taskConfiguration.algorithm == "Dijkstra") {
        priorityFunction = [&cellCosts](int cell) { return cellCosts[cell]; };
    } else if (taskConfiguration.algorithm == "AStar") {
        priorityFunction =
                [&cellCosts, &goal, &hweight = this->taskConfiguration.hweight, &heuristicFunction](int cell) {
                    return cellCosts[cell] + hweight * heuristicFunction(cell, goal);
                };
    } else { // BFS
        priorityFunction = [&bfsOrder](int cell) { return bfsOrder++; };
    }

    auto comparator = [&](std::tuple<double, int> a, std::tuple<double, int> b) {
        return std::get<1>(a) > std::get<1>(b);
    };

    auto queue = std::priority_queue<std::tuple<int, double>, std::vector<std::tuple<int, double>>, decltype(comparator)>{comparator};

    queue.push(std::make_tuple(start, priorityFunction(start)));

    std::map<int, int> parents;
    parents[start] = -1;

    while (!queue.empty()) {
        int current = std::get<0>(queue.top());
        queue.pop();

        if (current == goal) {
            break;
        }

        for (int next: grid.GetNeighbors(current)) {
            double cost = cellCosts[current] + grid.Cost(current, next);
            if (cellCosts.find(next) == cellCosts.end() || cost < cellCosts[next]) {
                cellCosts[next] = cost;
            } else {
                continue;
            }

            if (grid.IsCellTraversableFrom(current, next) && parents.find(next) == parents.end()) {
                queue.push(std::make_tuple(next, priorityFunction(next)));
                parents[next] = current;
            }
        }
    }

    return parents;
}

std::vector<int> Pathfinder::ReconstructPath(std::map<int, int> parents) {
    std::vector<int> path;
    int current = grid.GetCellIndex(taskConfiguration.goal_i, taskConfiguration.goal_j);
    while (current != -1) {
        path.push_back(current);
        current = parents.at(current);
    }
    std::reverse(path.begin(), path.end());

    return path;
}
