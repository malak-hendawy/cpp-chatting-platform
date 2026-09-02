#include "User.h"
#include <cstring>
#define _CRT_SECURE_NO_WARNINGS

// Queue implementation
void initInbox(InboxQueue* q) {
    q->front = 0;
    q->rear = 0;
}

int isInboxEmpty(InboxQueue* q) {
    return q->front == q->rear;
}

int isInboxFull(InboxQueue* q) {
    return (q->rear + 1) % MAX_MESSAGES == q->front;
}

void enqueueInbox(InboxQueue* q, const char* msg) {
    if (isInboxFull(q)) {
        // Option: overwrite oldest or reject. 
        // For simplicity, let's just return or maybe overwrite if we wanted circular buffer behavior.
        // Usually chat systems might drop old messages or reject.
        // Let's protect against overflow by doing nothing or simple return.
        return; 
    }
    strncpy(q->messages[q->rear], msg, MAX_MESSAGE_LEN - 1);
    q->messages[q->rear][MAX_MESSAGE_LEN - 1] = '\0';
    q->rear = (q->rear + 1) % MAX_MESSAGES;
}

void dequeueInbox(InboxQueue* q, char* buffer) {
    if (isInboxEmpty(q)) {
        buffer[0] = '\0';
        return;
    }
    strncpy(buffer, q->messages[q->front], MAX_MESSAGE_LEN); // Copy message
    q->front = (q->front + 1) % MAX_MESSAGES;
}
