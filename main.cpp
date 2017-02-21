// includes
#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include <regex>
#include <iomanip>
#include <ctime>
#include "HuffmanTree.h"

// using statements
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
using std::vector;
using std::find;
using std::regex;
using std::smatch;
using std::regex_match;
using std::regex_replace;
using std::sort;
using std::pair;
using std::istreambuf_iterator;
using std::endl;

// cleans the text found in one file and places the results in an output file
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

// cleans a single string
string cleanText(string input) {
    string output;
    for(char& x : input) {
        if(isalpha(x)) {
            output += tolower(x);
        }
    }
    return output;
}

// writes a string to a given file
void writeStringToFile(string outputPath, string outputData) {
    ofstream outputFile(outputPath, ios::trunc);
    outputFile << outputData << "\n";
    outputFile.close();
}

// returns a map with strings as keys and int number of occurrences
map<string, int> getFrequency(string input, const unsigned long nGrams = 1) {
    map<string, int> frequency;
    for (unsigned int i = 0; i < input.length(); i += nGrams) {
        const string nGram = input.substr(i, nGrams);
        frequency[nGram] += 1;
    }
    return frequency;
}

// returns the total number of occurrences for all keys in the map
int getTotalOccurrences(map<string, int> frequency) {
    int total = 0;
    for (const auto& element: frequency) {
        total += element.second;
    }
    return total;
}

// returns the entropy of the given frequency
float getEntropy(map<string, int> frequency) {
    float entropy = 0.0f;

    const float total = getTotalOccurrences(frequency);

    for (const auto& element: frequency) {
        float probabilityOfElement = element.second / total;
        entropy += probabilityOfElement * log2(probabilityOfElement);
    }
    return -1 * entropy;
}

// returns teh relative entropy of the two given frequencies
float getRelativeEntropy(map<string, int> frequencyP, map<string, int> frequencyQ) {
    float relativeEntropy = 0.0f;
    const float totalP = getTotalOccurrences(frequencyP);
    const float totalQ = getTotalOccurrences(frequencyQ);
    for (const auto& element : frequencyQ) {
        const int currentQOccurrences = element.second;
        auto tempOccurence = frequencyP[element.first];
        const int currentPOccurrences = tempOccurence;
        const float probabilityP = currentPOccurrences / totalP;
        const float probabilityQ = currentQOccurrences / totalQ;
        relativeEntropy += probabilityP * log2(probabilityP / probabilityQ);
    }
    return relativeEntropy;
}

// returns all the matches in a string based on a regex pattern
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

// returns a lempel ziv encoded string
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

// returns a decoded lempel ziv encoded string
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

// returns a huffman encoded string when given a huffman codebook generated from a huffman tree
string huffmanEncoding(string input, map<string, string> codebook) {
    string encoding = "";
    for (char x : input) {
        encoding += codebook.at(string(1, x));
    }
    return encoding;
}

// returns a huffman decoded string when given a huffman decodebook generated from a huffman tree
string huffmanDecoding(string input, map<string, string> decodebook) {
    string decoding = "";
    for (unsigned long i = 0; i < input.size();) {
        for (unsigned long j = 1; j + i <= input.size(); j++) {
            string code = input.substr(i, j);
            auto decodeIterator = decodebook.find(code);
            if (decodeIterator != decodebook.end()) {
                decoding += decodeIterator->second;
                i += j;
                break;
            }
        }
    }
    return decoding;
}

// gets all the text from a given text file
string getTextFromFile(string filepath) {
    // open the input file
    ifstream inputTextFile(filepath, ios::in|ios::ate);

    // create a string buffer to hold the new output file
    string outputData;
    outputData.reserve((unsigned int) inputTextFile.tellg());

    // set the file pointer back to the beginning
    inputTextFile.seekg(ios::beg);

    string currentLine;
    while (getline(inputTextFile, currentLine)) {
        outputData += currentLine;
    }
    inputTextFile.close();
    return outputData;
}

// outputs the values of a given frequency to the console
void printFrequency(map<string, int> frequency) {
    for (const auto& element : frequency) {
        cout << element.first << ", " << element.second << "\n";
    }
    cout << "\n";
}

// determines if a given input string is english or french
bool isEnglish(string englishReference, string frenchReference, string input) {
    map<string, int> englishFrequency = getFrequency(englishReference);
    map<string, int> frenchFrequency = getFrequency(frenchReference);
    string cleanedInput = cleanText(input);
    map<string, int> inputFrequency = getFrequency(cleanedInput);
    float relativeEntropyToEnglish = getRelativeEntropy(englishFrequency, inputFrequency);
    float relativeEntropyToFrench = getRelativeEntropy(frenchFrequency, inputFrequency);
    return relativeEntropyToEnglish < relativeEntropyToFrench;
}

// main function attempts to show work for all the questions on the information theory midterm
int main() {
    cout << "Starting Information Theory Midterm\n\n";
    string tomPath = "text/tom.txt";
    string tomCleanPath = "text/tom-clean.txt";
    string germainPath = "text/germaine.txt";
    string germainCleanPath = "text/germain-clean.txt";
    string huffmanOutputPath = "text/tom-huffman.txt";
    string lempelZivOutputPath = "text/tom-lempelziv.txt";

    cout << "Cleaning Text File " << tomPath << "\n\n";
    cleanTextFile(tomPath, tomCleanPath);
    cout << "Cleaned Text File " << tomPath << " to " << tomCleanPath << "\n\n";

    cout << "Cleaning Text File " << germainPath << "\n\n";
    cleanTextFile(germainPath, germainCleanPath);
    cout << "Cleaned Text File " << germainPath << " to " << germainCleanPath << "\n\n";

    // Clean Tom Text
    cout << "Retrieving Cleaned Text Files\n\n";
    string tomCleanText = getTextFromFile(tomCleanPath);
    string germaineCleanText = getTextFromFile(germainCleanPath);

    // Frequency Calculations
    cout << "Calculating Frequencies\n\n";
    map<string, int> tomSingleCharacterFrequency = getFrequency(tomCleanText, 1);
    map<string, int> tomBiGramFrequency = getFrequency(tomCleanText, 2);
    map<string, int> tomTriGramFrequency = getFrequency(tomCleanText, 3);


    // Entropy Calculations
    cout << "Calculating Entropies\n\n";
    float tomSingleCharacterEntropy = getEntropy(tomSingleCharacterFrequency);
    float tomBiGramEntropy = getEntropy(tomBiGramFrequency);
    float tomTriGramEntropy = getEntropy(tomTriGramFrequency);

    cout << "Single Character Frequency:\n";
    printFrequency(tomSingleCharacterFrequency);

    cout << "Entropies\n";
    cout << "Single Character Entropy : " << tomSingleCharacterEntropy << " bits per character\n";
    cout << "Bigram Entropy           : " << tomBiGramEntropy << " bits per bigram\n";
    cout << "TriGram Entropy          : " << tomTriGramEntropy << " bits per trigram\n\n";

    cout << "Calculating Lempel-Ziv Stuff:\n";
    double lempelZivEncodeTime = 0;
    double lempelZivDecodeTime = 0;

    clock_t begin = clock();
    string tomLempelZivEncoded = lempelZivEncoding(tomCleanText);
    clock_t end = clock();
    lempelZivEncodeTime = (((double) end - (double) begin) / (double) CLOCKS_PER_SEC) * 1000;
    begin = clock();
    string tomLempelZivDecoded = lempelZivDecoding(tomLempelZivEncoded);
    end = clock();
    lempelZivEncodeTime = (((double) end - (double) begin) / (double) CLOCKS_PER_SEC) * 1000;

    cout << "Saved Lempel Ziv Encoded File to " << lempelZivOutputPath << "\n\n";
    writeStringToFile(lempelZivOutputPath, tomLempelZivEncoded);

    if (tomCleanText.length() > 50) {
        cout << "Note: The input was to long to display fully here.\n";
        cout << "Input   : " << tomCleanText.substr(0, 50) << "...\n";
        cout << "Encoded : " << tomLempelZivEncoded.substr(0, 50) << "...\n";
        cout << "Decoded : " << tomLempelZivDecoded.substr(0, 50) << "...\n";
    } else {
        cout << "Input   : " << tomCleanText << "\n";
        cout << "Encoded : " << tomLempelZivEncoded << "\n";
        cout << "Decoded : " << tomLempelZivDecoded << "\n";
    }
    cout << "\n";

    unsigned long inputSize = tomCleanText.length() * 8;
    unsigned long encodedSize = tomLempelZivEncoded.length() * 8;
    double compressionRatio = (double) encodedSize / (double) inputSize;
    float encodedEntropy = getEntropy(getFrequency(tomLempelZivEncoded));
    cout << "Lempel Ziv Data:\n";
    cout << "Input Size         : " << inputSize << " bits\n";
    cout << "Encoded Size       : " << encodedSize << " bits\n";
    cout << "Compression Ratio  : " << compressionRatio << "\n";
    cout << "Input Entropy      : " << tomSingleCharacterEntropy << " bits per character\n";
    cout << "Encoded Entropy    : " << encodedEntropy << " bits per character\n";
    cout << "Encode Time        : " << lempelZivEncodeTime << " milliseconds\n";
    cout << "Decode Time        : " << lempelZivDecodeTime << " milliseconds\n";
    cout << "\n";


    cout << "Calculating Huffman Stuff:\n";
    double treeBuildTime = 0;
    double huffmanEncodeTime = 0;
    double huffmanDecodeTime = 0;

    begin = clock();
    HuffmanTree tomHuffmanTree = HuffmanTree(tomSingleCharacterFrequency);
    end = clock();
    treeBuildTime = (((double) end - (double) begin) / (double) CLOCKS_PER_SEC) * 1000;
    cout << "Huffman Tree:\n";
    tomHuffmanTree.print();
    cout << "\n";
    map<string, string> codebook = tomHuffmanTree.getCodebook();
    map<string, string> decodebook = tomHuffmanTree.getDecodebook();
    begin = clock();
    string tomHuffmanEncoded = huffmanEncoding(tomCleanText, codebook);
    end = clock();
    huffmanEncodeTime = (((double) end - (double) begin) / (double) CLOCKS_PER_SEC) * 1000;
    begin = clock();
    string tomHuffmanDecoded = huffmanDecoding(tomHuffmanEncoded, decodebook);
    end = clock();
    huffmanDecodeTime = (((double) end - (double) begin) / (double) CLOCKS_PER_SEC) * 1000;
    cout << "Codebook:\n";
    tomHuffmanTree.printCodeBook();
    cout << "\n";

    cout << "Saved Huffman Encoded File to " << huffmanOutputPath << "\n\n";
    writeStringToFile(huffmanOutputPath, tomHuffmanEncoded);

    if (tomCleanText.length() > 50) {
        cout << "Note: The input was to long to display fully here.\n";
        cout << "Input   : " << tomCleanText.substr(0, 50) << "...\n";
        cout << "Encoded : " << tomHuffmanEncoded.substr(0, 50) << "...\n";
        cout << "Decoded : " << tomHuffmanDecoded.substr(0, 50) << "...\n";
    } else {
        cout << "Input   : " << tomCleanText << "\n";
        cout << "Encoded : " << tomHuffmanEncoded << "\n";
        cout << "Decoded : " << tomHuffmanDecoded << "\n";
    }
    cout << "\n";

    inputSize = tomCleanText.length() * 8;
    encodedSize = tomHuffmanEncoded.length();
    compressionRatio = (double) encodedSize / (double) inputSize;
    encodedEntropy = getEntropy(getFrequency(tomHuffmanEncoded));
    cout << "Huffman Data:\n";
    cout << "Input Size         : " << inputSize << " bits\n";
    cout << "Encoded Size       : " << encodedSize << " bits\n";
    cout << "Compression Ratio  : " << compressionRatio << "\n";
    cout << "Input Entropy      : " << tomSingleCharacterEntropy << " bits per character\n";
    cout << "Encoded Entropy    : " << encodedEntropy << " bits per character\n";
    cout << "Tree Build Time    : " << treeBuildTime << " milliseconds\n";
    cout << "Encode Time        : " << huffmanEncodeTime << " milliseconds\n";
    cout << "Decode Time        : " << huffmanDecodeTime << " milliseconds\n";
    cout << "\n";

    cout << "Language Dection:\n";
    string englishTest1 = "In the Pride Lands of Africa, a lion rules over the animal kingdom from Pride Rock. King "
            "Mufasa and Queen Sarabi's newborn son, Simba, is presented to the assembled animals by Rafiki, a mandrill "
            "who serves as shaman and advisor. Months later, Mufasa shows young Simba the Pride Lands and explains to him "
            "the responsibilities of kingship and the \"circle of life\" which connects all living things. Mufasa's younger "
            "brother, Scar, covets the throne and plots to eliminate Mufasa and Simba so he may become king. He tricks Simba "
            "and his best friend Nala—to whom Simba is betrothed—into exploring a forbidden elephants' graveyard, where they "
            "are attacked by three spotted hyenas who are in league with Scar. Mufasa is alerted to the danger by his majordomo, "
            "the hornbill Zazu, and rescues the cubs. Though angry with Simba, Mufasa forgives him and explains that the great "
            "kings of the past watch over them from the night sky, from which he will one day watch over Simba.";

    string frenchTest1 = "Dans la savane africaine, tous les animaux de la Terre des Lions se sont réunis pour célébrer "
            "la naissance du prince Simba, fils du roi Mufasa et de la reine Sarabi. Tous, sauf Scar, frère cadet de Mufasa, "
            "pour qui la naissance de cet héritier anéantit tous ses espoirs d'accéder un jour au pouvoir. Avec la collaboration "
            "des hyènes, Scar imagine plusieurs plans diaboliques pour anéantir son frère et son neveu. Malgré un premier "
            "échec, il parvient à éliminer Mufasa et persuade Simba qu'il est responsable de la mort de son père pour le "
            "contraindre à s'enfuir et ainsi en faire la proie de ses hyènes. Bien que parvenant à s'échapper, Simba "
            "finit à bout de forces dans le désert. Il est secouru par Timon le suricate et Pumbaa le phacochère, avec "
            "lesquels il va se reconstruire et grandir, en se nourrissant de larves et en suivant une nouvelle "
            "philosophie de vie : Hakuna matata, c'est-à-dire, vivre sans souci, au jour le jour.";

    cout << "The following text:\n" << "\"" << englishTest1 << "\"\n";
    if(isEnglish(tomCleanText, germaineCleanText, englishTest1)) {
        cout << "Is English\n\n";
    } else {
        cout << "Is French\n\n";
    }

    cout << "The following text:\n" << "\"" << frenchTest1 << "\"\n";
    if(isEnglish(tomCleanText, germaineCleanText, frenchTest1)) {
        cout << "Is English\n\n";
    } else {
        cout << "Is French\n\n";
    }
}