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

string cleanText(string input) {
    string output;
    for(char& x : input) {
        if(isalpha(x)) {
            output += tolower(x);
        }
    }
    return output;
}

void writeStringToFile(string outputPath, string outputData) {
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

    const float total = getTotalOccurrences(frequency);

    for (const auto& element: frequency) {
        float probabilityOfElement = element.second / total;
        entropy += probabilityOfElement * log2(probabilityOfElement);
    }
    return -1 * entropy;
}

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
        relativeEntropy += probabilityP * (probabilityP / probabilityQ);
    }
    return relativeEntropy;
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

string huffmanEncoding(string input, map<string, string> codebook) {
    string encoding = "";
    for (char x : input) {
        encoding += codebook.at(string(1, x));
    }
    return encoding;
}

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

void testing() {
    cout << "Starting Information Theory Midterm\n";
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
//    vector<pair<string, int>> x;
//    for (auto element: frequency) {
//        cout << element.first << " " << element.second << "\n";
//        x.push_back(pair<string, int>(element.first, element.second));
//    }
//    sort(x.begin(), x.end(), [](const pair<string, int> &left, const pair<string, int> &right) {
//        return left.second < right.second;
//    });
//    cout << "\n";
//    for (auto element: x) {
//        cout << element.first << " " << element.second << "\n";
//    }
//    cout << getEntropy(frequency) << "\n";

//    HuffmanTree huffmanTree = HuffmanTree(frequency);
//    huffmanTree.print();
//    map<string, string> codebook;
//    huffmanTree.getCodebook(huffmanTree.root, "", codebook);
//    cout << "\nCodebook\n";
//    for (auto element : codebook) {
//        cout << element.first << " " << element.second << "\n";
//    }
//
//    for (auto element : huffmanTree.getDecodebook()) {
//        cout << element.first << " " << element.second << "\n";
//    }
//
//    string input = "aasdfaascdcs";
//    cout << "Input: " << "\n";
//    cout << input << "\n";
//    cout << "Compressed: " << "\n";
//    string compressed = huffmanEncoding(input);
//    cout << compressed << "\n";
//    cout << "Decompressed: " << "\n";
//    string decompressed = huffmanDecoding(compressed, huffmanTree.getDecodebook());
//    cout << decompressed << "\n";
}

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

void printFrequency(map<string, int> frequency) {
    for (const auto& element : frequency) {
        cout << element.first << ", " << element.second << "\n";
    }
    cout << "\n";
}

bool isEnglish(string englishReference, string frenchReference, string input) {
    map<string, int> englishFrequency = getFrequency(englishReference);
    map<string, int> frenchFrequency = getFrequency(frenchReference);
    string cleanedInput = cleanText(input);
    map<string, int> inputFrequency = getFrequency(cleanedInput);
    float relativeEntropyToEnglish = getRelativeEntropy(englishFrequency, inputFrequency);
    float relativeEntropyToFrench = getRelativeEntropy(frenchFrequency, inputFrequency);
    return relativeEntropyToEnglish < relativeEntropyToFrench;
}

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
    //tomCleanText = "aaaaaaaaabbbbbbbbbbbbbbbbbbccccccccc";

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

    double treeBuildTime = 0;
    double encodeTime = 0;
    double decodeTime = 0;

    double cps = CLOCKS_PER_SEC;
    clock_t begin = clock();
    HuffmanTree tomHuffmanTree = HuffmanTree(tomSingleCharacterFrequency);
    clock_t end = clock();
    treeBuildTime = (((double) end - (double) begin) / (double) CLOCKS_PER_SEC) * 1000;
    cout << "Huffman Tree:\n";
    tomHuffmanTree.print();
    cout << "\n";
    map<string, string> codebook = tomHuffmanTree.getCodebook();
    map<string, string> decodebook = tomHuffmanTree.getDecodebook();
    begin = clock();
    string tomHuffmanEncoded = huffmanEncoding(tomCleanText, codebook);
    end = clock();
    encodeTime = (((double) end - (double) begin) / (double) CLOCKS_PER_SEC) * 1000;
    begin = clock();
    string tomHuffmanDecoded = huffmanDecoding(tomHuffmanEncoded, decodebook);
    end = clock();
    decodeTime = (((double) end - (double) begin) / (double) CLOCKS_PER_SEC) * 1000;
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

    unsigned long inputSize = tomCleanText.length() * 8;
    unsigned long encodedSize = tomHuffmanEncoded.length();
    double compressionRatio = (double) encodedSize / (double) inputSize;
    float encodedEntropy = getEntropy(getFrequency(tomHuffmanEncoded));
    cout << "Huffman Data:\n";
    cout << "Input Size         : " << inputSize << " bits\n";
    cout << "Encoded Size       : " << encodedSize << " bits\n";
    cout << "Compression Ratio  : " << compressionRatio << "\n";
    cout << "Input Entropy      : " << tomSingleCharacterEntropy << " bits per character\n";
    cout << "Encoded Entropy    : " << encodedEntropy << " bits per character\n";
    cout << "Tree Build Time    : " << treeBuildTime << " milliseconds\n";
    cout << "Encode Time        : " << encodeTime << " milliseconds\n";
    cout << "Decode Time        : " << decodeTime << " milliseconds\n";
    cout << "\n";

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

    cout << "Hey I did not crash" << std::endl;
}