#include <iostream>
#include <string>
#include "tinyxml2.h"
#include "Grid.h"
#include <tuple>
#include <queue>
#include <map>

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

std::tuple<TaskConfiguration, Grid> ParseInput(char **argv) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile(argv[1]);
    auto root = doc.FirstChildElement();
    auto start = root->FirstChildElement();
    int start_i = start->IntAttribute("i");
    int start_j = start->IntAttribute("j");

    auto goal = start->NextSibling()->ToElement();
    int goal_i = goal->IntAttribute("i");
    int goal_j = goal->IntAttribute("j");

    auto map = goal->NextSibling()->ToElement();
    int width = map->IntAttribute("width");
    int height = map->IntAttribute("height");

    auto options = map->NextSibling()->ToElement();
    std::string metricType = options->Attribute("metrictype");
    std::string algorithm = options->Attribute("algorithm");
    int connections = std::stoi(options->Attribute("connections"));
    float hweight = std::stof(options->Attribute("hweight"));

    TaskConfiguration taskConfiguration = {
            start_i,
            start_j,
            goal_i,
            goal_j,
            metricType,
            algorithm,
            connections,
            hweight
    };

    // Create a grid, fill it with obstacles
    Grid grid(width, height, connections);
    int curi = 0;
    for (auto row = map->FirstChildElement(); row != nullptr; row = row->NextSiblingElement(), curi++) {
        std::string values = row->GetText();
        int curj = 0;
        for (char value: values) {
            if (value == ' ') {
                continue;
            } else {
                grid.SetCell(curi, curj, value != '1');
            }
            curj++;
        }
    }

    return std::make_tuple(taskConfiguration, grid);
}

std::map<int, int> TraverseGrid(const TaskConfiguration &taskConfiguration, const Grid &grid) {
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

std::vector<int> ReconstructPath(const std::map<int, int> &parents, int goal) {
    std::vector<int> path;
    int current = goal;
    while (current != -1) {
        path.push_back(current);
        current = parents.at(current);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<int> FindPath(const TaskConfiguration &taskConfiguration, const Grid &grid) {
    auto parents = TraverseGrid(taskConfiguration, grid);
    return ReconstructPath(parents, grid.GetCellIndex(taskConfiguration.goal_i, taskConfiguration.goal_j));
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <document>" << std::endl;
        return 1;
    }

    auto data = ParseInput(argv);
    auto taskConfiguration = std::get<0>(data);
    auto grid = std::get<1>(data);

    std::cout << "Config" << std::endl;
    std::cout << "\tGrid: " << grid.GetWidth() << "x" << grid.GetHeight() << std::endl;
    std::cout << "\tStart: " << taskConfiguration.start_i << ", " << taskConfiguration.start_j << std::endl;
    std::cout << "\tGoal: " << taskConfiguration.goal_i << ", " << taskConfiguration.goal_j << std::endl;
    std::cout << "\tMetric type: " << taskConfiguration.metricType << std::endl;
    std::cout << "\tAlgorithm: " << taskConfiguration.algorithm << std::endl;
    std::cout << "\tConnections: " << taskConfiguration.connections << std::endl;
    std::cout << "\tHweight: " << taskConfiguration.hweight << std::endl << std::endl;

    auto path = FindPath(std::get<0>(data), std::get<1>(data));
    grid.VisualisePath(path);

    return 0;
}

