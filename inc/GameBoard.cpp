#include "GameBoard.hpp"

GameBoard::GameBoard(int dim) : dimension(dim) {
    std::vector<char> pairs;
    char current_char = 'A';
    for (int i = 0; i < (dimension * dimension) / 2; ++i) {
        pairs.push_back(current_char);
        pairs.push_back(current_char);
        current_char++;
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(pairs.begin(), pairs.end(), std::default_random_engine(seed));
    cards.resize(dimension, std::vector<Card>(dimension));
    for (int r = 0; r < dimension; ++r) {
        for (int c = 0; c < dimension; ++c) {
            cards[r][c].value = pairs[r * dimension + c];
        }
    }
}

bool GameBoard::isGameOver() const {
    for (int r = 0; r < dimension; ++r) {
        for (int c = 0; c < dimension; ++c) {
            if (!cards[r][c].isMatched)
                return false;
        }
    }
    return true;
}
