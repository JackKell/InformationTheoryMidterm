#include "Node.h"

Node::Node(int frequency, string value, Node *left, Node *right) {
    this->frequency = frequency;
    this->value = value;
    this->left = left;
    this->right = right;
}

Node::~Node() {
    delete left;
    delete right;
}