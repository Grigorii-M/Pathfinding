#pragma once

#include <queue>
#include <tuple>
#include <functional>

class SmartQueue {
private:
    std::priority_queue<std::tuple<double, int>, std::vector<std::tuple<double, int>>, std::function<bool(std::tuple<double, int>, std::tuple<double, int>)>> queue; // (priority, cell)
    std::function<double (int)> priorityFunction;

public:
    SmartQueue() = default;
    explicit SmartQueue(std::function<double (int)> priorityFunction);
    void Push(int cell);
    int Pop();
    bool Empty();
};
