
#include "ChatSystem.h"
#include "HashMap.h"
#include "User.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
#define _CRT_SECURE_NO_WARNINGS
// Define constants

#define MAX_USERS 100
#define MAX_MESSAGES 100
#define MAX_MESSAGE_LEN 256

// Helper to get a line of text safely
void getLine(string& buffer) {
    getline(cin, buffer);
}

// Helper to get an integer safely
int getInt() {
    string buffer;
    getLine(buffer);
    return stoi(buffer);
}

// Use ChatSystem member variables instead of globals

ChatSystem::ChatSystem() {
    userCount = 0;
    currentUser = "";
    adminPassword = "1234";
}

ChatSystem::~ChatSystem() {
    // Cleanup if needed
}

void ChatSystem::clearScreen() {
    system("cls");
}

void ChatSystem::pauseForUser() {
    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
}

void ChatSystem::saveUsersToFile() {
    ofstream file("users.txt");
    if (!file) {
        cerr << "Error opening file for writing." << endl;
        return;
    }
    file << userCount << '\n';
    for (int i = 0; i < userCount; ++i) {
        file << users[i].name << ',' << users[i].phone << ',' << users[i].password << '\n';
    }
}

void ChatSystem::loadUsersFromFile() {
    ifstream file("users.txt");
    if (!file) return; // No file yet

    int count = 0;
    file >> count;
    file.ignore(); // Skip the newline after the count

    userCount = 0;
    string line;
    while (getline(file, line) && userCount < MAX_USERS) {
        istringstream iss(line);
        string name, phone, pass;
        if (getline(iss, name, ',') &&
            getline(iss, phone, ',') &&
            getline(iss, pass)) {
            users[userCount].name = name;
            users[userCount].phone = phone;
            users[userCount].password = pass;
            initInbox(&users[userCount].inbox);
            userMap.insert(users[userCount].name.c_str(), userCount);
            userCount++;
        }
    }
}

bool ChatSystem::deleteUser(string name) {
    int idx = userMap.search(name.c_str());
    if (idx == -1) return false;

    userMap.remove(name.c_str());

    if (idx != userCount - 1) {
        users[idx] = users[userCount - 1];
        userMap.insert(users[idx].name.c_str(), idx);
    }
    userCount--;
    saveUsersToFile();
    
    if (currentUser == name) {
        currentUser = "";
    }
    return true;
}

void ChatSystem::deleteAccount() {
    cout << "\n--- DELETE ACCOUNT ---" << endl;
    if (currentUser.empty()) {
        cout << "You must be logged in to delete an account." << endl;
        pauseForUser();
        return;
    }

    string confirmPass;
    cout << "Are you sure you want to delete your account (" << currentUser << ")? This action cannot be undone. (yes/no): ";
    string confirmation;
    getLine(confirmation);

    if (confirmation != "yes") {
        cout << "Account deletion cancelled." << endl;
        pauseForUser();
        return;
    }

    cout << "Please enter your password to confirm: ";
    getLine(confirmPass);

    int idx = userMap.search(currentUser.c_str());
    if (idx == -1 || users[idx].password != confirmPass) {
        cout << "Incorrect password. Account deletion failed." << endl;
        pauseForUser();
        return;
    }

    if (deleteUser(currentUser)) {
        cout << "Account '" << currentUser << "' deleted successfully." << endl;
        currentUser = ""; // Ensure current user is cleared after deletion
    } else {
        cout << "Error deleting account." << endl;
    }
    pauseForUser();
}

bool ChatSystem::registerUser(string name, string phone, string pass) {
    if (userCount >= MAX_USERS) return false;
    if (userMap.search(name.c_str()) != -1) return false;

    users[userCount].name = name;
    users[userCount].phone = phone;
    users[userCount].password = pass;
    initInbox(&users[userCount].inbox);

    userMap.insert(name.c_str(), userCount);
    userCount++;
    saveUsersToFile();
    return true;
}

void ChatSystem::signupUser() {
    string name, phone, pass;

    cout << "\n--- SIGNUP ---" << endl;
    cout << "Enter username: ";
    getLine(name);

    if (userMap.search(name.c_str()) != -1) {
        cout << "Username already exists." << endl;
        pauseForUser();
        return;
    }

    cout << "Enter phone: ";
    getLine(phone);
    cout << "Enter password: ";
    getLine(pass);

    if (registerUser(name, phone, pass)) {
        cout << "Signup successful!" << endl;
    } else {
        cout << "Signup failed (Limit reached or User exists)." << endl;
    }
    pauseForUser();
}

bool ChatSystem::authenticateUser(string name, string pass) {
    int idx = userMap.search(name.c_str());
    if (idx == -1) return false;
    if (users[idx].password == pass) {
        currentUser = name;
        return true;
    }
    return false;
}

void ChatSystem::loginUser() {
    string name, pass;

    cout << "\n--- LOGIN ---" << endl;
    cout << "Enter username: ";
    getLine(name);

    int idx = userMap.search(name.c_str());
    if (idx == -1) {
        cout << "User not found." << endl;
        pauseForUser();
        return;
    }

    cout << "Enter password: ";
    getLine(pass);

    if (authenticateUser(name, pass)) {
        cout << "Login successful! Welcome " << currentUser << "." << endl;
        pauseForUser();
    } else {
        cout << "Incorrect password." << endl;
        pauseForUser();
    }
}

void ChatSystem::displayMenu() {
    int choice;
    do {
        clearScreen();
        cout << "===========================" << endl;
        cout << "    CHAT SYSTEM MAIN MENU  " << endl;
        cout << "===========================" << endl;
        cout << "1. Signup" << endl;
        cout << "2. Login" << endl;
        cout << "3. Admin Login" << endl;
        cout << "4. Forget Password" << endl;
        cout << "5. Exit" << endl;
        cout << "===========================" << endl;
        cout << "Enter your choice: ";

        choice = getInt();

        switch (choice) {
            case 1: signupUser(); break;
            case 2:
                loginUser();
                if (!currentUser.empty()) {
                    postLoginMenu();
                }
                break;
            case 3: adminLogin(); break;
            case 4: forgetPassword(); break;
            case 5: cout << "Exiting..." << endl; break;
            default: cout << "Invalid choice! Please try again." << endl; pauseForUser(); break;
        }
    } while (choice != 5);
}

void ChatSystem::postLoginMenu() {
    int choice;
    do {
        clearScreen();
        cout << "===========================" << endl;
        cout << "    USER MENU: " << currentUser << endl;
        cout << "===========================" << endl;
        cout << "1. Send Message" << endl;
        cout << "2. View Inbox" << endl;
        cout << "3. Logout" << endl;
        cout << "4. Delete Account" << endl;
        cout << "===========================" << endl;
        cout << "Enter your choice: ";

        choice = getInt();

        switch (choice) {
            case 1: sendMessage(); break;
            case 2: viewInbox(); break;
            case 3: logoutUser(); return;
            case 4:
                deleteAccount();
                if (currentUser.empty()) return;
                break;
            default: cout << "Invalid choice! Please try again." << endl; pauseForUser(); break;
        }
    } while (true);
}

bool ChatSystem::sendMsg(string sender, string recipient, string msg) {
    int idx = userMap.search(recipient.c_str());
    if (idx == -1) return false;

    string formattedMsg = sender + ": " + msg;
    enqueueInbox(&users[idx].inbox, formattedMsg.c_str());
    saveUsersToFile();
    return true;
}

void ChatSystem::sendMessage() {
    string recipient, message;

    cout << "\n--- SEND MESSAGE ---" << endl;
    cout << "Enter recipient username: ";
    getLine(recipient);

    if (userMap.search(recipient.c_str()) == -1) {
        cout << "Error: User '" << recipient << "' not found." << endl;
        pauseForUser();
        return;
    }

    cout << "Enter message: ";
    getLine(message);

    if (sendMsg(currentUser, recipient, message)) {
        cout << "Message sent to " << recipient << "." << endl;
    } else {
        cout << "Error: User not found." << endl;
    }
    pauseForUser();
}

void ChatSystem::logout() {
    currentUser = "";
}

void ChatSystem::logoutUser() {
    if (currentUser.empty())
        cout << "Not logged in." << endl;
    else {
        cout << "Goodbye, " << currentUser << endl;
        logout();
    }
    pauseForUser();
}

std::string ChatSystem::getAllUsersString() {
    std::string result = "Registered Users:\n";
    for (int i = 0; i < userCount; i++) {
        result += "- " + users[i].name + "\n";
    }
    if (userCount == 0) result += "(None)";
    return result;
}

void ChatSystem::listUsers() {
    cout << "Registered users:" << endl;
    for (int i = 0; i < userCount; i++)
        cout << "- " << users[i].name << endl;
    pauseForUser();
}

void ChatSystem::viewInbox() {
    int idx = userMap.search(currentUser.c_str());
    if (idx == -1) return;

    InboxQueue* q = &users[idx].inbox;
    clearScreen();
    cout << "===========================" << endl;
    cout << "       INBOX FOR " << currentUser << endl;
    cout << "===========================" << endl;

    if (isInboxEmpty(q)) {
        cout << "(No messages)" << endl;
    } else {
        int i = q->front;
        while (i != q->rear) {
            cout << "> " << q->messages[i] << endl;
            i = (i + 1) % MAX_MESSAGES;
        }
    }
    cout << "===========================" << endl;
    pauseForUser();
}

bool ChatSystem::adminAuth(string name, string pass) {
    return (name == "admin" && pass == adminPassword);
}

void ChatSystem::resetAdminPassword(string newPass) {
    adminPassword = newPass;
}

void ChatSystem::adminLogin() {
    string name, password;
    cout << "\n--- ADMIN LOGIN ---" << endl;
    cout << "Enter Admin Username: ";
    getLine(name);
    cout << "Enter Admin Password: ";
    getLine(password);

    if (adminAuth(name, password)) {
        cout << "Admin Access Granted." << endl;
        listUsers();
    } else {
        cout << "Error: Access Denied." << endl;
        pauseForUser();
    }
}

bool ChatSystem::resetPassword(string name, string phone, string newPass) {
    int idx = userMap.search(name.c_str());
    if (idx == -1) return false;

    if (users[idx].phone == phone) {
        users[idx].password = newPass;
        saveUsersToFile();
        return true;
    }
    return false;
}

std::string ChatSystem::getInboxMessages(std::string name) {
    int idx = userMap.search(name.c_str());
    if (idx == -1) return "User not found.";

    InboxQueue* q = &users[idx].inbox;
    if (isInboxEmpty(q)) return "Inbox is empty.";

    std::string result = "";
    int i = q->front;
    while (i != q->rear) {
        result += q->messages[i];
        result += "\r\n"; // Windows line break for Edit controls
        i = (i + 1) % MAX_MESSAGES;
    }
    return result;
}

void ChatSystem::forgetPassword() {
    string name;
    cout << "\n--- FORGET PASSWORD ---" << endl;
    cout << "Enter username: ";
    getLine(name);

    if (name == "admin") {
        string newPass;
        cout << "Enter New Admin Password: ";
        getLine(newPass);
        resetAdminPassword(newPass);
        cout << "Admin password updated successfully!" << endl;
        pauseForUser();
        return;
    }

    int idx = userMap.search(name.c_str());
    if (idx == -1) {
        cout << "User not found." << endl;
        pauseForUser();
        return;
    }

    string phone;
    cout << "Enter Phone Number used for Signup: ";
    getLine(phone);

    string newPass;
    // We can't use getLine(newPass) conditionally logic inside resetPassword easily for CLI specific prompt flow
    // so we prompt first then call logic.
    
    // Check verification first to mimic previous flow exactly, or just assume we prompt.
    // To exact match:
    if (users[idx].phone == phone) {
         cout << "Verification Successful." << endl;
         cout << "Enter New Password: ";
         getLine(newPass);
         resetPassword(name, phone, newPass); 
         cout << "Password updated successfully!" << endl;
    } else {
         cout << "Verification Failed. Phone number does not match." << endl;
    }
    pauseForUser();
}

