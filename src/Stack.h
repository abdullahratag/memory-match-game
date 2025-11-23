#pragma once

#include <iostream>
#include <stdexcept>

struct CardPosition { int row; int col; };

struct Node { CardPosition data; Node* next = nullptr; };

class Stack {
private:
    Node* top;

public:
    Stack() : top(nullptr) {}

    ~Stack() { while (!isEmpty()) { pop(); } }

    bool isEmpty() { return top == nullptr; }

    void push(int r, int c) { Node* newNode = new Node(); newNode->data = {r, c}; newNode->next = top; top = newNode; }

    CardPosition pop() {
        if (isEmpty())
            throw std::runtime_error("Stack is empty");
        Node* temp = top;
        CardPosition poppedData = temp->data;
        top = top->next;
        delete temp;
        return poppedData;
    }

    CardPosition peek() {
        if (isEmpty())
            throw std::runtime_error("Stack is empty");
        return top->data;
    }

    CardPosition peekSecond() {
        if (isEmpty() || top->next == nullptr)
            throw std::runtime_error("Stack lacks a second item");
        return top->next->data;
    }

    int size() {
        int count = 0;
        Node* current = top;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }
};