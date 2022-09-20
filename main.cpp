#include <iostream>
#include <string>
#include "tinyxml2.h"
#include "Grid.h"
#include <tuple>
#include <queue>
#include <map>
#include "Pathfinder.h"

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

    auto pathfinder = Pathfinder(taskConfiguration, grid);
    auto path = pathfinder.FindPath();

    grid.VisualisePath(path);

    return 0;
}

