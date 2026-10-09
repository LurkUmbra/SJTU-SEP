#pragma once

#include <vector>

#include "TileType.h"

class Board {
public:
    Board(int rows, int cols);

    int rows() const;
    int cols() const;
    TileType at(int row, int col) const;

    void generateRandom();
    void clear();

private:
    int rows_;
    int cols_;
    std::vector<TileType> cells_;

    int index(int row, int col) const;
    bool inBounds(int row, int col) const;
};
