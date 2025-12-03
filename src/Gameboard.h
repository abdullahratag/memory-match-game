#pragma once

#include <vector>
#include <string>
#include <random>       // Needed for std::default_random_engine for shuffling
#include <algorithm>    // for std::shuffle
#include <chrono>       // for seeding the random number generator

// Represents the state of a single card on the game board.
struct Card { char value; bool isFlipped = false; bool isMatched = false; };

// Manages the state of the entire game board, including card generation and shuffling.
class GameBoard {
public:
    std::vector<std::vector<Card>> cards;  // The 2D grid of cards that represents the board.
    int dimension;      // The size of the board.
     // Constructor to initialize a game board of a given dimension.
    GameBoard(int dim) : dimension(dim) {        // 1. Create a flat list of character pairs.
        std::vector<char> pairs;
        char current_char = 'A';

        for (int i = 0; i < (dimension * dimension) / 2; ++i) {
            pairs.push_back(current_char);
            pairs.push_back(current_char);
            current_char++;
        }
        
          // 2. Shuffle the pairs randomly.
        // We use the system clock to provide a unique seed for the random number generator,
        // ensuring a different shuffle each time the game is played.
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(pairs.begin(), pairs.end(), std::default_random_engine(seed));
          // 3. Populate the 2D `cards` grid with the shuffled pairs.
        cards.resize(dimension, std::vector<Card>(dimension));

        for (int r = 0; r < dimension; ++r) {
            for (int c = 0; c < dimension; ++c) {
                cards[r][c].value = pairs[r * dimension + c];
            }
        }
    }


     // Checks if all cards on the board have been matched.
    // Returns true if the game is over, false otherwise.
    bool isGameOver() const {
        for (int r = 0; r < dimension; ++r) {            // Iterate through every card on the board.
            for (int c = 0; c < dimension; ++c) {           // If we find even one card that is not yet matched, the game is not over.
                if (!cards[r][c].isMatched)
                    return false;
            }
        }
        return true;
    }
};
