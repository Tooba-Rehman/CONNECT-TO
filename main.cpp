#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <unordered_map>
#include<cstring>
#include <algorithm>

#include "include\raylib.h"
#include "graph.h"

using namespace std;

void DrawCenteredText(const char *text, int y, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, (GetScreenWidth() - textWidth) / 2, y, fontSize, color);
}

int main() {
    SocialNetwork network;

    // Load users from file
    network.loadFromFile("users.txt");

    // Define custom colors
    Color LIGHT_BLUE = { 173, 216, 230, 255 }; // RGB(173, 216, 230) for background
    Color DARK_BLUE = { 0, 0, 139, 255 };      // RGB(0, 0, 139) for text and outlines
    Color BUTTON_WHITE = { 255, 255, 255, 255 }; // RGB(255, 255, 255) for button background

    // Initialize Raylib
    const int screenWidth = 1000; // Shorter width
    const int screenHeight = 700; // Shorter height
    InitWindow(screenWidth, screenHeight, "ConnectTo");

    // Set the target FPS
    SetTargetFPS(60);

    // Button properties
    Rectangle loginButton = { screenWidth / 2 - 125, screenHeight / 2 - 70, 250, 60 };
    Rectangle signUpButton = { screenWidth / 2 - 125, screenHeight / 2 + 10, 250, 60 };
    Rectangle exitButton = { screenWidth / 2 - 125, screenHeight / 2 + 90, 250, 60 };

    bool exitProgram = false;

    // Main loop
    while (!WindowShouldClose() && !exitProgram) {
        // Check for button clicks
        Vector2 mousePos = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mousePos, loginButton)) {
                network.userLogin();
            } else if (CheckCollisionPointRec(mousePos, signUpButton)) {
                network.userSignUp();
            } else if (CheckCollisionPointRec(mousePos, exitButton)) {
                cout << "Exiting the Program..." << endl;
                exitProgram = true;
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(LIGHT_BLUE); // Light blue background

        // Draw title
        DrawCenteredText("ConnectTo", 120, 50, DARK_BLUE);

        // Draw buttons with white background
        DrawRectangleRec(loginButton, BUTTON_WHITE);
        DrawCenteredText("Login", loginButton.y + 15, 25, DARK_BLUE);

        DrawRectangleRec(signUpButton, BUTTON_WHITE);
        DrawCenteredText("Sign Up", signUpButton.y + 15, 25, DARK_BLUE);

        DrawRectangleRec(exitButton, BUTTON_WHITE);
        DrawCenteredText("Exit", exitButton.y + 15, 25, DARK_BLUE);

        // Draw button outlines when hovered
        if (CheckCollisionPointRec(mousePos, loginButton)) {
            DrawRectangleLinesEx(loginButton, 2, DARK_BLUE);
        }
        if (CheckCollisionPointRec(mousePos, signUpButton)) {
            DrawRectangleLinesEx(signUpButton, 2, DARK_BLUE);
        }
        if (CheckCollisionPointRec(mousePos, exitButton)) {
            DrawRectangleLinesEx(exitButton, 2, DARK_BLUE);
        }

        EndDrawing();
    }

    // De-initialize Raylib
    CloseWindow();

    return 0;
}
