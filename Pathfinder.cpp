#include <queue>
#include <utility>
#include "Pathfinder.h"

Pathfinder::Pathfinder(TaskConfiguration taskConfiguration, const Grid &grid): grid(grid), taskConfiguration(std::move(taskConfiguration)) {}

std::vector<int> Pathfinder::FindPath() {
    auto parents = TraverseGrid();
    return ReconstructPath(parents);
}

std::map<int, int> Pathfinder::TraverseGrid() {
    std::queue<int> queue;
    std::map<int, int> parents;

    int start = grid.GetCellIndex(taskConfiguration.start_i, taskConfiguration.start_j);
    int goal = grid.GetCellIndex(taskConfiguration.goal_i, taskConfiguration.goal_j);

    queue.push(start);
    parents[start] = -1;

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        if (current == goal) {
            break;
        }

        for (int next: grid.GetNeighbors(current)) {
            if (grid.IsCellTraversable(current) && parents.find(next) == parents.end()) {
                queue.push(next);
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
