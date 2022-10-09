#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "Grid.h"

struct TaskConfiguration {
    int start_i;
    int start_j;

    int goal_i;
    int goal_j;

    std::string metricType;
    std::string algorithm;
    int connections;
    float hweight;
};

class Pathfinder {
   private:
    Grid grid;
    TaskConfiguration taskConfiguration;

   public:
    Pathfinder(TaskConfiguration taskConfiguration, const Grid &grid);

    std::vector<int> FindPath();

   private:
    /// Traverse the grid and construct all parent-child relationships
    std::map<int, int> TraverseGrid();

    /// Reconstruct the path from the parent-child relationships
    std::vector<int> ReconstructPath(std::map<int, int> parents);
};
