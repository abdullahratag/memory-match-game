#include "raylib.h"
#include "GameBoard.h" // Our custom game board
#include "Stack.h"     // Our custom stack ADT

enum GameScreen { MENU, GAMEPLAY, GAME_OVER };     // Defines the different "screens" or states our game can be in.

// Global constants for the window dimensions.
const int screenWidth = 800; 
const int screenHeight = 600;

int main(void) {
     // --- Initialization ---
    // This section runs only once at the very beginning of the game.
    InitWindow(screenWidth, screenHeight, "Memory Match Game");
    SetTargetFPS(60);
    
    // --- Game State Variables ---
    // These variables control the state and flow of the game across different screens.
    GameScreen currentScreen = MENU;
    GameBoard* board = nullptr;      // A pointer to the current game board. Starts as null because no game has been chosen.
    Stack flippedCardsStack;    // Our custom Stack to manage flipped cards per turn.

     // --- Gameplay Variables ---
    // These are reset every time a new game starts.
    int gameDimension = 0;    // Size of the board
    int turns = 0;      // Counter for the number of turns the player has taken.
    int matchesFound = 0;    // Counter for how many pairs have been matched.     
    float flipTimer = 0.0f;   // A timer to create a delay before flipping non-matching cards back over.
    const float FLIP_DELAY = 1.0f;    // The duration of the delay (1 second).

    // --- High Score Variables ---
    // These carry on between games to track the player's best scores.
    int highScoreEasy = 999;
    int highScoreMedium = 999;
    int highScoreHard = 999;
    int highScoreImpossible = 999;

    bool isImpossibleMode = false;    // Flag to check if the special "Impossible Mode" is active.
    

    // --- Main Game Loop ---
    // This loop runs continuously until the player closes the window.
    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();    // Get the current mouse coordinates once per frame.

          // --- Update Section (Game Logic) ---
        // This switch statement handles all the logic, state changes, and input based on the current screen.
        switch (currentScreen) {
            case MENU: {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    isImpossibleMode = false;

                    // Check which button was clicked and set the game dimension accordingly.
                    if (CheckCollisionPointRec(mousePoint, {250, 200, 300, 50})) { gameDimension = 2; }
                    if (CheckCollisionPointRec(mousePoint, {250, 270, 300, 50})) { gameDimension = 4; }
                    if (CheckCollisionPointRec(mousePoint, {250, 340, 300, 50})) { gameDimension = 6; }
                    if (CheckCollisionPointRec(mousePoint, {250, 410, 300, 50})) { gameDimension = 4; isImpossibleMode = true; }

                    if (CheckCollisionPointRec(mousePoint, {250, 480, 300, 50})) { CloseWindow(); return 0; }

                      // If a valid game mode was selected, initialize the game state.
                    if (gameDimension > 0) {
                        board = new GameBoard(gameDimension);
                        turns = 0; matchesFound = 0;
                        currentScreen = GAMEPLAY;
                    }
                }
            } break;
            
            case GAMEPLAY: {
                 // If the flip timer is active, just count it down. This pauses player input.
                if (flipTimer > 0) {
                    flipTimer -= GetFrameTime();
                    // When the timer ends, flip the two cards on the stack back over.
                    if (flipTimer <= 0) {
                        CardPosition pos1 = flippedCardsStack.pop();
                        CardPosition pos2 = flippedCardsStack.pop();
                        board->cards[pos1.row][pos1.col].isFlipped = false;
                        board->cards[pos2.row][pos2.col].isFlipped = false;
                    }
                } else {
                    // If the timer is not active, listen for player clicks.
                    if (flippedCardsStack.size() < 2 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        // Calculate which card was clicked based on mouse position
                        int boardSizePixels = 480;
                        int cardSize = boardSizePixels / gameDimension;
                        int offsetX = (screenWidth - boardSizePixels) / 2; 
                        int offsetY = (screenHeight - boardSizePixels) / 2;
                        if (CheckCollisionPointRec(mousePoint, {(float)offsetX, (float)offsetY, (float)boardSizePixels, (float)boardSizePixels})) {
                            int col = (mousePoint.x - offsetX) / cardSize; 
                            int row = (mousePoint.y - offsetY) / cardSize;

                            // If the click was valid (on an unflipped, unmatched card), process the turn.
                            if (!board->cards[row][col].isMatched && !board->cards[row][col].isFlipped) {
                                board->cards[row][col].isFlipped = true;
                                flippedCardsStack.push(row, col); // Use the Stack to remember the pick.

                                // If two cards have been picked, check for a match.
                                if (flippedCardsStack.size() == 2) {
                                    turns++;
                                    CardPosition pos1 = flippedCardsStack.peek(); 
                                    CardPosition pos2 = flippedCardsStack.peekSecond();
                                    if (board->cards[pos1.row][pos1.col].value == board->cards[pos2.row][pos2.col].value) {     // Match
                                        board->cards[pos1.row][pos1.col].isMatched = true; 
                                        board->cards[pos2.row][pos2.col].isMatched = true;
                                        matchesFound++;
                                        flippedCardsStack.pop(); 
                                        flippedCardsStack.pop(); // Clear the stack for the next turn.

                                        // Check for win condition.
                                        if (board->isGameOver()) { 

                                             // Update the relevant high score if a new record was set.
                                            if (isImpossibleMode && turns < highScoreImpossible) {
                                                highScoreImpossible = turns;
                                            } 
                                            
                                            else if (!isImpossibleMode && gameDimension == 2 && turns < highScoreEasy) {
                                                highScoreEasy = turns;
                                            } 
                                            else if (!isImpossibleMode && gameDimension == 4 && turns < highScoreMedium) {
                                                highScoreMedium = turns;
                                            } 
                                            else if (!isImpossibleMode && gameDimension == 6 && turns < highScoreHard) {
                                                highScoreHard = turns;
                                            }

                                            
                                            currentScreen = GAME_OVER;  // Switch to the game over screen.
                                        }
                                    } else {    // Not a match.
                                        if (isImpossibleMode) {    // If in Impossible Mode, reset all previous matches.
                                            for (int r = 0; r < board->dimension; ++r) {
                                                for (int c = 0; c < board->dimension; ++c) {
                                                    if (board->cards[r][c].isMatched) {
                                                        board->cards[r][c].isMatched = false;
                                                        board->cards[r][c].isFlipped = false; 
                                                    }
                                                }
                                            }
                                            matchesFound = 0;
                                        }
                                      
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
                     // If the "Back to Menu" button is clicked, clean up and switch states.
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

         // --- Draw Section ---
        // This section runs every frame and is responsible for drawing everything to the screen.
        BeginDrawing();
        ClearBackground(DARKGRAY);
        switch (currentScreen) {
            case MENU: {
                 // Draw the main menu UI elements.
                int titleWidth = MeasureText("MEMORY MATCH", 70);
                DrawText("MEMORY MATCH", (screenWidth / 2) - (titleWidth / 2), 100, 70, RED);

                DrawRectangle(250, 200, 300, 50, BLACK); DrawText("Easy (2x2)", 335, 215, 20, RAYWHITE);
                DrawRectangle(250, 270, 300, 50, BLACK); DrawText("Medium (4x4)", 320, 285, 20, RAYWHITE);
                DrawRectangle(250, 340, 300, 50, BLACK); DrawText("Hard (6x6)", 335, 355, 20, RAYWHITE);
                DrawRectangle(250, 410, 300, 50, BLACK); DrawText("Impossible", 320, 425, 20, RED);
                DrawRectangle(250, 480, 300, 50, MAROON); DrawText("Quit", 370, 495, 20, WHITE);

                // Draw high scores, showing "--" if no score has been set yet.
                const char* easyScoreText = (highScoreEasy == 999) ? "Best: --" : TextFormat("Best: %d", highScoreEasy);
                DrawText(easyScoreText, 570, 215, 20, LIGHTGRAY);
                
              
                const char* mediumScoreText = (highScoreMedium == 999) ? "Best: --" : TextFormat("Best: %d", highScoreMedium);
                DrawText(mediumScoreText, 570, 285, 20, LIGHTGRAY);

                
                const char* hardScoreText = (highScoreHard == 999) ? "Best: --" : TextFormat("Best: %d", highScoreHard);
                DrawText(hardScoreText, 570, 355, 20, LIGHTGRAY);

                const char* impossibleScoreText = (highScoreImpossible == 999) ? "Best: --" : TextFormat("Best: %d",
                highScoreImpossible);
                DrawText(impossibleScoreText, 570,425,20, RED);

            } break;
            case GAMEPLAY: {
                // Draw the game board by iterating through the 2D cards grid.
                int boardSizePixels = 480; int cardSize = boardSizePixels / gameDimension; int padding = 5;
                int offsetX = (screenWidth - boardSizePixels) / 2; int offsetY = (screenHeight - boardSizePixels) / 2;
                for (int r = 0; r < gameDimension; ++r) {
                    for (int c = 0; c < gameDimension; ++c) {
                        Rectangle cardRect = {(float)(offsetX + c * cardSize + padding), (float)(offsetY + r * cardSize + padding), 
                            (float)(cardSize - 2 * padding), (float)(cardSize - 2 * padding)};
                            
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

                // Draw gameplay stats
                DrawText(TextFormat("Turns: %d", turns), 20, 20, 30, RAYWHITE);
                int totalPairs = (gameDimension * gameDimension) / 2;
                DrawText(TextFormat("Matches: %d / %d", matchesFound, totalPairs), screenWidth - 250, 20, 30, RAYWHITE);
            } break;
            
            case GAME_OVER: { 
                 // Draw the game over UI elements, centering the text.
                int goodJobWidth = MeasureText("Good Job!", 70);
                DrawText("Good Job!", (screenWidth / 2) - (goodJobWidth / 2), 200 , 70, GOLD);

                const char* finishedText = TextFormat("You finished in %d turns!", turns);
                int finishedTextWidth = MeasureText(finishedText, 30);
                DrawText(finishedText, (screenWidth / 2) - (finishedTextWidth / 2), 280, 30, RAYWHITE);

                
                  bool isNewHighScore = (isImpossibleMode && turns == highScoreImpossible) || (!isImpossibleMode && gameDimension == 2 && turns == highScoreEasy) ||
                                      (!isImpossibleMode && gameDimension == 4 && turns == highScoreMedium) || (!isImpossibleMode && gameDimension == 6 && turns == highScoreHard);
                if(isNewHighScore && turns != 999){
                     DrawText("New High Score!", (screenWidth / 2) - (MeasureText("New High Score!", 20)/2), 320, 20, GOLD);
                }

                DrawRectangle(250,350,300,50, BLACK);
                int backToMenuTextWidth = MeasureText("Back to Menu", 20);
                DrawText("Back to Menu", (screenWidth / 2) - (backToMenuTextWidth / 2), 365, 20, RAYWHITE);
   
             } break;
        }
        EndDrawing();
    } // End of main game loo

     // --- De-Initialization ---
    // Clean up resources before the program closes.
    if (board != nullptr) { delete board; }
    CloseWindow();
    return 0;
}
