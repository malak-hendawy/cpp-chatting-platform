#pragma once

#ifndef USER_H
#define USER_H
#define _CRT_SECURE_NO_WARNINGS
#include <string>

// Define constants for maximum limits
#define MAX_NAME 64
#define MAX_PHONE 11
#define MAX_PASSWORD 12
#define MAX_MESSAGES 100
#define MAX_MESSAGE_LEN 256

// Define the InboxQueue structure
struct InboxQueue {
    char messages[MAX_MESSAGES][MAX_MESSAGE_LEN];
    int front;
    int rear;
};

// Define the User structure
struct User {
    std::string name;
    std::string phone;
    std::string password;
    InboxQueue inbox;
};

// Function prototypes for InboxQueue operations
void initInbox(InboxQueue* q);
int isInboxEmpty(InboxQueue* q);
int isInboxFull(InboxQueue* q);
void enqueueInbox(InboxQueue* q, const char* msg);
void dequeueInbox(InboxQueue* q, char* buffer);

#endif // USER_H
