#include "raylib.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>


class Stack 
{
    public: 
   
};


struct Card {

};


class GameBoard 
{
    public: 
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