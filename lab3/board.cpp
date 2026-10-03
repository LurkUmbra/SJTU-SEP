#include <iostream>

#include "board.h"
#include "queue.h"

Board::Board(const int num_disk)
    : num_disk(num_disk), rods{Rod(5, 0), Rod(5, 1), Rod(5, 2)}, disks(new Disk[num_disk]), history() {
        for (int i = num_disk - 1; i >= 0; i--) {
            disks[i] = Disk(0, 2 * i + 3);
            rods[0].push(disks[i]);
        }
    }

Board::~Board() {
    delete[] disks;
    disks = nullptr;
}

void Board::draw() {
    Canvas canvas {};
    canvas.reset();
    for (int i = 0; i < 3; i++) {
        rods[i].draw(canvas);
    }
    for (int i = 0; i <= Canvas::WIDTH - 1; i++) {
        if (i % 15 != 5) {
            canvas.buffer[Canvas::HEIGHT - 1][i] = '-';
        }
    }
    canvas.draw();
}

void Board::move(int from, int to, const bool log) {
    if (from <= 0 || from > 3 || to <= 0 || to > 3) {
        draw();
        return;
    }
    if (rods[from - 1].empty()) {
        draw();
        return;
    }
    if (!rods[to - 1].empty() && rods[from - 1].top().val > rods[to - 1].top().val) {
        draw();
        return;
    }
    Disk d = rods[from - 1].top();
    d.id = to - 1;
    rods[from - 1].pop();
    rods[to - 1].push(d);
    if (log) {
        history.push(std::make_pair(from, to));
    } else {
        std::cout << "Auto moving:" << from << "->" << to << std::endl;
    }
    draw();
}

bool Board::win() const {
    return int(rods[1].size()) == num_disk;
}

void solve(
    const int n,
    const int src,
    const int buf,
    const int dest,
    Queue<std::pair<int, int>> &solution
) {
    if (n == 1) {
        solution.push(std::make_pair(src, dest));
        return;
    }
    solve(n - 1, src, dest, buf, solution);
    solution.push(std::make_pair(src, dest));
    solve(n - 1, buf, src, dest, solution);
}

void Board::autoplay() {
    if (history.empty()) {
        Queue<std::pair<int, int>> solution;
        solve(num_disk, 1, 3, 2, solution);
        while (!solution.empty()) {
            const auto [from, to] = solution.front();
            move(from, to, false);
            solution.pop();
        }
    } else {
        while (!history.empty()) {
            const auto [from, to] = history.top();
            move(to, from, false);
            history.pop();
        }
        autoplay();
    }
}
