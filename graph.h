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

#include "include/raylib.h"

using namespace std;

#ifndef _GRAPH_
#define _GRAPH_

class User {
public:
    int userID;
    string name;
    string password;
    string interest;
    int posts;
    int comments;
    int likes;
    vector<int> friends;

    // Default constructor
    User();

    // Parameterized constructor
    User(int id, string username, string psw, string userInterest, int p, int c, int l, vector<int> userFriends);
};

class SocialNetwork {
private:
    map<int, User> users;
    int count;
    int loggedIn;

    // Function to add a user to the network
    void addUser(int id, string name, string password, string interest, int posts, int comments, int likes, vector<int> friends);

    // Function to give the total number of users
    int totalUsers();

    void DrawCenteredText(const char *text, int y, int fontSize, Color color);

    // Function to update user friends in the file
    void updateUsersFile(const string &filename);

    // Function to save the data of the user in the file as a new user
    void saveUserFile(const string &fileName, const User &user);

    // Function to show the list of connection of a particular user
    void showConnections(int ID, vector<string>& friendsList);

    // Functions to show the connections of a particular user
    void friendsUser(string name);

    // Function to show the options to the user
    void userMenu(string name);

    // Function to search usernames
   void searchUsers(const string currentUserName);

    void followUser(const string &currentUserName);

    // Function to follow a particular user in network
    void followUser(const string &currentUserName, const string &usernameToFollow);

    // Function to unfollow a particular user in network
    void unfollowUser(const string &currentUserName);
     
    // Function to change password in update profile
    void changePassword(int userID);

    // Function to change interest in update profile
    void changeInterest(int userID);

    // Function to show the profile of signedin or loggedin user
    void Profile(const string& targetUserName);

    // Function to show the profile of searched user
    void ProfileSearch(const string& targetUserName);

    vector<string> findShortestPathInNetwork(const string &startInput, const string &endInput);

    void findShortestPath(const string &startInput);

        // Function to find most influential person based on degree centrality
   vector<string> mostInfluential();
    
    // Function for recommending using interest
   vector<string> recommendFriendsByInterest(const string &username);

    // Function recommending friends mutually
    void recommendFriends(const string &username);

    // Function to give most trending and frequent poster
    vector<string> analyzeUserBehavior();

public:
     // Default constructor
    SocialNetwork();

    void mainPage(SocialNetwork& network);

    // Function to read users from a file
    void loadFromFile(const string &filename);

    // Function to login of existing user
    void userLogin();

    // Function to sign up as new user
    void userSignUp();
};

#endif