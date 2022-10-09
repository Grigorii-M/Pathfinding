#pragma once

#include <inttypes.h>
#include <vector>
#include <random>
#include <algorithm>
#include <tuple>
#include <iostream>

struct MazeCell {
  uint32_t x;
  uint32_t y;

  bool north;
  bool south;
  bool east;
  bool west;
};

std::vector<MazeCell> generateMaze(uint32_t width, uint32_t height) {
  std::vector<MazeCell> maze;
  std::vector<uint32_t> unvisited;

  for (uint32_t y = 0; y < height; y++) {
    for (uint32_t x = 0; x < width; x++) {
      maze.push_back(MazeCell {x, y, false, false, false, false});
      unvisited.push_back(maze.size() - 1);
    }
  }

  unvisited.erase(unvisited.begin() + rand() % unvisited.size());

  while (!unvisited.empty()) {
    uint32_t cell = unvisited.at(rand() % unvisited.size());
    std::vector<uint32_t> path = { cell };

    auto getNeighbors = [&maze, width] (uint32_t cell) {
      uint32_t x = cell % width;

      std::vector<uint32_t> neighbors;

      if (cell >= width) {
        neighbors.push_back(cell - width);
      }
      
      if (cell + width < maze.size()) {
        neighbors.push_back(cell + width);
      }

      if (x >= 1 && cell - 1 >= 0) {
        neighbors.push_back(cell - 1);
      }
      
      if (x + 1 < width && cell + 1 < maze.size()) {
        neighbors.push_back(cell + 1);
      }

      return neighbors;
    };

    while (std::find(unvisited.begin(), unvisited.end(), cell) != unvisited.end()) {
      auto neighbors = getNeighbors(cell);

      cell = neighbors.at(rand() % neighbors.size());

      auto iter = std::find(path.begin(), path.end(), cell);
      if (iter != path.end()) {
        path.erase(iter + 1, path.end());
      } else {
        path.push_back(cell);
      }
    }

    auto linkCells = [&maze] (uint32_t index1, uint32_t index2) {
      int dx = maze.at(index2).x - maze.at(index1).x;
      int dy = maze.at(index2).y - maze.at(index1).y;

      if (dx == 1 && dy == 0) {
        maze.at(index1).east = true;
        maze.at(index2).west = true;
      }

      if (dx == -1 && dy == 0) {
        maze.at(index1).west = true;
        maze.at(index2).east = true;
      }
      
      if (dx == 0 && dy == 1) {
        maze.at(index1).south = true;
        maze.at(index2).north = true;
      }

      if (dx == 0 && dy == -1) {
        maze.at(index1).north = true;
        maze.at(index2).south = true;
      }
    };

    for (uint32_t i = 0; i < path.size() - 1; i++) {
      linkCells(path[i], path[i + 1]);
      auto index = std::find(unvisited.begin(), unvisited.end(), path[i]); 
      if (index != unvisited.end()) {
        unvisited.erase(index);
      }
    }
  }

  return maze;
}

std::vector<bool> convertMaze(std::vector<MazeCell> maze, uint32_t width, uint32_t height) {
  std::vector<bool> cells(width * 3 * height * 3, false);

  for (uint32_t y = 0; y < height; y++) {
    for (uint32_t x = 0; x < width; x++) {
      auto cell = maze.at(x + y * width);

      uint32_t x3 = x * 3;
      uint32_t y3 = y * 3;
      uint32_t w3 = width * 3;

      // set center to wallable
      cells[x3 + 1 + (y3 + 1) * w3] = true;

      cells[x3 + 1 + y3 * w3] = cell.north;
      cells[x3 + 1 + (y3 + 2) * w3] = cell.south;
      cells[x3 + 2 + (y3 + 1) * w3] = cell.east;
      cells[x3 + (y3 + 1) * w3] = cell.west;
    }
  }

  return cells;
}

std::tuple<uint32_t, uint32_t> GetStartEndPoints(std::vector<bool> maze) {
  while (true) {
    auto index1 = rand() % maze.size();
    auto index2 = rand() % maze.size();

    if (maze[index1] && maze[index2]) {
      return {index1, index2};
    }
  }
}
