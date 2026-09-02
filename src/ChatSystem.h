#pragma once

#ifndef CHATSYSTEM_H
#define CHATSYSTEM_H
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include "User.h" // Include the User structure
#include "HashMap.h" // Include the HashMap structure

// Define constants for maximum limits
#define MAX_NAME 64

#ifndef MAX_USERS
#define MAX_USERS 100
#endif

class ChatSystem {
private:
    HashMap userMap; // Replace std::unordered_map with HashMap
    User users[MAX_USERS];
    int userCount;
    std::string currentUser;
    std::string adminPassword;

    void clearScreen();
    void pauseForUser();

public:
    ChatSystem();
    ~ChatSystem();

    void saveUsersToFile();
    void loadUsersFromFile();
    // Core Logic Methods (GUI Ready)
    bool registerUser(std::string name, std::string phone, std::string pass);
    bool authenticateUser(std::string name, std::string pass);
    bool adminAuth(std::string name, std::string pass);
    int findUser(std::string name); // Helper public or keep private? Making public for GUI/Logic separation convenience
    bool sendMsg(std::string sender, std::string recipient, std::string msg);
    bool resetPassword(std::string name, std::string phone, std::string newPass);
    void resetAdminPassword(std::string newPass);
    bool deleteUser(std::string name);
    std::string getInboxMessages(std::string name);
    void logout();
    std::string getAllUsersString();
    // Getters
    std::string getCurrentUser() { return currentUser; }
    int getUserCount() { return userCount; }
    User* getUserByIndex(int index) { return &users[index]; }

    // Console Wrappers (Keep for backward compatibility)
    void signupUser();
    void loginUser();
    void sendMessage();
    void viewInbox();
    void listUsers();
    void logoutUser();
    void displayMenu();
    void postLoginMenu();
    void adminLogin();
    void forgetPassword();
    void deleteAccount();
};

#endif // CHATSYSTEM_H
