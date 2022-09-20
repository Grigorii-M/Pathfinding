#pragma once

#include <string>
#include <map>
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
    std::map<int, int> TraverseGrid();
    std::vector<int> ReconstructPath(std::map<int, int> parents);
};
