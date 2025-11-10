#include "raylib.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>


struct CardPosition 
{
    int row; int col;
};

struct Node{
    CardPosition data; 
    Node* next = nullptr;
};



class Stack 
{
    private:
        Node* top;
    public:
        Stack() : top(nullptr) {}
        ~Stack() { while (!isEmpty()) { pop(); }}
        bool isEmpty() { return top == nullptr; }
        void push (int r, int c) { 
            Node* newNode =  new Node();
            newNode->data = {r, c};
            newNode->next = top;
            top = newNode;
         }

         CardPosition pop() 
         {
            if (isEmpty()) throw std::runtime_error("Stack is empty");
            Node* temp = top;
            CardPosition poppedData = temp->data;
            top = top->next;
            delete temp;
            return poppedData;
         }

         CardPosition peek()
         {
            if (isEmpty()) throw std::runtime_error("Stack is empty");
            return top->data;
         }

         CardPosition peekSecond()
         {
            if (isEmpty() || top->next == nullptr) throw std::runtime_error("Stack lacks second item");
            return top->next->data;
         }

         int size()
         {
            int count = 0; 
            Node* current = top;
            while ( current != nullptr )
            {
                count++; current = current->next;
            }
            return count;
        }
    };


struct Card {
    char value;
    bool isFlipped = false;
    bool isMatched = false;

};


class GameBoard 
{
    public: 
        std::vector<std::vector<Card>> cards;
        int dimension;
        GameBoard(int dim) : dimension(dim){
            std::vector<char> pairs;
            char current_char = 'A';
            for (int i = 0; i < (dimension * dimension) / 2; ++i){
                pairs.push_back(current_char);
                pairs.push_back(current_char);
                current_char++;
            }
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(pairs.begin(), pairs.end(), std::default_random_engine(seed));
            cards.resize(dimension, std::vector<Card>(dimension));
            for(int r = 0; r < dimension; ++r) 
            {
                for (int c = 0; c < dimension; ++c){
                    cards[r][c].value = pairs[r * dimension + c]; } }
                }
            bool isGameOver() const {
                for (int r = 0; r < dimension; ++r)
                {
                    for (int c = 0; c < dimension; ++c)
                    {
                        if (!cards[r][c].isMatched) return false;
                    }
                }
                return true;
            
            

        } 
};

int main(void) 
{
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Memory Match Game");
    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Initial Setup", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}