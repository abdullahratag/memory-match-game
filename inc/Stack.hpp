#pragma once
#include <stdexcept>

struct CardPosition {
    int row;
    int col;
};

struct Node {
    CardPosition data;
    Node* next = nullptr;
};

class Stack {
private:
    Node* top;
public:
    Stack();
    ~Stack();
    bool isEmpty();
    void push(int r, int c);
    CardPosition pop();
    CardPosition peek();
    CardPosition peekSecond();
    int size();
};
