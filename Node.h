#ifndef INFORMATIONTHEORYMIDTERM_NODE_H
#define INFORMATIONTHEORYMIDTERM_NODE_H

#include <string>

using std::string;

//tree implementation referenced form http://math.hws.edu/javanotes/c9/s4.html and https://gist.github.com/mgechev/5911348
class Node {
public:
    int frequency;
    string value;
    Node* left;
    Node* right;

    Node(int frequency, string value = "", Node* left = nullptr, Node* right = nullptr);

    ~Node();
};


#endif
