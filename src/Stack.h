#pragma once

#include <iostream>
#include <stdexcept>    // Required for the std::runtime_error exception class.

// A simple struct to hold the 2D coordinates of a card on the board.
struct CardPosition { 
    int row; 
    int col; 
};

// Represents a single node in the singly-linked list.
// Each node holds a card's position and a pointer to the next node in the stack.
struct Node { 
    CardPosition data; 
    Node* next = nullptr;  // Pointer to the next node in the list (or nullptr if it iss the last one).
};

// A custom Stack Abstract Data Type (ADT) implemented using a singly-linked list.
// This data structure follows the LIFO (Last-In, First-Out) principle.
class Stack {
private:
    Node* top;    // A pointer to the most recently added node (the top of the stack).

public:
    Stack() : top(nullptr) {}     // Constructor: Initializes an empty stack.

     // Destructor: Ensures all dynamically allocated nodes are freed to prevent memory leaks
    // when the Stack object goes out of scope. It does this by popping all elements.

    ~Stack() { while (!isEmpty()) { pop(); } }

    // Checks if the stack is empty.
    // Returns true if there are no elements, false otherwise.
    bool isEmpty() { return top == nullptr; }

    // Pushes a new element (a card's position) onto the top of the stack.
    void push(int r, int c) { 
        Node* newNode = new Node(); 
        newNode->data = {r, c}; 
        newNode->next = top; 
        top = newNode;
    }

     // Removes and returns the element at the top of the stack.
    // Throws an error if the stack is empty.
    CardPosition pop() {
        if (isEmpty())
            throw std::runtime_error("Stack is empty");
        Node* temp = top;
        CardPosition poppedData = temp->data;
        top = top->next;
        delete temp;
        return poppedData;
    }

     // Returns a copy of the element at the top of the stack without removing it.
    // Throws an error if the stack is empty.
    CardPosition peek() {
        if (isEmpty())
            throw std::runtime_error("Stack is empty");
        return top->data;
    }

    // A helper function to see the second element from the top of the stack.
    // Crucial for comparing the two flipped cards in our game.
    CardPosition peekSecond() {
        if (isEmpty() || top->next == nullptr)
            throw std::runtime_error("Stack lacks a second item");
        return top->next->data;
    }

    // Returns the current number of elements in the stack.
    // Primarily used for debugging and validation.
    int size() {
        int count = 0;
        Node* current = top;
        while (current != nullptr) {    // Traverse the list from top to bottom, counting each node.
            count++;
            current = current->next;
        }
        return count;
    }
};
