#include "raylib.h"
#include "GameBoard.h" // Our custom game board
#include "Stack.h"     // Our custom stack ADT

enum GameScreen { MENU, GAMEPLAY, GAME_OVER };
const int screenWidth = 800; const int screenHeight = 600;

int main(void) {
    InitWindow(screenWidth, screenHeight, "Memory Match Game");
    SetTargetFPS(60);
    GameScreen currentScreen = MENU;
    GameBoard* board = nullptr;
    Stack flippedCardsStack;
    int gameDimension = 0; int turns = 0; int matchesFound = 0;
    float flipTimer = 0.0f; const float FLIP_DELAY = 1.0f;

    int highScoreEasy = 999;
    int highScoreMedium = 999;
    int highScoreHard = 999;

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();
        switch (currentScreen) {
            case MENU: {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (CheckCollisionPointRec(mousePoint, {250, 200, 300, 50})) { gameDimension = 2; }
                    if (CheckCollisionPointRec(mousePoint, {250, 270, 300, 50})) { gameDimension = 4; }
                    if (CheckCollisionPointRec(mousePoint, {250, 340, 300, 50})) { gameDimension = 6; }
                    if (CheckCollisionPointRec(mousePoint, {250, 410, 300, 50})) { CloseWindow(); return 0; }
                    if (gameDimension > 0) {
                        board = new GameBoard(gameDimension);
                        turns = 0; matchesFound = 0;
                        currentScreen = GAMEPLAY;
                    }
                }
            } break;
            case GAMEPLAY: {
                if (flipTimer > 0) {
                    flipTimer -= GetFrameTime();
                    if (flipTimer <= 0) {
                        CardPosition pos1 = flippedCardsStack.pop(); CardPosition pos2 = flippedCardsStack.pop();
                        board->cards[pos1.row][pos1.col].isFlipped = false; board->cards[pos2.row][pos2.col].isFlipped = false;
                    }
                } else {
                    if (flippedCardsStack.size() < 2 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        int boardSizePixels = 480; int cardSize = boardSizePixels / gameDimension;
                        int offsetX = (screenWidth - boardSizePixels) / 2; int offsetY = (screenHeight - boardSizePixels) / 2;
                        if (CheckCollisionPointRec(mousePoint, {(float)offsetX, (float)offsetY, (float)boardSizePixels, (float)boardSizePixels})) {
                            int col = (mousePoint.x - offsetX) / cardSize; int row = (mousePoint.y - offsetY) / cardSize;
                            if (!board->cards[row][col].isMatched && !board->cards[row][col].isFlipped) {
                                board->cards[row][col].isFlipped = true; flippedCardsStack.push(row, col);
                                if (flippedCardsStack.size() == 2) {
                                    turns++;
                                    CardPosition pos1 = flippedCardsStack.peek(); CardPosition pos2 = flippedCardsStack.peekSecond();
                                    if (board->cards[pos1.row][pos1.col].value == board->cards[pos2.row][pos2.col].value) {
                                        board->cards[pos1.row][pos1.col].isMatched = true; board->cards[pos2.row][pos2.col].isMatched = true;
                                        matchesFound++;
                                        flippedCardsStack.pop(); flippedCardsStack.pop();
                                        if (board->isGameOver()) { 

                                            if (gameDimension == 2 && turns < highScoreEasy){
                                                highScoreEasy = turns;
                                            } else if (gameDimension == 4 && turns < highScoreMedium) {
                                                highScoreMedium = turns;
                                            } else if (gameDimension == 6 && turns < highScoreHard){
                                                highScoreHard = turns;
                                            }
                                            currentScreen = GAME_OVER; 
                                        }
                                    } else {
                                        flipTimer = FLIP_DELAY;
                                    }
                                }
                            }
                        }
                    }
                }
            } break;
            case GAME_OVER: {
                 if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if(CheckCollisionPointRec(mousePoint, {250,350,300,50}))
                    {
                        delete board;
                        board = nullptr;
                        gameDimension = 0;
                        currentScreen = MENU;
                    }
                }
              } break;
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);
        switch (currentScreen) {
            case MENU: {
                int titleWidth = MeasureText("MEMORY MATCH", 70);
                DrawText("MEMORY MATCH", (screenWidth / 2) - (titleWidth / 2), 100, 70, RED);

                DrawRectangle(250, 200, 300, 50, BLACK); DrawText("Easy (2x2)", 335, 215, 20, RAYWHITE);
                DrawRectangle(250, 270, 300, 50, BLACK); DrawText("Medium (4x4)", 320, 285, 20, RAYWHITE);
                DrawRectangle(250, 340, 300, 50, BLACK); DrawText("Hard (6x6)", 335, 355, 20, RAYWHITE);
                DrawRectangle(250, 410, 300, 50, MAROON); DrawText("Quit", 370, 425, 20, WHITE);
               
                const char* easyScoreText = (highScoreEasy == 999) ? "Best: --" : TextFormat("Best: %d", highScoreEasy);
                DrawText(easyScoreText, 570, 215, 20, LIGHTGRAY);
                
              
                const char* mediumScoreText = (highScoreMedium == 999) ? "Best: --" : TextFormat("Best: %d", highScoreMedium);
                DrawText(mediumScoreText, 570, 285, 20, LIGHTGRAY);

                
                const char* hardScoreText = (highScoreHard == 999) ? "Best: --" : TextFormat("Best: %d", highScoreHard);
                DrawText(hardScoreText, 570, 355, 20, LIGHTGRAY);
            } break;
            case GAMEPLAY: {
                int boardSizePixels = 480; int cardSize = boardSizePixels / gameDimension; int padding = 5;
                int offsetX = (screenWidth - boardSizePixels) / 2; int offsetY = (screenHeight - boardSizePixels) / 2;
                for (int r = 0; r < gameDimension; ++r) {
                    for (int c = 0; c < gameDimension; ++c) {
                        Rectangle cardRect = {(float)(offsetX + c * cardSize + padding), (float)(offsetY + r * cardSize + padding), (float)(cardSize - 2 * padding), (float)(cardSize - 2 * padding)};
                        if (board->cards[r][c].isFlipped || board->cards[r][c].isMatched) {
                            DrawRectangleRec(cardRect, LIGHTGRAY);
                            DrawRectangleLinesEx(cardRect, 2, board->cards[r][c].isMatched ? RED : BLACK);
                            std::string val(1, board->cards[r][c].value);
                            int fontSize = cardSize / 2;
                           
                            int textWidth = MeasureText(val.c_str(), fontSize);
                            DrawText(val.c_str(), cardRect.x + cardRect.width / 2 - textWidth / 2, cardRect.y + cardRect.height / 2 - fontSize / 2, fontSize, BLACK);
                        } else {
                            DrawRectangleRec(cardRect, BLACK);
                        }
                    }
                }
                DrawText(TextFormat("Turns: %d", turns), 20, 20, 30, RAYWHITE);
                int totalPairs = (gameDimension * gameDimension) / 2;
                DrawText(TextFormat("Matches: %d / %d", matchesFound, totalPairs), screenWidth - 250, 20, 30, RAYWHITE);
            } break;
            case GAME_OVER: { 
                int goodJobWidth = MeasureText("Good Job!", 70);
                DrawText("Good Job!", (screenWidth / 2) - (goodJobWidth / 2), 200 , 70, GOLD);

                const char* finishedText = TextFormat("You finished in %d turns!", turns);
                int finishedTextWidth = MeasureText(finishedText, 30);
                DrawText(finishedText, (screenWidth / 2) - (finishedTextWidth / 2), 280, 30, RAYWHITE);

                
                bool isNewHighScore = (gameDimension == 2 && turns == highScoreEasy) ||
                                      (gameDimension == 4 && turns == highScoreMedium) ||
                                      (gameDimension == 6 && turns == highScoreHard);
                if(isNewHighScore && turns != 999){
                     DrawText("New High Score!", (screenWidth / 2) - (MeasureText("New High Score!", 20)/2), 320, 20, GOLD);
                }

                DrawRectangle(250,350,300,50, BLACK);
                int backToMenuTextWidth = MeasureText("Back to Menu", 20);
                DrawText("Back to Menu", (screenWidth / 2) - (backToMenuTextWidth / 2), 365, 20, RAYWHITE);
   
             } break;
        }
        EndDrawing();
    }
    if (board != nullptr) { delete board; }
    CloseWindow();
    return 0;
}
