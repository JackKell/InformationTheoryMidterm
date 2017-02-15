#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <map>
#include <streambuf>
#include <vector>
#include <algorithm>

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

map<string, int> getFrequency(string input, int nGrams = 1) {
    map<string, int> nGramAlphabetMap;
    for (int i = 0; i < input.length(); i += nGrams) {
        const string nGram = input.substr(i, nGrams);
        nGramAlphabetMap[nGram] += 1;
    }
    return nGramAlphabetMap;
}

int getTotalOccurrences(map<string, int> frequency) {
    int total = 0;
    for (auto element: frequency) {
        total += element.second;
    }
    return total;
}

float getEntropy(map<string, int> frequency) {
    float entropy = 0.0f;

    const int total = getTotalOccurrences(frequency);

    for (auto element: frequency) {
        float probabilityOfElement = element.second / (float) total;
        entropy += probabilityOfElement * log2(probabilityOfElement);
    }
    return -1 * entropy;
}

string lempelZivEncoding(string input) {
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

void lempelZivDecoding(string input) {

}

void huffmanEncoding(string input) {

}

void huffmanDecoding() {

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
//    cout << getEntropy(frequency);

    string test = "Hello My name is Brandon Olson and I am here to party!!!";
    cout << test << "\n";
    cout << lempelZivEncoding(test) << "\n";
    return 0;
}