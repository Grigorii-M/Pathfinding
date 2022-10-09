#include <iostream>
#include <string>
#include "tinyxml2.h"
#include "Grid.h"
#include <tuple>
#include "Pathfinder.h"
#include <utility>
#include "MazeGenerator.h"

#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"

std::tuple<TaskConfiguration, Grid> GenerateTask();

/// This class is used to visualize the grid and the path
class Application : public olc::PixelGameEngine {
private:
    Pathfinder pathfinder;
    Grid grid;
    int start_i;
    int start_j;
    int goal_i;
    int goal_j;
    bool mazeSolved;

public:
    Application(Pathfinder pathfinder, Grid grid, int start_i, int start_j, int goal_i, int goal_j) : pathfinder(
            std::move(pathfinder)), grid(grid), start_i(start_i), start_j(start_j), goal_i(goal_i), goal_j(goal_j) {
        sAppName = "Pathfinder";
        this->mazeSolved = false;
    }

public:
    bool OnUserCreate() override {
        Clear(olc::BLACK);
        for (int i = 0; i < grid.GetHeight(); i++) {
            for (int j = 0; j < grid.GetWidth(); j++) {
                if (grid.IsCellTraversable(grid.GetCellIndex(i, j))) {
                    Draw(j, i, olc::GREY);
                } else {
                    Draw(j, i, olc::DARK_GREY);
                }

                if (i == start_i && j == start_j) {
                    Draw(j, i, olc::MAGENTA);
                } else if (i == goal_i && j == goal_j) {
                    Draw(j, i, olc::YELLOW);
                }
            }
        }
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
            Clear(olc::BLACK);
      if (GetKey(olc::Key::ENTER).bPressed) {
        this->mazeSolved = true;
      }
        if (mazeSolved) {
            auto path = pathfinder.FindPath();
            for (int i = 0; i < grid.GetHeight(); i++) {
                for (int j = 0; j < grid.GetWidth(); j++) {
                if (grid.IsCellTraversable(grid.GetCellIndex(i, j))) {
                    Draw(j, i, olc::GREY);
                } else {
                    Draw(j, i, olc::DARK_GREY);
                }

                if (i == start_i && j == start_j) {
                    Draw(j, i, olc::MAGENTA);
                } else if (i == goal_i && j == goal_j) {
                    Draw(j, i, olc::YELLOW);
                }
                    if (std::find(path.begin(), path.end(), grid.GetCellIndex(i, j)) != path.end()) {
                        Draw(j, i, olc::GREEN);
                    }

                    if (i == start_i && j == start_j) {
                        Draw(j, i, olc::MAGENTA);
                    } else if (i == goal_i && j == goal_j) {
                        Draw(j, i, olc::CYAN);
                    }
                }
            }
        } else {
        for (int i = 0; i < grid.GetHeight(); i++) {
            for (int j = 0; j < grid.GetWidth(); j++) {
                if (grid.IsCellTraversable(grid.GetCellIndex(i, j))) {
                    Draw(j, i, olc::GREY);
                } else {
                    Draw(j, i, olc::DARK_GREY);
                }

                if (i == start_i && j == start_j) {
                    Draw(j, i, olc::MAGENTA);
                } else if (i == goal_i && j == goal_j) {
                    Draw(j, i, olc::YELLOW);
                }
            }
        }

        }

        if (GetKey(olc::Key::SPACE).bPressed) {
          auto task = GenerateTask();
          auto taskConfiguration = std::get<0>(task);
          
          this->start_i = taskConfiguration.start_i;
          this->start_j = taskConfiguration.start_j;
          this->goal_i = taskConfiguration.goal_i;
          this->goal_j = taskConfiguration.goal_j;

          this->grid = std::get<1>(task);

          this->pathfinder = Pathfinder(taskConfiguration, this->grid);
          mazeSolved = false;
        }

        return true;
    }
};

std::tuple<TaskConfiguration, Grid> GenerateTask() {
    const int WIDTH = 30;
    const int HEIGHT = 30;

    srand(time(0));
    auto maze = generateMaze(WIDTH, HEIGHT);
    auto mazeCells = convertMaze(maze, WIDTH, HEIGHT);
    Grid grid(WIDTH * 3, HEIGHT * 3, 4);

    for (int i = 0; i < mazeCells.size(); i++) {
      int x = i % (WIDTH * 3);
      int y = i / (WIDTH * 3);

      grid.SetCell(y, x, mazeCells[i]);
    }

    auto startEnd = GetStartEndPoints(mazeCells);
    int start_i = std::get<0>(startEnd) / (HEIGHT * 3);
    int start_j = std::get<0>(startEnd) % (HEIGHT * 3);

    int end_i = std::get<1>(startEnd) / (HEIGHT * 3);
    int end_j = std::get<1>(startEnd) % (HEIGHT * 3);

    TaskConfiguration taskConfiguration = {
      start_i,
      start_j,
      end_i,
      end_j,
      "Manhattan",
      "AStar",
      4,
      1.0,
    };

  return {taskConfiguration, grid};
}

int main() {

    auto task = GenerateTask();
    auto taskConfiguration = std::get<0>(task);
    auto grid = std::get<1>(task);

    Pathfinder pathfinder(taskConfiguration, grid);

    Application app(pathfinder, grid, taskConfiguration.start_i, taskConfiguration.start_j, taskConfiguration.goal_i,
                    taskConfiguration.goal_j);
    if (app.Construct(grid.GetWidth(), grid.GetHeight(), 10, 10))
        app.Start();

    return 0;
}

