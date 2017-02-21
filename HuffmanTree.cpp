#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include "HuffmanTree.h"

using std::string;
using std::setw;
using std::cout;
using std::pair;
using std::vector;
using std::sort;

// function reference http://stackoverflow.com/questions/279854/how-do-i-sort-a-vector-of-pairs-based-on-the-second-element-of-the-pair
HuffmanTree::HuffmanTree(map<string, int> frequency) {
    vector<pair<string, int>> sortedFrequency;
    for (auto element: frequency) {
//        cout << element.first << " " << element.second << "\n";
        sortedFrequency.push_back(pair<string, int>(element.first, element.second));
    }
    sort(sortedFrequency.begin(), sortedFrequency.end(), [](
            const pair<string, int> &left,
            const pair<string, int> &right) {
        return left.second < right.second;
    });

    int max = sortedFrequency.back().second;
    for (unsigned int i = 0; i < sortedFrequency.size(); i++) {
        pair<string, int> currentFrequency = (pair<string, int> &&) sortedFrequency.at(i);
        if (root) {
            if (!(root->right)) {
                root->right = new Node(currentFrequency.second, currentFrequency.first);
                root->frequency += currentFrequency.second;
            } else {
                if (root->frequency <= max) {
                    Node* newLeftNode = new Node(currentFrequency.second, currentFrequency.first);
                    Node* newRootNode = new Node(root->frequency + currentFrequency.second, "", newLeftNode, root);
                    root = newRootNode;
                } else {
                    const unsigned long remainingElements = sortedFrequency.size() - i;
                    if (remainingElements == 1) {
                        Node* newLeftNode = new Node(currentFrequency.second, currentFrequency.first);
                        Node* newRootNode = new Node(root->frequency + currentFrequency.second, "", newLeftNode, root);
                        root = newRootNode;
                    } else if (remainingElements == 2) {
                        Node* newSubTreeLeftNode = new Node(currentFrequency.second, currentFrequency.first);
                        pair<string, int> nextFrequency = (pair<string, int> &&) sortedFrequency.at(i + 1);
                        print(root);
                        cout << "=======" << i << "\n";
                        Node* newSubTreeRightNode = new Node(nextFrequency.second, nextFrequency.first);
                        Node* newSubTreeRootNode = new Node(currentFrequency.second + nextFrequency.second, "", newSubTreeLeftNode, newSubTreeRightNode);
                        Node* newRootNode = new Node(newSubTreeRootNode->frequency + root->frequency, "", newSubTreeRootNode, root);
                        root = newRootNode;
                        i++;
                    } else { //if (remainingElements >= 3){
                        Node* subTreeLeftNode = new Node(currentFrequency.second, currentFrequency.first);
                        pair<string, int> nextFrequency = (pair<string, int> &&) sortedFrequency.at(i + 1);
                        Node* subTreeRightNode = new Node(nextFrequency.second, nextFrequency.first);
                        Node* subTreeRootNode = new Node(currentFrequency.second + nextFrequency.second, "", subTreeLeftNode, subTreeRightNode);
                        pair<string, int> nextNextFrequency = (pair<string, int> &&) sortedFrequency.at(i + 2);
                        Node* newLeftNode = new Node(nextNextFrequency.second, nextNextFrequency.first);
                        if (subTreeRootNode->frequency < root->frequency) {
                            Node* newSubTreeRootNode = new Node(nextFrequency.second + subTreeRootNode->frequency, "", newLeftNode, subTreeRootNode);
                            subTreeRootNode = newSubTreeRootNode;
                        } else {
                            Node* newRootNode = new Node(nextFrequency.second + root->frequency, "", newLeftNode, root);
                            root = newRootNode;
                        }
                        Node* newRootNode = new Node(subTreeRootNode->frequency + root->frequency, "", subTreeRootNode, root);
                        root = newRootNode;
                        i += 2;
                    }
                }
            }
        } else {
            root = new Node(currentFrequency.second, "", new Node(currentFrequency.second, currentFrequency.first));
        }
    }
}

void HuffmanTree::print(Node* node, int indent) {
    if(node != nullptr) {
        if(node->left) {
            print(node->left, indent + 6);
        }
        if(node->right) {
            print(node->right, indent + 6);
        }
        if (indent) {
            cout << setw(indent) << ' ';
        }
        cout<< node->value << " : " << node->frequency << "\n ";
    }
}

void HuffmanTree::print(int indent) {
    print(root, indent);
}

void HuffmanTree::getCodebook(Node* node, string code, map<string, string>& codebook) {
    if(node != nullptr) {
        if(!(node->left && node->right)) {
            codebook[node->value] = code;
        } else {
            if (node->left) {
                getCodebook(node->left, code + "0", codebook);
            }
            if (node->right) {
                getCodebook(node->right, code + "1", codebook);
            }
        }
    }
}

map<string, string> HuffmanTree::getCodebook() {
    map<string, string> codebook;
    getCodebook(root, "", codebook);
    return codebook;
}

map<string, string> HuffmanTree::getDecodebook() {
    map<string, string> codebook;
    map<string, string> decodebook;
    getCodebook(root, "", codebook);
    for (auto element : codebook) {
        decodebook[element.second] = element.first;
    }
    return decodebook;
}

void HuffmanTree::printCodeBook() {
    for (const auto& element : getCodebook()) {
        cout << element.first << " = " << element.second << "\n";
    }
}


