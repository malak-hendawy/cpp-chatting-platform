#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <cstdio>

// Define the maximum size of the hashmap
#define HASHMAP_SIZE 101 // Use a prime number for better distribution

// Define a structure for hashmap nodes
struct HashNode {
    char key[MAX_NAME]; // Username
    int value;          // Index in the users array
    HashNode* next;     // For chaining in case of collisions
};

// Define the HashMap class
class HashMap {
private:
    HashNode* buckets[HASHMAP_SIZE];

    // Hash function
    int hashFunction(const char* key) {
        int hash = 0;
        while (*key) {
            hash = (hash * 31 + *key) % HASHMAP_SIZE; // Simple hash function
            key++;
        }
        return hash;
    }

public:
    // Constructor
    HashMap() {
        memset(buckets, 0, sizeof(buckets));
    }

    // Insert a key-value pair
    void insert(const char* key, int value) {
        int index = hashFunction(key);
        HashNode* node = buckets[index];
        
        // Check if key exists and update
        while (node) {
            if (strcmp(node->key, key) == 0) {
                node->value = value;
                return;
            }
            node = node->next;
        }

        // Insert new node at head
        HashNode* newNode = new HashNode();
        strncpy(newNode->key, key, sizeof(newNode->key) - 1);
        newNode->key[sizeof(newNode->key) - 1] = '\0';
        newNode->value = value;
        newNode->next = buckets[index];
        buckets[index] = newNode;
    }

    // Search for a value by key
    int search(const char* key) {
        int index = hashFunction(key);
        HashNode* node = buckets[index];
        while (node) {
            if (strcmp(node->key, key) == 0) {
                return node->value;
            }
            node = node->next;
        }
        return -1; // Key not found
    }

    // Remove a key-value pair
    void remove(const char* key) {
        int index = hashFunction(key);
        HashNode* node = buckets[index];
        HashNode* prev = nullptr;
        while (node) {
            if (strcmp(node->key, key) == 0) {
                if (prev) {
                    prev->next = node->next;
                } else {
                    buckets[index] = node->next;
                }
                delete node;
                return;
            }
            prev = node;
            node = node->next;
        }
    }

    // Destructor
    ~HashMap() {
        for (int i = 0; i < HASHMAP_SIZE; ++i) {
            HashNode* node = buckets[i];
            while (node) {
                HashNode* temp = node;
                node = node->next;
                delete temp;
            }
        }
    }
};