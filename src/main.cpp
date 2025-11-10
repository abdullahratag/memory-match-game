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

enum GameScreen { MENU, GAMEPLAY, GAME_OVER };
const int screenWidth = 800;
const int screenHeight = 600;

int main(void) 
{
    InitWindow(screenWidth, screenHeight, "Memory Match Game");
    SetTargetFPS(60);
    GameScreen currentScreen = MENU;
    GameBoard* board = nullptr;
    int gameDimension = 0;

    while (!WindowShouldClose()) 
    {
        Vector2 mousePoint = GetMousePosition();
        switch (currentScreen){
            case MENU: {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if(CheckCollisionPointRec(mousePoint, {250, 200, 300, 50})) { gameDimension = 2;}
                    if(CheckCollisionPointRec(mousePoint, {250, 270, 300, 50})) { gameDimension = 4;}
                    if(CheckCollisionPointRec(mousePoint, {250, 340, 300, 50})) { gameDimension = 2;}
                    if(CheckCollisionPointRec(mousePoint, {250, 410, 300, 50})) { CloseWindow(); return 0; }
                    if (gameDimension > 0 ) {
                        board = new GameBoard(gameDimension); 
                        currentScreen = GAMEPLAY;
                    }
                } break;
                case GAMEPLAY: {} break; 
                case GAME_OVER: {} break;
            }
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch (currentScreen){
            case MENU: {
                DrawText("MEMORY MATCH", 170, 100, 70, DARKGRAY);
                DrawRectangle(250, 200, 300, 50, LIGHTGRAY); DrawText("Easy", 335, 215, 20, BLACK);
                DrawRectangle(250, 270, 300, 50, LIGHTGRAY); DrawText("Medium", 320, 285, 20, BLACK);
                DrawRectangle(250, 340, 300, 50, LIGHTGRAY); DrawText("Hard", 335, 355, 20, BLACK);
                DrawRectangle(250, 410, 300, 50, MAROON); DrawText("Quit", 370, 425, 20, WHITE);
            } break;
            case GAMEPLAY: {} break;
            case GAME_OVER: {} break;
        }
        EndDrawing();
    }
    if (board != nullptr){
        delete board;
    }
    CloseWindow();
    return 0;
}