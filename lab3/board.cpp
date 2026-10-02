#include <iostream>

#include "board.h"
#include "queue.h"

Board::Board(const int num_disk) : /* TODO */ {
    // TODO
}

Board::~Board() {
    // TODO
}

void Board::draw() {
    Canvas canvas {};
    canvas.reset();
    // TODO
}

void Board::move(int from, int to, const bool log) {
    // TODO
}

bool Board::win() const {
    // TODO
}

void solve(
    const int n,
    const int src,
    const int buf,
    const int dest,
    Queue<std::pair<int, int>> &solution
) {
    // TODO
}

void Board::autoplay() {
    // TODO
}
