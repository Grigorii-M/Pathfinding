#pragma once

#include <queue>
#include <tuple>
#include <functional>

/// A priority queue that allows
class SmartQueue {
private:
    std::priority_queue<std::tuple<int, double>, std::vector<std::tuple<int, double>>, std::function<bool(
            std::tuple<int, double>, std::tuple<int, double>)>> queue; // (priority, cell)

public:
    SmartQueue();

    void Push(int cell, double priority);

    int Pop();

    bool Empty();
};
