#pragma once
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>

struct Card {
    char value;
    bool isFlipped = false;
    bool isMatched = false;
};

class GameBoard {
public:
    std::vector<std::vector<Card>> cards;
    int dimension;

    GameBoard(int dim);
    bool isGameOver() const;
};
