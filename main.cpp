#define OLC_PGE_APPLICATION
#include <iostream>
#include <string>
#include "tinyxml2.h"
#include "olcPixelGameEngine.h"

int main(int argc, char **argv) {
    // Get document name from an argument, parse it using tinyxml2 and print it
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <document>" << std::endl;
        return 1;
    }

    for (int i = 0; i < argc; i++) {
        std::cout << "Argument " << i << ": " << argv[i] << std::endl;
    }

    tinyxml2::XMLDocument doc;
    doc.LoadFile(argv[1]);
    auto root = doc.FirstChildElement();
    auto start = root->FirstChildElement();
    int start_x = start->IntAttribute("i");
    int start_y = start->IntAttribute("j");
    std::cout << "Start: " << start_x << ", " << start_y << std::endl;

    auto goal = start->NextSibling()->ToElement();
    int goal_x = goal->IntAttribute("i");
    int goal_y = goal->IntAttribute("j");
    std::cout << "Goal: " << goal_x << ", " << goal_y << std::endl;

    auto map = goal->NextSibling()->ToElement();


    auto options = map->NextSibling()->ToElement();
    std::string metrictype = options->Attribute("metrictype");
    std::string algorithm = options->Attribute("algorithm");
    std::string connections = options->Attribute("connections");
    std::string hweight = options->Attribute("hweight");
    std::cout << "Metric type: " << metrictype << std::endl;
    std::cout << "Algorithm: " << algorithm << std::endl;
    std::cout << "Connections: " << connections << std::endl;
    std::cout << "Hweight: " << hweight << std::endl;

    return 0;
}
