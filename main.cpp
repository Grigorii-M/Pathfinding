#include <iostream>
#include <string>
#include "tinyxml2.h"
#include "Grid.h"
#include <tuple>
#include "Pathfinder.h"
#include <utility>

#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"

/// Parse the XML file and return the task configuration and the grid
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
    int i = 0;
    for (auto row = map->FirstChildElement(); row != nullptr; row = row->NextSiblingElement(), i++) {
        std::string values = row->GetText();
        int j = 0;
        for (char value: values) {
            if (value == ' ') {
                continue;
            } else {
                grid.SetCell(i, j, value != '1');
            }
            j++;
        }
    }

    return std::make_tuple(taskConfiguration, grid);
}

/// This class is used to visualize the grid and the path
class Application : public olc::PixelGameEngine {
private:
    Pathfinder pathfinder;
    Grid grid;
    int start_i;
    int start_j;
    int goal_i;
    int goal_j;

public:
    Application(Pathfinder pathfinder, Grid grid, int start_i, int start_j, int goal_i, int goal_j) : pathfinder(
            std::move(pathfinder)), grid(grid), start_i(start_i), start_j(start_j), goal_i(goal_i), goal_j(goal_j) {
        sAppName = "Pathfinder";
    }

public:
    bool OnUserCreate() override {
        Clear(olc::BLACK);
        for (int i = 0; i < grid.GetHeight(); i++) {
            for (int j = 0; j < grid.GetWidth(); j++) {
                if (grid.IsCellTraversable(grid.GetCellIndex(i, j))) {
                    Draw(j, i, olc::GREY);
                } else {
                    Draw(j, i, olc::RED);
                }

                if (i == start_i && j == start_j) {
                    Draw(j, i, olc::MAGENTA);
                } else if (i == goal_i && j == goal_j) {
                    Draw(j, i, olc::CYAN);
                }
            }
        }
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (GetKey(olc::Key::ENTER).bPressed) {
            auto path = pathfinder.FindPath();

            Clear(olc::BLACK);
            for (int i = 0; i < grid.GetHeight(); i++) {
                for (int j = 0; j < grid.GetWidth(); j++) {
                    if (std::find(path.begin(), path.end(), grid.GetCellIndex(i, j)) != path.end()) {
                        Draw(j, i, olc::GREEN);
                    } else if (grid.IsCellTraversable(grid.GetCellIndex(i, j))) {
                        Draw(j, i, olc::GREY);
                    } else {
                        Draw(j, i, olc::RED);
                    }

                    if (i == start_i && j == start_j) {
                        Draw(j, i, olc::MAGENTA);
                    } else if (i == goal_i && j == goal_j) {
                        Draw(j, i, olc::CYAN);
                    }
                }
            }
        }

        return true;
    }
};

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <document>" << std::endl;
        return 1;
    }

    auto data = ParseInput(argv);
    auto taskConfiguration = std::get<0>(data);
    auto grid = std::get<1>(data);

    // Print config to console
    std::cout << "Config" << std::endl;
    std::cout << "\tGrid: " << grid.GetWidth() << "x" << grid.GetHeight() << std::endl;
    std::cout << "\tStart: " << taskConfiguration.start_i << ", " << taskConfiguration.start_j << std::endl;
    std::cout << "\tGoal: " << taskConfiguration.goal_i << ", " << taskConfiguration.goal_j << std::endl;
    std::cout << "\tMetric type: " << taskConfiguration.metricType << std::endl;
    std::cout << "\tAlgorithm: " << taskConfiguration.algorithm << std::endl;
    std::cout << "\tConnections: " << taskConfiguration.connections << std::endl;
    std::cout << "\tHweight: " << taskConfiguration.hweight << std::endl << std::endl;

    Pathfinder pathfinder(taskConfiguration, grid);

    Application app(pathfinder, grid, taskConfiguration.start_i, taskConfiguration.start_j, taskConfiguration.goal_i,
                    taskConfiguration.goal_j);
    if (app.Construct(grid.GetWidth(), grid.GetHeight(), 5, 5))
        app.Start();

    return 0;
}

