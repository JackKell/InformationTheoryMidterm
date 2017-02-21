#ifndef INFORMATIONTHEORYMIDTERM_HUFFMANTREE_H
#define INFORMATIONTHEORYMIDTERM_HUFFMANTREE_H

#include <map>
#include "Node.h"

using std::map;

class HuffmanTree {
private:
    void print(Node* node, int indent = 0);
public:
    Node *root = nullptr;
    HuffmanTree(map<string, int> frequency);
    ~HuffmanTree() {
        delete root;
    }
    void print(int indent = 0);
    void getCodebook(Node* node, string code, map<string, string>& codebook);
    void printCodeBook();
    map<string, string> getCodebook();
    map<string, string> getDecodebook();
};


#endif
