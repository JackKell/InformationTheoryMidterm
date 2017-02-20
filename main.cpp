// includes
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include <regex>

// using statements
using std::unordered_map;
using std::map;
using std::ofstream;
using std::ios;
using std::string;
using std::ifstream;
using std::cout;
using std::getline;
using std::tolower;
using std::stoi;
using std::to_string;
using std::log2;
using std::istreambuf_iterator;
using std::vector;
using std::find;
using std::regex;
using std::sregex_token_iterator;
using std::smatch;
using std::regex_match;
using std::regex_replace;
using std::sort;
using std::pair;

void cleanTextFile(string inputPath, string outputPath) {
    // open the input file
    ifstream inputTextFile(inputPath, ios::in|ios::ate);

    // create a string buffer to hold the new output file
    string outputData;
    outputData.reserve((unsigned int) inputTextFile.tellg());

    // set the file pointer back to the beginning
    inputTextFile.seekg(ios::beg);

    // for each line in the file remove all non alphabet characters and canalization
    string currentLine;
    while (getline(inputTextFile, currentLine)) {
        for(char& x : currentLine) {
            if(isalpha(x)) {
                outputData += tolower(x);
            }
        }
    }
    inputTextFile.close();

    // output the cleaned content to the output file
    ofstream outputFile(outputPath, ios::trunc);
    outputFile << outputData << "\n";
    outputFile.close();
}

map<string, int> getFrequency(string input, const unsigned long nGrams = 1) {
    map<string, int> frequency;
    for (unsigned int i = 0; i < input.length(); i += nGrams) {
        const string nGram = input.substr(i, nGrams);
        frequency[nGram] += 1;
    }
    return frequency;
}

int getTotalOccurrences(map<string, int> frequency) {
    int total = 0;
    for (const auto& element: frequency) {
        total += element.second;
    }
    return total;
}

float getEntropy(map<string, int> frequency) {
    float entropy = 0.0f;

    const int total = getTotalOccurrences(frequency);

    for (const auto& element: frequency) {
        float probabilityOfElement = element.second / (float) total;
        entropy += probabilityOfElement * log2(probabilityOfElement);
    }
    return -1 * entropy;
}

string lempelZivEncoding(const string input) {
    // the string representing the final compressed encoding
    string encoding = "";
    // the list of unique strings that have appeared in the list
    vector<string> encodingList(0);
    // iterate through each character in the input string
    for (int i = 0; i < input.length();) {
        // iterate through each character after the current character
        for (int j = 1; j + i <= input.length(); j++) {
            // the current string to test if it has appeared previously
            string currentString = input.substr(i, j);
            // check if the current string has appeared in the input before
            auto iter = find(encodingList.begin(), encodingList.end(), currentString);
            // if the current string has not appeared
            if(iter == encodingList.end()) {
                // the string's length is 1 and it does not appear in the encoding we can simply add it
                if (currentString.length() == 1) {
                    // add the current string to the encoding list
                    encodingList.push_back(currentString);
                    // add the current string to the encoding
                    encoding += currentString;
                } else {
                    // the searchString is the last string that may have occurred in the list
                    string searchString = currentString.substr(0, currentString.size() - 1);
                    // find where search string appeared in the encoding list
                    const long pointerIndex = find(encodingList.begin(), encodingList.end(), searchString) - encodingList.begin();
                    // the string that will be added to the encoding is the index of the searchString and the last character of the current string
                    const string insertString = to_string(pointerIndex) + currentString[currentString.size() - 1];
                    // add the current string to the encoding list
                    encodingList.push_back(currentString);
                    // add the insert string to the encoding
                    encoding += insertString;
                }
                // increment the position of i by the size of the string added
                i += j;
                // end this iteration of the j loop
                break;
            } else {
                // if the current string does has appeared before but it is the last string in the input
                if (j + i == input.length()) {
                    // then simply add the index of the current string to the encoding
                    encoding += to_string(iter - encodingList.begin());
                    i += j;
                }
            }
        }
    }
    return encoding;
}

vector<string> split(string input, const regex regexPattern) {
    vector<string> matches;
    smatch sm;
    while(regex_search(input, sm, regexPattern)) {
        if(sm.str() == "")
            break;
        matches.push_back(sm.str());
        input = sm.suffix();
    }
    return matches;
}

string lempelZivDecoding(const string input) {
    const regex lempelZivRegexPattern = regex("(\\d*\\D?)");
    const regex numberPattern = regex("\\d+.*");
    const regex numberMather = regex("\\d+");
    const vector<string> encodingList = split(input, lempelZivRegexPattern);
    string decoding = "";

    for (unsigned int i = 0; i < encodingList.size(); i++) {
        smatch match;
        string partialDecoding = encodingList.at(i);
        while (regex_match(partialDecoding, match, numberPattern)) {
            const string indexAsString = match.str();
            const unsigned long index = (const unsigned long) stoi(indexAsString);
            partialDecoding = regex_replace(partialDecoding, numberMather, encodingList.at(index));
        }
        decoding += partialDecoding;
    }
    return decoding;
}


void huffmanEncoding(string input) {

}

void huffmanDecoding() {

}

//tree implementation referenced form http://math.hws.edu/javanotes/c9/s4.html and https://gist.github.com/mgechev/5911348
struct Node {
    int frequency;
    string value;
    Node* left;
    Node* right;

    Node(int frequency, string value) {
        Node(frequency, value, nullptr, nullptr);
    }

    Node(int frequency, string value, Node* left, Node* right) {
        this->frequency = frequency;
        this->value = value;
        this->left = left;
        this->right = right;
    };
};

class HuffmanTree {
private:
    Node *root;
    int codebook;

    void addLeaf() {
        if (root) {

        } else {

        }
    }

public:
    HuffmanTree(map<string, int> frequency);
};

// function reference http://stackoverflow.com/questions/279854/how-do-i-sort-a-vector-of-pairs-based-on-the-second-element-of-the-pair
HuffmanTree::HuffmanTree(map<string, int> frequency) {
    vector<pair<string, int>> sortedFrequency;
    for (auto element: frequency) {
        cout << element.first << " " << element.second << "\n";
        sortedFrequency.push_back(pair<string, int>(element.first, element.second));
    }
    sort(sortedFrequency.begin(), sortedFrequency.end(), [](
            const pair<string, int> &left,
            const pair<string, int> &right) {
        return left.second > right.second;
    });
    for (unsigned int i = 0; i < sortedFrequency.size(); i++) {
        pair<string, int> currentFrequency = (pair<string, int> &&) sortedFrequency.at(i);
        if (root) {
            if (root->left) {

            } else if (root->right) {

            } else {

            }
        } else {

        }
    }
}

int main() {
//    cout << "Starting Information Theory Midterm\n";
//    string inputPath = "text/tom.txt";
//    string outputPath = "text/tom-clean.txt";
//
//    cout << "Cleaning Text File " << inputPath << "\n";
//    cleanTextFile(inputPath, outputPath);
//    cout << "Cleaned Text File " << inputPath << " to " << outputPath << "\n";
//
//    string cleanText;
//    ifstream cleanTextFile(outputPath);
//    // for each line in the file remove all non alphabet characters and canalization
//    string buffer;
//    while (getline(cleanTextFile, buffer)) {
//        cleanText += buffer;
//    }
//
//    cout << cleanText << "\n";
//    map<string, int> frequency = getFrequency(cleanText, 2);
//    for (auto element: frequency) {
//        cout << element.first << " " << element.second << "\n";
//    }
//    cout << getEntropy(frequency) << "\n";

//    // Lempel Ziv Test
//    cout << "Input:\n";
//    string test = "aaabbbaaaaaaaaabababababababba";
//    cout << test << "\n\n";
//
//    cout << "Compressed:\n";
//    string compressed = lempelZivEncoding(test);
//    cout << compressed << "\n\n";
//
//    cout << "Decompressed:\n";
//    string decompressed = lempelZivDecoding(compressed);
//    cout << decompressed << "\n\n";

    map<string, int> frequency = getFrequency("aasdfaascdcs");
    vector<pair<string, int>> x;
    for (auto element: frequency) {
        cout << element.first << " " << element.second << "\n";
        x.push_back(pair<string, int>(element.first, element.second));
    }
    sort(x.begin(), x.end(), [](const pair<string, int> &left, const pair<string, int> &right) {
        return left.second > right.second;
    });
    cout << "\n";
    for (auto element: x) {
        cout << element.first << " " << element.second << "\n";
    }
    cout << getEntropy(frequency) << "\n";
}