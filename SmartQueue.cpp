#include "SmartQueue.h"
#include <tuple>

SmartQueue::SmartQueue() {
    auto comparator = [&](std::tuple<double, int> a, std::tuple<double, int> b) {
        return std::get<1>(a) > std::get<1>(b);
    };

    queue = std::priority_queue<
            std::tuple<int, double>,
            std::vector<std::tuple<int, double>>,
            std::function<bool(std::tuple<int, double>, std::tuple<int, double>)>
    >{comparator};
}

void SmartQueue::Push(int cell, double priority) {
    queue.push(std::make_tuple(cell, priority));
}

int SmartQueue::Pop() {
    auto [cell, priority] = queue.top();
    queue.pop();
    return cell;
}

bool SmartQueue::Empty() {
    return queue.empty();
}
