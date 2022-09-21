#include "SmartQueue.h"
#include <utility>
#include <tuple>

SmartQueue::SmartQueue(std::function<double (int)> priorityFunction) : priorityFunction(std::move(priorityFunction)) {
    auto comparator = [&](std::tuple<double, int> a, std::tuple<double, int> b) {
        return std::get<0>(a) > std::get<0>(b);
    };

    queue = std::priority_queue<
            std::tuple<double, int>,
            std::vector<std::tuple<double, int>>,
            std::function<bool(std::tuple<double, int>, std::tuple<double, int>)>
    > {comparator};
}

void SmartQueue::Push(int cell) {
    int heuristicValue = priorityFunction(cell);
    queue.push(std::make_tuple(heuristicValue, cell));
}

int SmartQueue::Pop() {
    auto [heuristicValue, cell] = queue.top();
    queue.pop();
    return cell;
}

bool SmartQueue::Empty() {
    return queue.empty();
}
