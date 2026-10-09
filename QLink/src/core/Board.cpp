#include "Board.h"

#include <algorithm>
#include <random>

namespace {
constexpr int kEmptyPercent = 25;
constexpr int kTileTypeCount = 4;
}

Board::Board(int rows, int cols)
    : rows_(rows),
      cols_(cols),
      cells_(static_cast<size_t>(rows) * static_cast<size_t>(cols), TileType::Empty) {
}

int Board::rows() const {
    return rows_;
}

int Board::cols() const {
    return cols_;
}

TileType Board::at(int row, int col) const {
    if (!inBounds(row, col)) {
        return TileType::Empty;
    }
    return cells_[index(row, col)];
}

void Board::generateRandom() {
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> emptyDistribution(1, 100);
    std::uniform_int_distribution<int> typeDistribution(1, kTileTypeCount);

    for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
            if (emptyDistribution(generator) <= kEmptyPercent) {
                cells_[index(row, col)] = TileType::Empty;
            } else {
                cells_[index(row, col)] = static_cast<TileType>(typeDistribution(generator));
            }
        }
    }
}

void Board::clear() {
    std::fill(cells_.begin(), cells_.end(), TileType::Empty);
}

int Board::index(int row, int col) const {
    return row * cols_ + col;
}

bool Board::inBounds(int row, int col) const {
    return row >= 0 && row < rows_ && col >= 0 && col < cols_;
}
