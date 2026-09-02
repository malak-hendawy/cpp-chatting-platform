#define _CRT_SECURE_NO_WARNINGS
#include "ChatSystem.h"
#include <iostream>
#include <stdio.h>                        
using namespace std;


int main() {  
    try {
        cout << "Application Starting..." << endl;
        // Create an instance of ChatSystem on heap to avoid stack overflow (due to large array)
        ChatSystem* chatSystem = new ChatSystem();
    

        // Call member functions on the ChatSystem instance
        chatSystem->loadUsersFromFile(); // Load users from the file at the start
        chatSystem->displayMenu();       // Display the main menu
        chatSystem->saveUsersToFile();   // Save users to the file before exiting
        
        delete chatSystem;
    } catch (const exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
    } catch (...) {
        cerr << "An unknown error occurred." << endl;
    }
    return 0;
}

