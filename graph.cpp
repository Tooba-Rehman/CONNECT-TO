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

#include "graph.h"
#include "include/raylib.h"

using namespace std;

// Default constructor
User::User() : userID(0), name(""), password(""), interest(""), posts(0), comments(0), likes(0), friends(vector<int>()) {}

// Parameterized constructor
User::User(int id, string username, string psw, string userInterest, int p, int c, int l, vector<int> userFriends) : userID(id), name(username), password(psw), interest(userInterest), posts(p), comments(c), likes(l), friends(userFriends) {}

// Function to add a user to the network
void SocialNetwork::addUser(int id, string name, string password, string interest, int posts, int comments, int likes, vector<int> friends) {
        users[id] = User(id, name, password, interest, posts, comments, likes, friends);
}

// Function to give the total number of users
int SocialNetwork::totalUsers() {
        return count;
}

void SocialNetwork::DrawCenteredText(const char *text, int y, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, (GetScreenWidth() - textWidth) / 2, y, fontSize, color);
}

// Function to update user friends in the file
void SocialNetwork::updateUsersFile(const string &filename) {
    ofstream outFile(filename);

    if (!outFile.is_open()) {
        cout << "Error: Unable to open file for updating." << endl;
        return;
    }

    // Write each user's data to the file
    for (const auto &userPair : users) {
        const User &user = userPair.second;
        outFile << user.userID << " " << user.name << " " << user.password << " " << user.interest << " " << user.posts << " " << user.comments << " " << user.likes;

        // Write the list of friends
        for (int friendID : user.friends) {
            outFile << " " << friendID;
        }
        outFile << endl; // Move to the next line for the next user
    }

    outFile.close();
}

// Function to save the data of the user in the file as a new user
void SocialNetwork::saveUserFile(const string &fileName, const User &user) {
    ofstream outFile(fileName, ios::app);
    if (!outFile) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    outFile << user.userID << " " << user.name << " " << user.password << " " << user.interest << " " << user.posts << " " << user.comments << " " << user.likes;
    outFile << endl;

    outFile.close();
}

void SocialNetwork::friendsUser(string name) {
    bool found = false;

    // Raylib Window Setup
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Friends List - ConnectTo");
    SetTargetFPS(60);

    // Custom Colors
    Color LIGHT_BLUE = { 173, 216, 230, 255 }; // RGB(173, 216, 230) for background
    Color DARK_BLUE = { 0, 0, 139, 255 };      // RGB(0, 0, 139) for text and outlines
    Color BUTTON_WHITE = { 255, 255, 255, 255 }; // RGB(255, 255, 255) for button background

    // Prepare to display friends
    vector<string> friendsList;
    
    // Search for the user and gather their friends
    for (const auto &userPair : users) {
        string UserName = userPair.second.name;
        if (name == UserName) {
            found = true;
            int degree = userPair.second.friends.size();
            string Name = userPair.second.name;
            friendsList.push_back("Number of friends = " + to_string(degree));

            if(degree != 0) {
                friendsList.push_back("List of Friends : ");
                // Populate the friends list
                showConnections(userPair.second.userID, friendsList);
            }
            break;
        }
    }

    if (!found) {
        friendsList.push_back("Error: No User with username " + name + " found in Network");
    }

    // Raylib Rendering Loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(LIGHT_BLUE); // Set background color

        // Heading: "My Friends"
        DrawText("My Friends", screenWidth / 2 - MeasureText("My Friends", 40) / 2, 40, 40, DARK_BLUE); // Title

        // Draw each message from the friends list
        int yOffset = 100;
        for (const string &msg : friendsList) {
            DrawText(msg.c_str(), screenWidth / 2 - MeasureText(msg.c_str(), 20) / 2, yOffset, 20, DARK_BLUE);
            yOffset += 30; // Space between messages
        }

        // Draw Back button (optional)
        Rectangle backButton = {screenWidth / 2 - 75, screenHeight - 80, 150, 40};
        DrawRectangleRec(backButton, BUTTON_WHITE);
        DrawText("Back", backButton.x + 55, backButton.y + 10, 20, DARK_BLUE);

        // Handle button click
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, backButton)) {
            // Go back to the user menu
            CloseWindow();  // Close the current window
            userMenu(name); // Call userMenu function to go back
            return; // Ensure the current function doesn't continue after calling userMenu
        }

        EndDrawing();
    }

    CloseWindow(); // Close window after the loop ends
}

// Function to show the list of connections (friends) of a particular user
void SocialNetwork::showConnections(int ID, vector<string>& friendsList) {
    if (users.find(ID) != users.end()) {
        User user = users[ID];
        for (int friendID : user.friends) {
            if (users.find(friendID) != users.end()) {
                friendsList.push_back("Name = " + users[friendID].name);
            }
        }
    }
    else {
        friendsList.push_back("Error: User not found in the network.");
    }
}

// Default constructor
SocialNetwork::SocialNetwork() {
    count = 0;
    loggedIn = -1;
}

void SocialNetwork::mainPage(SocialNetwork& network) {
    // Define colors
    Color LIGHT_BLUE = { 173, 216, 230, 255 };  // Light blue background
    Color DARK_BLUE = { 0, 0, 139, 255 };       // Dark blue for text and outlines
    Color BUTTON_WHITE = { 255, 255, 255, 255 }; // White for button background

    const int screenWidth = 1000;
    const int screenHeight = 700;
    InitWindow(screenWidth, screenHeight, "ConnectTo");

    // Set the target FPS
    SetTargetFPS(60);

    // Button properties for login, sign up, and exit buttons
    Rectangle loginButton = { screenWidth / 2 - 125, screenHeight / 2 - 70, 250, 60 };
    Rectangle signUpButton = { screenWidth / 2 - 125, screenHeight / 2 + 10, 250, 60 };
    Rectangle exitButton = { screenWidth / 2 - 125, screenHeight / 2 + 90, 250, 60 };

    bool exitProgram = false;

    // Main loop for login/signup screen
    while (!WindowShouldClose() && !exitProgram) {
        Vector2 mousePos = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mousePos, loginButton)) {
                network.userLogin(); // Call login function
            } else if (CheckCollisionPointRec(mousePos, signUpButton)) {
                network.userSignUp(); // Call signup function
            } else if (CheckCollisionPointRec(mousePos, exitButton)) {
                exitProgram = true;
            }
        }

        // Draw the UI
        BeginDrawing();
        ClearBackground(LIGHT_BLUE);  // Light blue background

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

    CloseWindow();  // Close window when done
}

// Function to read users from a file
void SocialNetwork::loadFromFile(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int userID, posts, comments, likes;
        string name;
        string password;
        string interest;
        vector<int> friends;

        // Read user ID and name
        ss >> userID >> name >> password >> interest >> posts >> comments >> likes;

        // Read friends (rest of the integers)
        int friendID;
        while (ss >> friendID) {
            friends.push_back(friendID);
        }

        // Add user to the network
        addUser(userID, name, password, interest, posts, comments, likes, friends);
        count++;
    }

    file.close();
}

void SocialNetwork::userLogin() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Login - ConnectTo");

    SetTargetFPS(60);

    // Input field properties
    const int maxInputLength = 20; // Maximum characters for username and password
    Rectangle usernameBox = { screenWidth / 2 - 200, 200, 400, 50 };
    Rectangle passwordBox = { screenWidth / 2 - 200, 280, 400, 50 };
    Rectangle loginButton = { screenWidth / 2 - 75, 380, 150, 40 };

    string usernameInput = "";
    string passwordInput = "";
    bool usernameActive = false;
    bool passwordActive = false;

    string errorMessage = "";
    bool loginSuccessful = false;

    // Custom Colors
    Color LIGHT_BLUE = { 173, 216, 230, 255 }; // Background color
    Color DARK_BLUE = { 0, 0, 139, 255 };      // Text and outlines
    Color BUTTON_WHITE = { 255, 255, 255, 255 }; // Button background color

    while (!WindowShouldClose() && !loginSuccessful) {
        // Mouse position
        Vector2 mousePos = GetMousePosition();

        // Handle clicks for activating input boxes
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            usernameActive = CheckCollisionPointRec(mousePos, usernameBox);
            passwordActive = CheckCollisionPointRec(mousePos, passwordBox);

            // If clicking outside input boxes, deactivate both
            if (!usernameActive && !passwordActive) {
                usernameActive = false;
                passwordActive = false;
            }
        }

        // Handle text input for username with a character limit
        if (usernameActive) {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 126 && usernameInput.length() < maxInputLength) {
                    usernameInput += static_cast<char>(key); // Append character if within limit
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !usernameInput.empty()) {
                usernameInput.pop_back();
            }
        }

        // Handle text input for password with a character limit
        if (passwordActive) {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 126 && passwordInput.length() < maxInputLength) {
                    passwordInput += static_cast<char>(key); // Append character if within limit
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !passwordInput.empty()) {
                passwordInput.pop_back();
            }
        }

        // Check if login button is clicked
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, loginButton)) {
            bool userFound = false;
            for (const auto &pair : users) {
                const User &user = pair.second;
                if (user.name == usernameInput) {
                    userFound = true;
                    if (user.password == passwordInput) {
                        errorMessage = "Login successful!";
                        loggedIn = user.userID;
                        loginSuccessful = true;
                    } else {
                        errorMessage = "Incorrect password!";
                    }
                    break;
                }
            }
            if (!userFound) {
                errorMessage = "Error: User does not exist!";
            }
        }

        // Draw interface
        BeginDrawing();
        ClearBackground(LIGHT_BLUE); // Set background color

        // Draw heading: "Login"
        DrawText("Login", screenWidth / 2 - MeasureText("Login", 40) / 2, 100, 40, DARK_BLUE);

        // Draw username box
        DrawRectangleRec(usernameBox, BUTTON_WHITE);
        if (usernameActive) DrawRectangleLinesEx(usernameBox, 2, DARK_BLUE);
        DrawText(("Username: " + usernameInput).c_str(), usernameBox.x + 5, usernameBox.y + 10, 20, DARK_BLUE);

        // Draw password box
        DrawRectangleRec(passwordBox, BUTTON_WHITE);
        if (passwordActive) DrawRectangleLinesEx(passwordBox, 2, DARK_BLUE);
        DrawText(("Password: " + string(passwordInput.size(), '*')).c_str(), passwordBox.x + 5, passwordBox.y + 10, 20, DARK_BLUE);

        // Draw login button
        DrawRectangleRec(loginButton, BUTTON_WHITE);
        DrawText("Login", loginButton.x + 50, loginButton.y + 15, 20, DARK_BLUE);

        // Highlight login button if hovered
        if (CheckCollisionPointRec(mousePos, loginButton)) {
            DrawRectangleLinesEx(loginButton, 2, DARK_BLUE);
        }

        // Draw error message (login success or failure)
        if (!errorMessage.empty()) {
            Color messageColor = (errorMessage == "Login successful!") ? DARKGREEN : RED;
            DrawText(errorMessage.c_str(), screenWidth / 2 - MeasureText(errorMessage.c_str(), 20) / 2, 450, 20, messageColor);
        }

        EndDrawing();
    }

    CloseWindow();

    if (loginSuccessful) {
        userMenu(usernameInput); // Proceed to user menu after successful login
    }
}

void SocialNetwork::userSignUp() {
    const int screenWidth = 800;  // Increased width
    const int screenHeight = 800;  // Increased height
    InitWindow(screenWidth, screenHeight, "Sign Up - ConnectTo");
    SetTargetFPS(60);

    // Custom Colors
    Color LIGHT_BLUE = {173, 216, 230, 255}; // RGB(173, 216, 230) for background
    Color DARK_BLUE = {0, 0, 139, 255};      // RGB(0, 0, 139) for text and outlines
    Color BUTTON_WHITE = {255, 255, 255, 255}; // RGB(255, 255, 255) for button background

    // Input fields for username and password
    Rectangle usernameBox = {screenWidth / 2 - 200, 200, 400, 50};
    Rectangle passwordBox = {screenWidth / 2 - 200, 280, 400, 50};
    Rectangle nextButton = {screenWidth / 2 - 75, 370, 150, 40}; // For proceeding to next screen

    // Input fields for interest selection
    const string options[] = {"Coding", "Cooking", "Dancing", "Gaming", "Painting", 
                              "Photography", "Reading", "Singing", "Traveling", "Writing", "Other"};
    string errorMessage = "";
    bool signUpSuccessful = false;
    bool onInterestScreen = false; // To switch between username/password screen and interest screen
    string usernameInput = "";
    string passwordInput = "";
    string interestInput = "";
    bool usernameActive = false;
    bool passwordActive = false;
    bool interestActive = false;

    // Position of the "Sign Up" button below the interest options
    float lastOptionY = 160 + (sizeof(options) / sizeof(options[0]) - 1) * 40;
    float signUpButtonY = lastOptionY + 40 + 20;  // 40 for option height, 20 for offset

    Rectangle signUpButton = {screenWidth / 2 - 75, signUpButtonY, 150, 40}; // Updated button position

    // Calculate Y position of the message below the sign-up button
    float messageY = signUpButton.y + signUpButton.height + 10; // 10 for offset

    while (!WindowShouldClose() && !signUpSuccessful) {
        Vector2 mousePos = GetMousePosition();

        // Handle mouse clicks to activate input boxes
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (!onInterestScreen) {
                usernameActive = CheckCollisionPointRec(mousePos, usernameBox);
                passwordActive = CheckCollisionPointRec(mousePos, passwordBox);
            } else {
                interestActive = false;
                // Handle clicks on interest options
                for (size_t i = 0; i < sizeof(options) / sizeof(options[0]); i++) {
                    Rectangle optionBox = {screenWidth / 2 - 200, static_cast<float>(160 + i * 40), 400, 40}; // Adjust position based on option index
                    if (CheckCollisionPointRec(mousePos, optionBox)) {
                        interestInput = options[i];
                    }
                }
            }
        }

        // Username input
        if (!onInterestScreen && usernameActive) {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 126 && usernameInput.length() < 20) {
                    usernameInput += static_cast<char>(key);
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !usernameInput.empty()) {
                usernameInput.pop_back();
            }
        }

        // Password input
        if (!onInterestScreen && passwordActive) {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 126 && passwordInput.length() < 7) {
                    passwordInput += static_cast<char>(key);
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !passwordInput.empty()) {
                passwordInput.pop_back();
            }
        }

        // Interest input
        if (onInterestScreen && interestActive) {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 126 && interestInput.length() < 20) {
                    interestInput += static_cast<char>(key);
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !interestInput.empty()) {
                interestInput.pop_back();
            }
        }

        // Next button click (proceed to interest selection)
        if (!onInterestScreen && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, nextButton)) {
            if (usernameInput.empty() || passwordInput.empty()) {
                errorMessage = "Error: All fields must be filled!";
            } else if (passwordInput.length() < 5 || passwordInput.length() > 7) {
                errorMessage = "Password must be 5-7 characters.";
            } else {
                bool hasAlphabet = false;
                bool hasNumber = false;

                // Check if password contains at least one alphabet and one number
                for (char c : passwordInput) {
                    if (isalpha(c)) {
                        hasAlphabet = true;
                    }
                    if (isdigit(c)) {
                        hasNumber = true;
                    }
                }

                // If no alphabet or number is found, set an error message
                if (!hasAlphabet || !hasNumber) {
                    errorMessage = "Password must contain at least one alphabet and one number.";
                } else {
                    bool usernameExists = false;
                    for (const auto &userPair : users) {
                        if (userPair.second.name == usernameInput) {
                            usernameExists = true;
                            break;
                        }
                    }
                    if (usernameExists) {
                        errorMessage = "Username already exists!";
                    } else {
                        onInterestScreen = true;
                        errorMessage = ""; // Clear error message when proceeding
                    }
                }
            }
        }

        // Sign up button click (final sign up)
        if (onInterestScreen && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, signUpButton)) {
            bool validInterest = false;
            for (const string &option : options) {
                if (option == interestInput) {
                    validInterest = true;
                    break;
                }
            }
            if (!validInterest) {
                errorMessage = "Interest must match one of the options.";
            } else {
                // All inputs are valid, perform sign-up
                int newUserID = totalUsers() + 1;
                User newUser(newUserID, usernameInput, passwordInput, interestInput, 0, 0, 0, {});
                users[newUserID] = newUser;
                loggedIn = newUserID;
                saveUserFile("users.txt", newUser);
                errorMessage = "Sign-up successful!";
                signUpSuccessful = true;
            }
        }

        // Draw interface
        BeginDrawing();
        ClearBackground(LIGHT_BLUE);

        if (!onInterestScreen) {
            DrawText("Sign Up", screenWidth / 2 - MeasureText("Sign Up", 40) / 2, 100, 40, DARK_BLUE);

            // Draw username box
            DrawRectangleRec(usernameBox, BUTTON_WHITE);
            if (usernameActive) DrawRectangleLinesEx(usernameBox, 2, DARK_BLUE);
            DrawText(("Username: " + usernameInput).c_str(), usernameBox.x + 5, usernameBox.y + 10, 20, DARK_BLUE);

            // Draw password box
            DrawRectangleRec(passwordBox, BUTTON_WHITE);
            if (passwordActive) DrawRectangleLinesEx(passwordBox, 2, DARK_BLUE);
            DrawText(("Password: " + string(passwordInput.size(), '*')).c_str(), passwordBox.x + 5, passwordBox.y + 10, 20, DARK_BLUE);

            // Next button
            DrawRectangleRec(nextButton, BUTTON_WHITE);
            DrawText("Next", nextButton.x + 50, nextButton.y + 10, 20, DARK_BLUE);
        } else {
            DrawText("Choose Your Interest", screenWidth / 2 - MeasureText("Choose Your Interest", 40) / 2, 100, 40, DARK_BLUE);

            // Display options
            for (size_t i = 0; i < sizeof(options) / sizeof(options[0]); i++) {
                Rectangle optionBox = {screenWidth / 2 - 200, static_cast<float>(160 + i * 40), 400, 40}; // Adjust position based on option index
                DrawRectangleRec(optionBox, BUTTON_WHITE);
                if (interestInput == options[i]) {
                    DrawRectangleLinesEx(optionBox, 2, DARK_BLUE);
                }
                DrawText(options[i].c_str(), optionBox.x + 5, optionBox.y + 10, 20, DARK_BLUE);
            }

            // Sign up button
            DrawRectangleRec(signUpButton, BUTTON_WHITE);
            DrawText("Sign Up", signUpButton.x + 35, signUpButton.y + 10, 20, DARK_BLUE);
        }

        // Display error or success message below the sign-up button
        if (!errorMessage.empty()) {
            Color messageColor = signUpSuccessful ? GREEN : RED;
            DrawText(errorMessage.c_str(), screenWidth / 2 - MeasureText(errorMessage.c_str(), 20) / 2, messageY, 20, messageColor);
        }

        EndDrawing();
    }

    CloseWindow();

    if (signUpSuccessful) {
        userMenu(usernameInput); // Proceed to user menu after successful signup
    }
}

void SocialNetwork::changeInterest(int userID) {
    if (users.find(userID) == users.end()) {
        return; // Exit if userID is not valid
    }

    const int screenWidth = 800;  // Increased window width
    const int screenHeight = 700; // Increased window height
    InitWindow(screenWidth, screenHeight, "Change Interest");
    SetTargetFPS(60);

    // Custom colors
    Color LIGHT_BLUE = {173, 216, 230, 255};
    Color DARK_BLUE = {0, 0, 139, 255};
    Color BUTTON_WHITE = {255, 255, 255, 255};
    Color SUCCESS_GREEN = {0, 200, 0, 255};

    const string options[] = {"Coding", "Cooking", "Dancing", "Gaming", "Painting",
                              "Photography", "Reading", "Singing", "Traveling", "Writing", "Other"};

    bool updated = false;
    string selectedInterest = users[userID].interest;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(LIGHT_BLUE);

        // Heading
        DrawText("Change Your Interest", screenWidth / 2 - MeasureText("Change Your Interest", 40) / 2, 40, 40, DARK_BLUE);

        // Options Box
        int optionsStartY = 120; // Starting Y position of options
        Rectangle optionBox;

        for (size_t i = 0; i < sizeof(options) / sizeof(options[0]); i++) {
            optionBox = {screenWidth / 2 - 200, static_cast<float>(optionsStartY + i * 40), 400, 40};

            DrawRectangleRec(optionBox, BUTTON_WHITE);

            if (selectedInterest == options[i]) {
                DrawRectangleLinesEx(optionBox, 2, DARK_BLUE); // Highlight selected interest
            }
            DrawText(options[i].c_str(), optionBox.x + 10, optionBox.y + 10, 20, DARK_BLUE);

            // Handle option click
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), optionBox)) {
                selectedInterest = options[i];
            }
        }

        // Success message
        if (updated) {
            DrawText("Interest updated successfully!", screenWidth / 2 - MeasureText("Interest updated successfully!", 20) / 2,
                     screenHeight - 150, 20, SUCCESS_GREEN);
        }

        // Update Button
        Rectangle updateButton = {screenWidth / 2 - 180, screenHeight - 80, 150, 40};
        DrawRectangleRec(updateButton, BUTTON_WHITE);
        DrawText("Update", updateButton.x + 45, updateButton.y + 10, 20, DARK_BLUE);

        // Back Button
        Rectangle backButton = {screenWidth / 2 + 30, screenHeight - 80, 150, 40};
        DrawRectangleRec(backButton, BUTTON_WHITE);
        DrawText("Back", backButton.x + 55, backButton.y + 10, 20, DARK_BLUE);

        // Handle Update Button
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), updateButton)) {
            if (!selectedInterest.empty() && selectedInterest != users[userID].interest) {
                users[userID].interest = selectedInterest; // Update interest
                updateUsersFile("users.txt");             // Save updated data
                updated = true;
            }
        }

        // Handle Back Button
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), backButton)) {
            CloseWindow();          // Close window
            Profile(users[userID].name); // Return to Profile
            return;
        }

        EndDrawing();
    }

    CloseWindow();
    Profile(users[userID].name); // Return to Profile
}

void SocialNetwork::changePassword(int userID) {
    if (users.find(userID) == users.end()) {
        return; // Exit if userID is not valid
    }

    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Change Password");
    SetTargetFPS(60);

    // Custom colors
    Color LIGHT_BLUE = {173, 216, 230, 255};
    Color DARK_BLUE = {0, 0, 139, 255};
    Color BUTTON_WHITE = {255, 255, 255, 255};

    string newPassword;
    int key = 0;
    bool updated = false;
    bool passwordValid = true;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(LIGHT_BLUE);

        // Heading
        DrawText("Change Password", screenWidth / 2 - MeasureText("Change Password", 40) / 2, 40, 40, DARK_BLUE);

        // Input box for password
        DrawRectangle(50, 150, 700, 50, WHITE);
        DrawText("Enter your new password:", 50, 120, 20, DARK_BLUE);
        DrawText(newPassword.c_str(), 60, 165, 20, DARK_BLUE);

        // Display error message
        if (!passwordValid) {
            DrawText("Password must be 5 to 7 characters, contain at least 1 letter and 1 digit!", 
                     screenWidth / 2 - MeasureText("Password must be 5 to 7 characters, contain at least 1 letter and 1 digit!", 20) / 2, 
                     220, 20, RED);
        }

        // Back Button
        Rectangle backButton = {screenWidth / 2 - 75, screenHeight - 80, 150, 40};
        DrawRectangleRec(backButton, BUTTON_WHITE);
        DrawText("Back", backButton.x + 55, backButton.y + 10, 20, DARK_BLUE);

        // Update Button
        Rectangle updateButton = {screenWidth / 2 - 75, screenHeight - 130, 150, 40};
        DrawRectangleRec(updateButton, BUTTON_WHITE);
        DrawText("Update", updateButton.x + 45, updateButton.y + 10, 20, DARK_BLUE);

        // Handle input
        key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 125) {
                newPassword += (char)key; // Add the key to the string
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && newPassword.length() > 0) {
            newPassword.pop_back();
        }

        // Handle Back Button
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, backButton)) {
            CloseWindow();          // Close the window
            Profile(users[userID].name); // Return to Profile
            return;
        }

        // Handle Update Button
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, updateButton)) {
            bool containsLetter = false, containsDigit = false;

            // Check for at least one letter and one digit
            for (char c : newPassword) {
                if (isalpha(c)) containsLetter = true;
                if (isdigit(c)) containsDigit = true;
            }

            // Validate length and conditions
            if (newPassword.length() >= 5 && newPassword.length() <= 7 && containsLetter && containsDigit) {
                users[userID].password = newPassword; // Update password
                updateUsersFile("users.txt"); // Save the updated password to the file
                updated = true;  // Set updated flag to true
                passwordValid = true;  // Valid password
            } else {
                passwordValid = false;  // Invalid password
            }
        }

        // Display success message if updated
        if (updated) {
            DrawText("Password updated successfully!", screenWidth / 2 - MeasureText("Password updated successfully!", 20) / 2, 300, 20, GREEN);
        }

        EndDrawing();
    }

    CloseWindow();
    Profile(users[userID].name); // Return to Profile
}

void SocialNetwork::Profile(const string& targetUserName) {
    int userIDToShow = loggedIn; // Default to the logged-in user
    bool userFound = false;

    if (!targetUserName.empty()) {
        for (const auto& pair : users) {
            if (pair.second.name == targetUserName) {
                userIDToShow = pair.first;
                userFound = true;
                break;
            }
        }
    } else {
        userFound = true;
    }

    if (!userFound) {
        InitWindow(600, 400, "Error");
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(WHITE);
            DrawText(("Error: User " + targetUserName + " not found!").c_str(), 20, 20, 20, RED);
            DrawText("Press [ESC] to go back.", 20, 60, 20, DARKGRAY);
            EndDrawing();
            if (IsKeyPressed(KEY_ESCAPE)) break;
        }
        CloseWindow();
        return;
    }

    const int screenWidth = 1000; // Increased screen width
    const int screenHeight = 800; // Increased screen height
    InitWindow(screenWidth, screenHeight, "User Profile");
    SetTargetFPS(60);

    Color LIGHT_BLUE = {173, 216, 230, 255};
    Color DARK_BLUE = {0, 0, 139, 255};
    Color BUTTON_WHITE = {255, 255, 255, 255};
    Color BUTTON_LIGHT_GRAY = {211, 211, 211, 255}; // Different background for second button

    const User& user = users.at(userIDToShow);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(LIGHT_BLUE);

        // Display user profile details
        DrawText("Profile", screenWidth / 2 - MeasureText("User Profile", 40) / 2, 40, 40, DARK_BLUE);
        DrawText(("Username: " + user.name).c_str(), 50, 120, 20, DARK_BLUE);
        DrawText(("Posts: " + to_string(user.posts)).c_str(), 50, 160, 18, DARK_BLUE);
        DrawText(("Likes: " + to_string(user.likes)).c_str(), 50, 200, 18, DARK_BLUE);
        DrawText(("Interest: " + user.interest).c_str(), 50, 240, 18, DARK_BLUE);

        // Display friends list
        DrawText("Friends List:", 50, 280, 18, DARK_BLUE);
        int yOffset = 310;
        if (user.friends.empty()) {
            DrawText("You don't have any friends yet.", 70, yOffset, 18, DARK_BLUE);
        } else {
            for (int friendID : user.friends) {
                if (users.find(friendID) != users.end()) {
                    DrawText(("- " + users.at(friendID).name).c_str(), 70, yOffset, 18, DARK_BLUE);
                    yOffset += 30;
                }
            }
        }

        // Buttons - Moved to the bottom of the window
        Rectangle changePasswordBtn = {50.0f, static_cast<float>(screenHeight - 140), 250.0f, 40.0f}; 
        Rectangle changeInterestBtn = {300.0f, static_cast<float>(screenHeight - 140), 250.0f, 40.0f}; 
        Rectangle backButton = {550, static_cast<float>(screenHeight - 140), 150, 40};

        // Change Password Button
        DrawRectangleRec(changePasswordBtn, BUTTON_WHITE);
        DrawText("Change Password", changePasswordBtn.x + 10, changePasswordBtn.y + 10, 20, DARK_BLUE);

        // Change Interest Button - Different color background
        DrawRectangleRec(changeInterestBtn, BUTTON_LIGHT_GRAY);
        DrawText("Change Interest", changeInterestBtn.x + 10, changeInterestBtn.y + 10, 20, DARK_BLUE);

        // Back Button
        DrawRectangleRec(backButton, BUTTON_WHITE);
        DrawText("Back", backButton.x + 55, backButton.y + 10, 20, DARK_BLUE);

        Vector2 mousePos = GetMousePosition();

        // Handle button clicks
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mousePos, changePasswordBtn)) {
                changePassword(userIDToShow);
            } else if (CheckCollisionPointRec(mousePos, changeInterestBtn)) {
                changeInterest(userIDToShow);
            } else if (CheckCollisionPointRec(mousePos, backButton)) {
                CloseWindow();
                userMenu(user.name);
                return;
            }
        }

        EndDrawing();
    }

    CloseWindow();
}

void SocialNetwork::ProfileSearch(const string& targetUserName) {
    int userIDToShow = loggedIn; // Default to the logged-in user
    bool userFound = false;

    if (!targetUserName.empty()) {
        for (const auto& pair : users) {
            if (pair.second.name == targetUserName) {
                userIDToShow = pair.first;
                userFound = true;
                break;
            }
        }
    } else {
        userFound = true;
    }

    if (!userFound) {
        InitWindow(600, 400, "Error");
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(WHITE);
            DrawText(("Error: User " + targetUserName + " not found!").c_str(), 20, 20, 20, RED);
            DrawText("Press [ESC] to go back.", 20, 60, 20, DARKGRAY);
            EndDrawing();
            if (IsKeyPressed(KEY_ESCAPE)) break;
        }
        CloseWindow();
        return;
    }

    const int screenWidth = 1000; // Increased screen width
    const int screenHeight = 800; // Increased screen height
    InitWindow(screenWidth, screenHeight, "User Profile");
    SetTargetFPS(60);

    Color LIGHT_BLUE = {173, 216, 230, 255};
    Color DARK_BLUE = {0, 0, 139, 255};
    Color BUTTON_WHITE = {255, 255, 255, 255};

    const User& user = users.at(userIDToShow);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(LIGHT_BLUE);

        // Display user profile details
        DrawText("Profile", screenWidth / 2 - MeasureText("User Profile", 40) / 2, 40, 40, DARK_BLUE);
        DrawText(("Username: " + user.name).c_str(), 50, 120, 20, DARK_BLUE);
        DrawText(("Posts: " + to_string(user.posts)).c_str(), 50, 160, 18, DARK_BLUE);
        DrawText(("Likes: " + to_string(user.likes)).c_str(), 50, 200, 18, DARK_BLUE);
        DrawText(("Interest: " + user.interest).c_str(), 50, 240, 18, DARK_BLUE);

        // Display friends list
        DrawText("Friends List:", 50, 280, 18, DARK_BLUE);
        int yOffset = 310;
        if (user.friends.empty()) {
            DrawText("Don't have any friends yet.", 70, yOffset, 18, DARK_BLUE);
        } else {
            for (int friendID : user.friends) {
                if (users.find(friendID) != users.end()) {
                    DrawText(("- " + users.at(friendID).name).c_str(), 70, yOffset, 18, DARK_BLUE);
                    yOffset += 30;
                }
            }
        }

        // Buttons - Moved to the bottom of the window
        Rectangle backButton = {550, static_cast<float>(screenHeight - 140), 150, 40};

        // Back Button
        DrawRectangleRec(backButton, BUTTON_WHITE);
        DrawText("Back", backButton.x + 55, backButton.y + 10, 20, DARK_BLUE);

        Vector2 mousePos = GetMousePosition();

        // Handle button clicks
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mousePos, backButton)) {
                CloseWindow();
                userMenu(user.name);
                return;
            }
        }

        EndDrawing();
    }

    CloseWindow();
}

vector<string> SocialNetwork::findShortestPathInNetwork(const string &startInput, const string &endInput) {
    // Find user IDs based on usernames
    int startUser = -1;
    int endUser = -1;

    for (const auto &entry : users) {
        if (entry.second.name == startInput) {
            startUser = entry.first;
        }
        if (entry.second.name == endInput) {
            endUser = entry.first;
        }
    }

    // If either username is invalid
    if (startUser == -1 || endUser == -1) {
        return {}; // Return empty path if either user does not exist
    }

    // BFS to find shortest path
    queue<int> q;
    unordered_map<int, int> distances;
    unordered_map<int, int> predecessors;

    // Initialize distances to -1
    for (const auto &userPair : users) {
        distances[userPair.first] = -1;
    }

    q.push(startUser);
    distances[startUser] = 0;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        // Explore all friends (neighbors) of the current user
        for (int neighbor : users[current].friends) {
            if (distances[neighbor] == -1) {
                q.push(neighbor);
                distances[neighbor] = distances[current] + 1;
                predecessors[neighbor] = current;
            }
        }
    }

    // If no connection exists
    if (distances[endUser] == -1) {
        return {};
    }

    // Reconstruct the path using predecessors
    vector<int> path;
    for (int at = endUser; at != startUser; at = predecessors[at]) {
        path.push_back(at);
    }
    path.push_back(startUser);

    reverse(path.begin(), path.end());

    // Convert user IDs to usernames for the result
    vector<string> result;
    for (int userID : path) {
        result.push_back(users[userID].name);
    }

    return result;
}

void SocialNetwork::findShortestPath(const string &startInput) {
    const int screenWidth = 900;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Ties of Connections - ConnectTo");

    SetTargetFPS(60);

    // Input field properties
    const int maxInputLength = 20; // Maximum characters for username
    Rectangle endBox = { screenWidth / 2 - 200, 200, 400, 50 };
    Rectangle findButton = { screenWidth / 2 - 75, 300, 150, 40 };
    Rectangle backButton = { screenWidth / 2 - 75, 450, 150, 40 }; // Back button, moved below the path

    string endInput = "";
    bool endActive = false;

    string resultMessage = "";

    // Custom Colors
    Color LIGHT_BLUE = { 173, 216, 230, 255 }; // Background color
    Color DARK_BLUE = { 0, 0, 139, 255 };      // Text and outlines
    Color BUTTON_WHITE = { 255, 255, 255, 255 }; // Button background color

    while (!WindowShouldClose()) {
        // Mouse position
        Vector2 mousePos = GetMousePosition();

        // Handle clicks for activating input box
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            endActive = CheckCollisionPointRec(mousePos, endBox);

            // If clicking outside input box, deactivate it
            if (!endActive) {
                endActive = false;
            }
        }

        // Handle text input for ending username
        if (endActive) {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 126 && endInput.length() < maxInputLength) {
                    endInput += static_cast<char>(key); // Append character if within limit
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !endInput.empty()) {
                endInput.pop_back();
            }
        }

        // Check if Back button is clicked
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, backButton)) {
            CloseWindow();
            userMenu(startInput); // Return to the menu immediately
            return;
        }

        // Check if Find Path button is clicked
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, findButton)) {
            if (endInput.empty()) {
                resultMessage = "Error: End username is required!";
            } else if (startInput == endInput) {
                resultMessage = "Error: You cannot enter your own username!";
            } else {
                vector<string> path = findShortestPathInNetwork(startInput, endInput);
                if (path.empty() || path.size() == 1) {
                    resultMessage = "No path found between users.";
                } else if (path.size() == 2 || path.size() == 3) {
                    resultMessage = "You are connected to " + endInput + " directly.";
                } else {
                    resultMessage = "You are connected to " + endInput + " through:\n";

                    // Flag to prevent adding startInput and endInput to the path
                    bool first = true;
                    for (const auto &username : path) {
                        // Skip adding startInput and endInput in the path
                        if (username == startInput || username == endInput) continue;

                        // If it's the first valid username in the path, skip it
                        if (first) {
                            first = false;
                            continue;
                        }

                        resultMessage += username + " -> ";
                    }

                    // Remove the last " -> " (arrow) from the path
                    if (resultMessage.length() > 0 && resultMessage[resultMessage.length() - 4] == ' ' &&
                        resultMessage[resultMessage.length() - 3] == '-' && resultMessage[resultMessage.length() - 2] == '>') {
                        resultMessage = resultMessage.substr(0, resultMessage.length() - 4); // Remove the last " -> "
                    }
                }
            }
        }

        // Draw interface
        BeginDrawing();
        ClearBackground(LIGHT_BLUE); // Set background color

        // Draw heading: "Find Shortest Path"
        DrawText("Ties of Connections", screenWidth / 2 - MeasureText("Ties of Connections", 40) / 2, 100, 40, DARK_BLUE);

        // Draw ending username box
        DrawRectangleRec(endBox, BUTTON_WHITE);
        if (endActive) DrawRectangleLinesEx(endBox, 2, DARK_BLUE);
        DrawText(("End: " + endInput).c_str(), endBox.x + 5, endBox.y + 10, 20, DARK_BLUE);

        // Draw Find Path button
        DrawRectangleRec(findButton, BUTTON_WHITE);
        DrawText("Find Path", findButton.x + 30, findButton.y + 10, 20, DARK_BLUE);

        // Highlight Find Path button if hovered
        if (CheckCollisionPointRec(mousePos, findButton)) {
            DrawRectangleLinesEx(findButton, 2, DARK_BLUE);
        }

        // Draw result message (path or error message)
        if (!resultMessage.empty()) {
            Color messageColor = (resultMessage.find("Error") != string::npos || resultMessage.find("No path") != string::npos) ? RED : DARK_BLUE;
            DrawText(resultMessage.c_str(), screenWidth / 2 - MeasureText(resultMessage.c_str(), 20) / 2, 370, 20, messageColor);
        }

        // Draw Back button
        DrawRectangleRec(backButton, BUTTON_WHITE);
        DrawText("Back", backButton.x + 50, backButton.y + 10, 20, DARK_BLUE);

        // Highlight Back button if hovered
        if (CheckCollisionPointRec(mousePos, backButton)) {
            DrawRectangleLinesEx(backButton, 2, DARK_BLUE);
        }

        EndDrawing();
    }

    CloseWindow();
}
    
vector<string> SocialNetwork::recommendFriendsByInterest(const string &username) {
vector<string> recommendations;

int userID = -1;
for (const auto &entry : users) {
    if (entry.second.name == username) {
        userID = entry.second.userID;
        break;
    }
}
if (userID == -1) {
    recommendations.push_back("Error: User \"" + username + "\" not found.");
    return recommendations;
}

string userInterest = users[userID].interest;
for (const auto &entry : users) {
    int potentialFriendID = entry.first;
    if (potentialFriendID != userID && 
        find(users[userID].friends.begin(), users[userID].friends.end(), potentialFriendID) == users[userID].friends.end()) {
        if (entry.second.interest == userInterest) {
            recommendations.push_back("Name: " + entry.second.name + " (Interest: " + entry.second.interest + ")");
        }
    }
}

if (recommendations.empty()) {
    recommendations.push_back("No recommendations available based on interests.");
}
return recommendations;
}

vector<string> SocialNetwork::mostInfluential() {
vector<string> influencers;

map<int, int> inDegree;
for (const auto &userPair : users) {
    const User &user = userPair.second;
    for (int friendID : user.friends) {
        inDegree[friendID]++;
    }
}

vector<pair<int, int>> inDegreeVector;
for (const auto &entry : inDegree) {
    inDegreeVector.push_back({entry.first, entry.second});
}
sort(inDegreeVector.begin(), inDegreeVector.end(), [](const pair<int, int> &a, const pair<int, int> &b) {
     return b.second < a.second;
});

for (size_t i = 0; i < 3 && i < inDegreeVector.size(); ++i) {
    int userID = inDegreeVector[i].first;
    influencers.push_back("Name: " + users[userID].name + " (Followers: " + to_string(inDegreeVector[i].second) + ")");
}

if (influencers.empty()) {
     influencers.push_back("No influencers found in the network.");
}
return influencers;
}

vector<string> SocialNetwork::analyzeUserBehavior() {
vector<string> behavior;

map<string, int> postFrequency;
int maxPosts = 0, maxLikes = 0;
string topposts, mosttrending;

for (const auto &entry : users) {
    const User &user = entry.second;
    if (user.posts > maxPosts) {
        maxPosts = user.posts;
        topposts = user.name;
    }
    if (user.likes > maxLikes) {
        maxLikes = user.likes;
        mosttrending = user.name;
    }
}

behavior.push_back("Frequent Poster: " + topposts + " with " + to_string(maxPosts) + " posts.");
behavior.push_back("Trending User: " + mosttrending + " with " + to_string(maxLikes) + " likes.");
return behavior;
}

void SocialNetwork::recommendFriends(const string &username) {
    vector<string> combinedResults;

    // ------------------- Mutual Friend Recommendation -------------------- //
    int userID = -1;
    for (const auto &entry : users) {
        if (entry.second.name == username) {
            userID = entry.second.userID;
            break;
        }
    }

    if (userID == -1) {
        combinedResults.push_back("Error: User \"" + username + "\" not found.");
    } else {
        map<int, int> recommendations; // Potential friends based on mutual friends
        vector<int> &userFriends = users[userID].friends;

        for (int friendID : userFriends) {
            for (int potentialFriend : users[friendID].friends) {
                if (potentialFriend != userID &&
                    find(userFriends.begin(), userFriends.end(), potentialFriend) == userFriends.end()) {
                    recommendations[potentialFriend]++;
                }
            }
        }

        combinedResults.push_back("---------------- Mutual Friend Recommendations ----------------");
        if (recommendations.empty()) {
            combinedResults.push_back("No mutual friend recommendations available.");
        } else {
            for (const auto &rec : recommendations) {
                if (users.find(rec.first) != users.end()) {
                    string recommendedName = users[rec.first].name;
                    combinedResults.push_back("Name: " + recommendedName +
                                              " (Mutual Friends: " + to_string(rec.second) + ")");
                }
            }
        }
    }

    // ------------------- Interest-Based Recommendations ------------------- //
    vector<string> interestRecommendations = recommendFriendsByInterest(username);
    combinedResults.push_back("---------------- Interest-Based Recommendations ----------------");
    if (interestRecommendations.empty()) {
        combinedResults.push_back("No interest-based recommendations available.");
    } else {
        combinedResults.insert(combinedResults.end(), interestRecommendations.begin(), interestRecommendations.end());
    }

    // ------------------- Most Influential Users ------------------- //
    vector<string> influencerRecommendations = mostInfluential();
    combinedResults.push_back("---------------- Most Influential Users ----------------");
    if (influencerRecommendations.empty()) {
        combinedResults.push_back("No influential users available.");
    } else {
        combinedResults.insert(combinedResults.end(), influencerRecommendations.begin(), influencerRecommendations.end());
    }

    // ------------------- User Behavior Analysis ------------------- //
    vector<string> behaviorAnalysis = analyzeUserBehavior();
    combinedResults.push_back("---------------- Active User Recommendations ----------------");
    if (behaviorAnalysis.empty()) {
        combinedResults.push_back("No behavior-based recommendations available.");
    } else {
        combinedResults.insert(combinedResults.end(), behaviorAnalysis.begin(), behaviorAnalysis.end());
    }

    // --------------------------- RAYLIB DISPLAY --------------------------- //
    const int screenWidth = 1000;  // Increased window width
    const int screenHeight = 700;  // Increased window height

    InitWindow(screenWidth, screenHeight, "Friend Recommendations");
    SetTargetFPS(60);

    // Define custom colors
    Color LIGHT_BLUE = {173, 216, 230, 255};  // Custom light blue color
    Color DARK_BLUE = {0, 0, 139, 255};      // Custom dark blue color
    Color BUTTON_WHITE = {255, 255, 255, 255}; // White button color

   // Back Button Logic
    Rectangle backButton = {screenWidth - 120 - 10, screenHeight - 60, 120, 40}; // Position the button at the right corner


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(LIGHT_BLUE);  // Use custom light blue background

        // Title
        DrawText("Friend Recommendations", 20, 20, 30, DARK_BLUE);  // Increased font size for title

        // Display Recommendations
        int y = 60;
        for (size_t i = 0; i < combinedResults.size(); ++i) {
            if (y < screenHeight - 100) { // Prevent overflow below the back button
                DrawText(combinedResults[i].c_str(), 20, y, 18, DARK_BLUE);  // Increased font size for recommendations
                y += 30; // Increased spacing
            } else {
                DrawText("... More recommendations exist.", 20, y, 18, DARK_BLUE);  // Increased font size
                break;
            }
        }

        // Draw Back Button
        DrawRectangleRec(backButton, BUTTON_WHITE);  // Use custom white button color
        DrawText("Back", backButton.x + 30, backButton.y + 10, 20, DARK_BLUE);  // Increased font size for back button

        // Button Logic
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(GetMousePosition(), backButton)) {
            // Go back to the user menu
            CloseWindow();  // Close the current window
            userMenu(username); // Call userMenu function to go back
            return; // Ensure the current function doesn't continue after calling userMenu
        }

        EndDrawing();
    }

    CloseWindow();
    userMenu(username); // Call userMenu function to go back
}

void SocialNetwork::userMenu(string name) {
    // Define colors
    Color LIGHT_BLUE = { 173, 216, 230, 255 };  // Light blue background
    Color DARK_BLUE = { 0, 0, 139, 255 };       // Dark blue for text and outlines
    Color BUTTON_WHITE = { 255, 255, 255, 255 }; // White for button background

    const int screenWidth = 1000;  // Screen width
    const int screenHeight = 700;  // Screen height
    InitWindow(screenWidth, screenHeight, ("Hi, " + name).c_str());

    SetTargetFPS(60);

    // Menu options
    vector<string> options = {
        "1. My Profile",
        "2. Follow People",
        "3. Unfollow People",
        "4. Check Connections",
        "5. Search Accounts",
        "6. Recommend Friends",
        "7. My Friends",
        "8. Log Out",
        "9. Exit"
    };

    int selectedOption = -1;

    // Button properties
    vector<Rectangle> buttons;
    int buttonWidth = 300;  // Adjusted button width
    int buttonHeight = 50;  // Adjusted button height
    int buttonSpacing = 15; // Reduced spacing
    int startY = 100;       // Start position for buttons

    for (size_t i = 0; i < options.size(); i++) {
        buttons.push_back({ (float)(screenWidth / 2 - buttonWidth / 2), 
                            (float)(startY + i * (buttonHeight + buttonSpacing)), 
                            (float)buttonWidth, 
                            (float)buttonHeight });
    }

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();

        // Check for button clicks
        for (size_t i = 0; i < buttons.size(); i++) {
            if (CheckCollisionPointRec(mousePos, buttons[i]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                selectedOption = i + 1;
                break;
            }
        }

        if (selectedOption != -1) {
            switch (selectedOption) {
                case 1:
                    Profile(name);
                    break;
                case 2: {
                    followUser(name);
                    break;
                }
                case 3: {
                    unfollowUser(name);
                    break;
                }
                case 4: {
                    findShortestPath(name);
                    break;
                }
                case 5: {
                    searchUsers(name);
                    break;
                }
                case 6:
                    recommendFriends(name);
                    break;
                case 7:
                    friendsUser(name);
                    break;
                case 8:  // Log Out
                    CloseWindow();
                    mainPage(*this);  // Go back to main page (login/signup)
                    return;
                case 9:  // Exit
                    CloseWindow();
                    exit(0);
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    break;
            }

            selectedOption = -1; // Reset selected option
        }

        // Draw menu
        BeginDrawing();
        ClearBackground(LIGHT_BLUE);  // Light blue background

        // Shortened title text
        string welcomeText = "Hi, " + name + "!";
        int textWidth = MeasureText(welcomeText.c_str(), 40);  // Measure text width
        DrawText(welcomeText.c_str(), (screenWidth - textWidth) / 2, 40, 40, DARK_BLUE);  // Title text

        for (size_t i = 0; i < buttons.size(); i++) {
            DrawRectangleRec(buttons[i], BUTTON_WHITE);  // Button background
            DrawText(options[i].c_str(), buttons[i].x + 20, buttons[i].y + 15, 20, DARK_BLUE);  // Button text

            // Highlight button if hovered
            if (CheckCollisionPointRec(mousePos, buttons[i])) {
                DrawRectangleLinesEx(buttons[i], 2, DARK_BLUE);  // Button outline
            }
        }

        EndDrawing();
    }

    CloseWindow();
}

void SocialNetwork::searchUsers(const string currentUserName) {
    // Input Screen State
    enum ScreenState { INPUT, RESULTS, USER_ACTIONS };
    ScreenState state = INPUT;

    char inputBuffer[64] = {0};
    vector<string> matchingUsers;
    string selectedUser;

    Rectangle searchButton = {150.0f, 150.0f, 100.0f, 40.0f}; // Cast to float
    Rectangle backButton = {280.0f, 150.0f, 100.0f, 40.0f}; // Cast to float

    const int maxInputLength = 20; // Limit to 20 characters

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground({173, 216, 230, 255});

        int screenWidth = GetScreenWidth();
        Vector2 mousePos = GetMousePosition(); // Get mouse position for hover detection

        if (state == INPUT) {
            // Input Screen
            DrawText("Enter the username to search:", screenWidth / 2 - MeasureText("Enter the username to search:", 20) / 2, 20, 20, {0, 0, 139, 255});
            DrawRectangle(screenWidth / 2 - 150, 60, 300, 40, WHITE);
            DrawText(inputBuffer, screenWidth / 2 - MeasureText(inputBuffer, 20) / 2, 70, 20, DARKBLUE);

            // Buttons
            DrawRectangleRec(searchButton, WHITE);
            DrawText("Search", searchButton.x + (searchButton.width - MeasureText("Search", 20)) / 2, searchButton.y + 10, 20, DARKBLUE);

            DrawRectangleRec(backButton, WHITE);
            DrawText("Back", backButton.x + (backButton.width - MeasureText("Back", 20)) / 2, backButton.y + 10, 20, DARKBLUE);

            // Draw button outlines when hovered
            if (CheckCollisionPointRec(mousePos, searchButton)) {
                DrawRectangleLinesEx(searchButton, 2, DARKBLUE); // Draw outline for search button
            }
            if (CheckCollisionPointRec(mousePos, backButton)) {
                DrawRectangleLinesEx(backButton, 2, DARKBLUE); // Draw outline for back button
            }

            // Handle Input
            int key = GetCharPressed();
            if (key >= 32 && key <= 126 && strlen(inputBuffer) < maxInputLength) {
                size_t len = strlen(inputBuffer);
                inputBuffer[len] = (char)key;
                inputBuffer[len + 1] = '\0';
            }
            if (IsKeyPressed(KEY_BACKSPACE) && strlen(inputBuffer) > 0) {
                inputBuffer[strlen(inputBuffer) - 1] = '\0';
            }

            // Button Actions
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePos, searchButton)) {
                    if (strlen(inputBuffer) > 0) {
                        // Perform Search
                        matchingUsers.clear();
                        for (const auto& userPair : users) {
                            if (userPair.second.name.rfind(inputBuffer, 0) == 0) {
                                matchingUsers.push_back(userPair.second.name);
                            }
                        }
                        state = RESULTS;
                    }
                } else if (CheckCollisionPointRec(mousePos, backButton)) {
                    return; // Return to the main menu
                }
            }
        } else if (state == RESULTS) {
            // Results Screen
            DrawText("Search Results", screenWidth / 2 - MeasureText("Search Results", 20) / 2, 20, 20, BLACK);

            if (matchingUsers.empty()) {
                DrawText("No users found!", screenWidth / 2 - MeasureText("No users found!", 20) / 2, 60, 20, RED);
            } else {
                DrawText("Matching Users:", screenWidth / 2 - MeasureText("Matching Users:", 20) / 2, 60, 20, BLACK);

                for (size_t i = 0; i < matchingUsers.size(); ++i) {
                    DrawText(matchingUsers[i].c_str(), screenWidth / 2 - MeasureText(matchingUsers[i].c_str(), 20) / 2, 100 + i * 40, 20, DARKBLUE);

                    Rectangle usernameArea = {screenWidth / 2 - 100.0f, (100 + i * 40.0f), 200.0f, 30.0f}; // Cast to float

                    // Draw outline on hover for user names
                    if (CheckCollisionPointRec(mousePos, usernameArea)) {
                        DrawRectangleLinesEx(usernameArea, 2, DARKBLUE);
                    }

                    if (CheckCollisionPointRec(mousePos, usernameArea) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        selectedUser = matchingUsers[i];
                        state = USER_ACTIONS;
                        break;
                    }
                }
            }

            // Back Button
            Rectangle backButtonRec = {screenWidth / 2 - 50.0f, 550.0f, 100.0f, 40.0f}; // Cast to float
            DrawRectangleRec(backButtonRec, WHITE);
            DrawText("Back", backButtonRec.x + (backButtonRec.width - MeasureText("Back", 20)) / 2, backButtonRec.y + 10, 20, DARKBLUE);

            // Draw button outline when hovered
            if (CheckCollisionPointRec(mousePos, backButtonRec)) {
                DrawRectangleLinesEx(backButtonRec, 2, DARKBLUE); // Outline for back button
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, backButtonRec)) {
                state = INPUT; // Go back to input screen
            }
        } else if (state == USER_ACTIONS) {
            // User Actions Screen
            DrawText("User Actions", screenWidth / 2 - MeasureText("User Actions", 20) / 2, 20, 20, DARKBLUE);
            DrawText(selectedUser.c_str(), screenWidth / 2 - MeasureText(selectedUser.c_str(), 20) / 2, 60, 20, BLACK);

            Rectangle followButton = {screenWidth / 2 - 70.0f, 120.0f, 140.0f, 50.0f}; // Cast to float
            Rectangle profileButton = {screenWidth / 2 - 70.0f, 190.0f, 140.0f, 50.0f}; // Cast to float

            DrawRectangleRec(followButton, WHITE);
            DrawText("Follow", followButton.x + (followButton.width - MeasureText("Follow", 20)) / 2, followButton.y + 15, 20, DARKBLUE);

            DrawRectangleRec(profileButton, WHITE);
            DrawText("See Profile", profileButton.x + (profileButton.width - MeasureText("See Profile", 20)) / 2, profileButton.y + 15, 20, DARKBLUE);

            // Draw button outlines when hovered
            if (CheckCollisionPointRec(mousePos, followButton)) {
                DrawRectangleLinesEx(followButton, 2, DARKBLUE);
            }
            if (CheckCollisionPointRec(mousePos, profileButton)) {
                DrawRectangleLinesEx(profileButton, 2, DARKBLUE);
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePos, followButton)) {
                    followUser(currentUserName, selectedUser);
                } else if (CheckCollisionPointRec(mousePos, profileButton)) {
                    ProfileSearch(selectedUser);
                }
            }

            // Back Button
            Rectangle backButtonRec = {screenWidth / 2 - 50.0f, 550.0f, 100.0f, 40.0f}; // Cast to float
            DrawRectangleRec(backButtonRec, WHITE);
            DrawText("Back", backButtonRec.x + (backButtonRec.width - MeasureText("Back", 20)) / 2, backButtonRec.y + 10, 20, DARKBLUE);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, backButtonRec)) {
                state = RESULTS; // Go back to results screen
            }
        }

        EndDrawing();
    }
}

void SocialNetwork::followUser(const string &currentUserName) {
    const int screenWidth = 800; // Increased width
    const int screenHeight = 500; // Increased height
    InitWindow(screenWidth, screenHeight, "Follow User");

    SetTargetFPS(60);

    // Input box properties
    Rectangle inputBox = {screenWidth / 2 - 150, 140, 300, 40};
    Rectangle followButton = {screenWidth / 2 - 75, 250, 150, 40};
    Rectangle backButton = {screenWidth / 2 - 75, 330, 150, 40};

    // Define custom colors
    Color LIGHT_BLUE = {173, 216, 230, 255};
    Color DARK_BLUE = {0, 0, 139, 255};
    Color BUTTON_WHITE = {255, 255, 255, 255};
    Color DARK_GREEN = {0, 128, 0, 255};

    string inputUsername;
    string feedback;
    bool inputActive = false;

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();

        // Handle input box activation and text input
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            inputActive = CheckCollisionPointRec(mousePos, inputBox);
        }

        if (inputActive) {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 126 && inputUsername.length() < 20) {  // Limit to 20 characters
                    inputUsername += static_cast<char>(key);
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !inputUsername.empty()) {
                inputUsername.pop_back();
            }
        }

        // Handle button clicks
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mousePos, followButton)) {
                // Perform follow action
                int currentUserId = -1, userToFollowId = -1;
                for (const auto &userPair : users) {
                    if (userPair.second.name == currentUserName) currentUserId = userPair.first;
                    if (userPair.second.name == inputUsername) userToFollowId = userPair.first;
                }

                if (userToFollowId == -1) {
                    feedback = "Error: User not found.";
                } else if (currentUserId == userToFollowId) {
                    feedback = "Error: Cannot follow yourself.";
                } else {
                    User &currentUser = users[currentUserId];
                    if (find(currentUser.friends.begin(), currentUser.friends.end(), userToFollowId) == currentUser.friends.end()) {
                        currentUser.friends.push_back(userToFollowId);
                        feedback = "Successfully followed " + inputUsername + "!";
                        updateUsersFile("users.txt");
                    } else {
                        feedback = "You are already following " + inputUsername + ".";
                    }
                }
            } else if (CheckCollisionPointRec(mousePos, backButton)) {
                CloseWindow();
                userMenu(currentUserName);
                return; // Exit to main menu
            }
        }

        // Draw interface
        BeginDrawing();
        ClearBackground(LIGHT_BLUE);

        // Title
        int titleFontSize = 30; // Larger font size for the title
        DrawText("Follow User", screenWidth / 2 - MeasureText("Follow User", titleFontSize) / 2, 60, titleFontSize, DARK_BLUE);

        // Input box
        DrawRectangleRec(inputBox, BUTTON_WHITE);
        DrawRectangleLinesEx(inputBox, 2, inputActive ? DARK_BLUE : DARK_BLUE);
        DrawText((inputUsername + ((inputActive && (int(GetTime() * 2) % 2 == 0)) ? "|" : "")).c_str(),
                 inputBox.x + 5, inputBox.y + 10, 20, DARK_BLUE);

        // Follow button
        DrawRectangleRec(followButton, BUTTON_WHITE);
        DrawRectangleLinesEx(followButton, 2, DARK_BLUE);
        DrawText("Follow", followButton.x + 40, followButton.y + 10, 20, DARK_BLUE);

        // Back button
        DrawRectangleRec(backButton, BUTTON_WHITE);
        DrawRectangleLinesEx(backButton, 2, DARK_BLUE);
        DrawText("Back", backButton.x + 50, backButton.y + 10, 20, DARK_BLUE);

        // Feedback message
        if (!feedback.empty()) {
            Color feedbackColor = (feedback.find("Successfully") != string::npos) ? DARK_GREEN : DARK_BLUE;
            DrawText(feedback.c_str(), screenWidth / 2 - MeasureText(feedback.c_str(), 20) / 2, 400, 20, feedbackColor);
        }

        EndDrawing();
    }

    CloseWindow();
}

void SocialNetwork::followUser(const string &currentUserName, const string &usernameToFollow) {
    const int screenWidth = 600;
    const int screenHeight = 400;
    InitWindow(screenWidth, screenHeight, "Follow User");

    SetTargetFPS(60);

    // Input box properties
    Rectangle inputBox = {screenWidth / 2 - 150, 120, 300, 40};
    Rectangle followButton = {screenWidth / 2 - 75, 200, 150, 40};
    Rectangle backButton = {screenWidth / 2 - 75, 280, 150, 40};

    // Define custom colors
    Color LIGHT_BLUE = {173, 216, 230, 255};    // RGB(173, 216, 230) for background
    Color DARK_BLUE = {0, 0, 139, 255};         // RGB(0, 0, 139) for text and outlines
    Color BUTTON_WHITE = {255, 255, 255, 255};  // RGB(255, 255, 255) for button background
    Color DARK_GREEN = {0, 128, 0, 255};        // RGB(0, 128, 0) for success messages

    string inputUsername = usernameToFollow; // Optional parameter to pre-fill username
    string feedback;
    bool inputActive = false;

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();

        // Handle input box activation and text input
        if (usernameToFollow.empty()) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                inputActive = CheckCollisionPointRec(mousePos, inputBox);
            }

            if (inputActive) {
                int key = GetCharPressed();
                while (key > 0) {
                    if (key >= 32 && key <= 126 && inputUsername.length() < 20) {
                        inputUsername += static_cast<char>(key);
                    }
                    key = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE) && !inputUsername.empty()) {
                    inputUsername.pop_back();
                }
            }
        }

        // Handle button clicks
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mousePos, followButton)) {
                // Perform follow action
                int currentUserId = -1, userToFollowId = -1;
                for (const auto &userPair : users) {
                    if (userPair.second.name == currentUserName) currentUserId = userPair.first;
                    if (userPair.second.name == inputUsername) userToFollowId = userPair.first;
                }

                if (userToFollowId == -1) {
                    feedback = "Error: User not found.";
                } else if (currentUserId == userToFollowId) {
                    feedback = "Error: Cannot follow yourself.";
                } else {
                    User &currentUser = users[currentUserId];
                    if (find(currentUser.friends.begin(), currentUser.friends.end(), userToFollowId) == currentUser.friends.end()) {
                        currentUser.friends.push_back(userToFollowId);
                        feedback = "Successfully followed " + inputUsername + "!";
                        updateUsersFile("users.txt");
                    } else {
                        feedback = "You are already following " + inputUsername + ".";
                    }
                }
            } else if (CheckCollisionPointRec(mousePos, backButton)) {
                CloseWindow();
                userMenu(currentUserName);
                return; // Exit to main menu
            }
        }

        // Draw interface
        BeginDrawing();
        ClearBackground(LIGHT_BLUE);

        // Title
        DrawText("Follow User", screenWidth / 2 - MeasureText("Follow User", 20) / 2, 40, 20, DARK_BLUE);

        // Input box
        if (usernameToFollow.empty()) {
            DrawRectangleRec(inputBox, BUTTON_WHITE);
            DrawRectangleLinesEx(inputBox, 2, inputActive ? DARK_BLUE : DARK_BLUE);
            DrawText((inputUsername + ((inputActive && (int(GetTime() * 2) % 2 == 0)) ? "|" : "")).c_str(),
                     inputBox.x + 5, inputBox.y + 10, 20, DARK_BLUE);
        } else {
            DrawText(("Following user: " + usernameToFollow).c_str(), screenWidth / 2 - MeasureText(("Following user: " + usernameToFollow).c_str(), 20) / 2, 120, 20, DARK_BLUE);
        }

        // Follow button
        DrawRectangleRec(followButton, BUTTON_WHITE);
        DrawRectangleLinesEx(followButton, 2, DARK_BLUE);
        DrawText("Follow", followButton.x + 40, followButton.y + 10, 20, DARK_BLUE);

        // Back button
        DrawRectangleRec(backButton, BUTTON_WHITE);
        DrawRectangleLinesEx(backButton, 2, DARK_BLUE);
        DrawText("Back", backButton.x + 50, backButton.y + 10, 20, DARK_BLUE);

        // Feedback message
        if (!feedback.empty()) {
            Color feedbackColor = (feedback.find("Successfully") != string::npos) ? DARK_GREEN : DARK_BLUE;
            DrawText(feedback.c_str(), screenWidth / 2 - MeasureText(feedback.c_str(), 20) / 2, 340, 20, feedbackColor);
        }

        EndDrawing();
    }

    CloseWindow();
}

void SocialNetwork::unfollowUser(const string &currentUserName) {
    // Define custom colors
    Color LIGHT_BLUE = { 173, 216, 230, 255 };   // RGB(173, 216, 230) for background
    Color DARK_BLUE = { 0, 0, 139, 255 };        // RGB(0, 0, 139) for text and outlines
    Color BUTTON_WHITE = { 255, 255, 255, 255 }; // RGB(255, 255, 255) for button background

    const int screenWidth = 600;  // Window width
    const int screenHeight = 400; // Window height
    InitWindow(screenWidth, screenHeight, "Unfollow User");
    SetTargetFPS(60);

    char inputText[21] = ""; // Input buffer (20 chars max + 1 for null terminator)
    int letterCount = 0;
    bool unfollowButtonPressed = false;
    string feedback;

    Rectangle unfollowButton = {220, 180, 150, 40}; // Unfollow button dimensions
    Rectangle backButton = {220, 240, 150, 40};     // Back button dimensions

    while (!WindowShouldClose()) {
        // Handle user text input
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (letterCount < 20)) { // Limit to 20 letters
                inputText[letterCount] = (char)key;
                letterCount++;
                inputText[letterCount] = '\0';
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
            letterCount--;
            inputText[letterCount] = '\0';
        }

        // Button click detection
        if (CheckCollisionPointRec(GetMousePosition(), unfollowButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            unfollowButtonPressed = true;
        }
        if (CheckCollisionPointRec(GetMousePosition(), backButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
             // Go back to the user menu
            CloseWindow();  // Close the current window
            userMenu(currentUserName); // Call userMenu function to go back
            return; 
        }

        // Process unfollow logic when the Unfollow button is pressed
        if (unfollowButtonPressed) {
            string usernameToUnfollow = inputText;
            int userToUnfollowId = -1;

            for (const auto &userPair : users) {
                if (userPair.second.name == usernameToUnfollow) {
                    userToUnfollowId = userPair.first;
                    break;
                }
            }

            if (userToUnfollowId == -1) {
                feedback = "Error: User not found.";
            } else if (currentUserName == usernameToUnfollow) {
                feedback = "Error: You cannot unfollow yourself.";
            } else {
                for (auto &userPair : users) {
                    if (userPair.second.name == currentUserName) {
                        User &currentUser = userPair.second;
                        auto it = find(currentUser.friends.begin(), currentUser.friends.end(), userToUnfollowId);
                        if (it != currentUser.friends.end()) {
                            currentUser.friends.erase(it);
                            feedback = "You have unfollowed " + usernameToUnfollow + ".";
                            updateUsersFile("users.txt");
                        } else {
                            feedback = "You are not following " + usernameToUnfollow + ".";
                        }
                        break;
                    }
                }
            }
            unfollowButtonPressed = false; // Reset the button flag
        }

        // Rendering the window
        BeginDrawing();
        ClearBackground(LIGHT_BLUE); // Custom LIGHT_BLUE for background

        // Title
        DrawText("Unfollow User", 200, 20, 30, DARK_BLUE);

        // Prompt
        DrawText("Enter the username to unfollow:", 20, 80, 20, DARK_BLUE);

        // Input box
        DrawRectangle(20, 120, 560, 40, BUTTON_WHITE);
        DrawRectangleLines(20, 120, 560, 40, DARK_BLUE);
        DrawText(inputText, 30, 130, 20, DARK_BLUE);

        // Unfollow button
        DrawRectangleRec(unfollowButton, BUTTON_WHITE);
        DrawRectangleLinesEx(unfollowButton, 2, DARK_BLUE);
        DrawText("Unfollow", unfollowButton.x + 35, unfollowButton.y + 10, 20, DARK_BLUE);

        // Back button
        DrawRectangleRec(backButton, BUTTON_WHITE);
        DrawRectangleLinesEx(backButton, 2, DARK_BLUE);
        DrawText("Back", backButton.x + 50, backButton.y + 10, 20, DARK_BLUE);

        
        if (!feedback.empty()) {
            // Set text color to GREEN if it's a success message
            Color feedbackColor = (feedback.find("You have unfollowed") == 0) ? GREEN : DARK_BLUE;
            DrawText(feedback.c_str(), 20, 320, 20, feedbackColor);
        }

        EndDrawing();
    }

    CloseWindow();
    userMenu(currentUserName); // Call userMenu function to go back
}