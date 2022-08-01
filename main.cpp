#include <iostream>
#include <fstream>
#include <unordered_map>
#include <iomanip>
#include <vector>
#include <list>
using namespace std;


void readFile(unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string, string>>& otherGameAttributes, unordered_map<string, bool>& publishers, unordered_map<string, bool>& genres, vector<string>& allTags);

void runMostSimilarWithPQ();
vector<pair<float, string>> findSimilarGames(const string& gameSelected,unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string, string>>& otherGameAttributes, unordered_map<string, bool>& publishers, unordered_map<string, bool>& genres);

void runUnorderedMapTaggedSearch();
vector<pair<float , string>> taggedSearch(unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string, string>>& otherGameAttributes, vector<int>& tagSelection, vector<string>& allTags);

class myPriorityQueue {
private:
    int capacity = 16, size = 0;
    pair<float, string>* topGames;

    float getLoadFactor() const {
        return (float)size / (float)capacity;
    }

public:
    myPriorityQueue() {
        topGames = new pair<float, string>[16];
    }

    ~myPriorityQueue() {
        delete[] topGames;
    }

    int getSize() const {
        return size;
    }

    // Use this when the array is getting too small
    void resize() {
        pair<float, string> * tempData = topGames;
        topGames = new pair<float,string>[capacity * 2];
        capacity = capacity * 2;

        for (int i = 0; i < getSize(); i++) {
            topGames[i].first = tempData[i].first;
            topGames[i].second = tempData[i].second;
        }
        delete[] tempData;
    }

    // Reuse and changed from quiz 7
    pair<float, string> extractMax() {
        // This will return the top value and delete it
        pair<float, string> top = topGames[0];
        topGames[0] = topGames[--size];
        heapifyDown(0);

        return top;
    }

    // Reused and modified from my quiz 7
    void heapifyDown(int index) {
        int leftChildIndex = (2 * index) + 1;
        int rightChildIndex = (2 * index) + 2;

        pair<float, string> parent = topGames[index];
        float  *leftChild = nullptr;
        float *rightChild = nullptr;

        // if the left child is past our size, stop
        if (size <= leftChildIndex)
            return;

        // If there is a left child
        if (size > leftChildIndex)
            leftChild = &topGames[leftChildIndex].first;

        // If there is a right child
        if (size > rightChildIndex)
            rightChild = &topGames[rightChildIndex].first;

        // If there is no right child
        if (rightChild == nullptr) {
            // We look at left child and compare with parent, if child bigger we swap
            if (*leftChild > parent.first) {
                swap(index, leftChildIndex);
                heapifyDown(leftChildIndex);
            }
        }
        else {
            // When there is right child, we see which side is bigger and swap accordingly
            if (*rightChild > *leftChild) {
                if (*rightChild > parent.first) {
                    swap(index, rightChildIndex);
                    heapifyDown(rightChildIndex);
                }
            }
            else if (*leftChild > *rightChild) {
                if (*leftChild > parent.first) {
                    swap(index, leftChildIndex);
                    heapifyDown(leftChildIndex);
                }
            }
            else {
                swap(index, leftChildIndex);
                heapifyDown(leftChildIndex);
            }
        }
    }

    // Got this from Module 5 Heaps. Lecture PPT page 33
    void insert(const pair<float, string>& newItem) {
        // This allows up to dynamically allocate our array memory
        if (getLoadFactor() > 0.5)
            resize();

        size++;
        int childIndex = size - 1;
        int parentIndex = (childIndex - 1) / 2;
        topGames[size - 1] = newItem;

        // We heapifyUp
        while (parentIndex >= 0 && topGames[parentIndex].first < topGames[childIndex].first) {
            swap(parentIndex, childIndex);
            childIndex = parentIndex;
            parentIndex = (childIndex - 1) / 2;
        }
    }

    // Reused and modified from my quiz 7
    void swap(int parentIndex, int childToSwapIndex) {
        pair<float, string> tempHold = topGames[parentIndex];
        topGames[parentIndex] = topGames[childToSwapIndex];
        topGames[childToSwapIndex] = tempHold;
    }
};


int main() {
    int selection = 0;
    vector<pair<float, string>> games;

    cout << "Choose an option" << endl;
    cout << "   1. Find Game similar to: " << endl;
    cout << "   2. Tag Based Search\n" << endl;
    cout << "Enter selection (-1 Exit): ";
    cin >> selection;
    cin.get();

    while (selection != -1) {
        switch (selection) {
            case 1:
                // Print names and prices of games most similar to game given
                runMostSimilarWithPQ();
                break;
            case 2:
                // Print prices of the games found based on tags
                runUnorderedMapTaggedSearch();
                break;
            default:
                cout << "Not an option" << endl;
                break;
        }

        cout << "   1. Find Game similar to: " << endl;
        cout << "   2. Tag Based Search\n" << endl;
        cout << "Enter selection (-1 Exit): ";
        cin >> selection;
        cin.get();
    }

    return 0;
}


void readFile(unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string, string>>& otherGameAttributes, unordered_map<string, bool>& publishers, unordered_map<string, bool>& genres, vector<string>& allTags) {
    string tempLine = {};
    ifstream file("videogames.csv");
    getline(file, tempLine);
    istringstream currentLine(tempLine);

    // Keep a vector with allTags in order to label everything properly, take in all game info
    while (currentLine) {
        string temp = {};
        getline(currentLine, temp, ',');
        allTags.push_back(temp);
    }
    while (getline(file, tempLine)) {
        int i = 1;
        int numTempData;
        string title = {};
        string tempData = {};
        istringstream currentLine(tempLine);

        // Title - i, starts  1 because title is first tag
        getline(currentLine, title, ',');

        // Console
        getline(currentLine, tempData, ',');
        otherGameAttributes[title][allTags[i]] = tempData;
        i++;

        // Used price
        getline(currentLine, tempData, ',');
        otherGameAttributes[title][allTags[i]] = tempData;
        i++;

        // Year released
        getline(currentLine, tempData, ',');
        otherGameAttributes[title][allTags[i]] = tempData;
        i++;

        // Publishers, there are 19
        for (int j = 0; j < 19 ; j++) {
            getline(currentLine, tempData, ',');
            numTempData = stoi(tempData);

            if (numTempData != 0)
                gameAttributes[title][allTags[i]] = numTempData;

            publishers[allTags[i]] = true;
            i++;
        }

        // We go through all the attributes, only add if cvs value is higher than 0 to save space
        while (i < allTags.size() - 2) {
            getline(currentLine, tempData, ',');
            numTempData = stoi(tempData);

            if (numTempData != 0)
                gameAttributes[title][allTags[i]] = numTempData;

            // For the CVS file I use, genres start  43
            if (i > 43)
                genres[allTags[i]] = true;

            i++;
        }

        // Publisher, since commas get in way, it is read last as remaining of line
        getline(currentLine, tempData, '\n');
        otherGameAttributes[title][allTags[i]] = tempData;
    }
}

void runMostSimilarWithPQ() {
    vector<string> allTags;
    vector<pair<float, string>> games;
    unordered_map<string, bool> genres;
    unordered_map<string, bool> publishers;
    unordered_map<string, unordered_map<string, int>> gameAttributes;
    unordered_map<string, unordered_map<string, string>> otherGameAttributes;
        // otherGameAttributes stores anything that is not an int

    readFile(gameAttributes, otherGameAttributes, publishers, genres, allTags);

    // We take in a title for initial loop and then keep asking for next game input, printing each games' most similar games and price.
    cout << "\nInput title (-1 Done): ";
    string selection;
    getline(cin, selection);

    while (selection != "-1") {
        games = findSimilarGames(selection, gameAttributes, otherGameAttributes, publishers, genres);

        if (games.empty()) {
            cout << "No similar games found" << endl;
        }
        else {
            int i = 1;

            cout << "Most Similar Games: " << endl;
            for (auto iter = games.begin(); iter < games.end(); iter++) {
                cout << "   " << i << ". " << iter->second << " Price: $" << otherGameAttributes[iter->second]["Usedprice"] << endl;
                i++;
            }
        }

        cout << "\nInput title (-1 Done): ";
        getline(cin, selection);
    }
}

vector<pair<float , string>> findSimilarGames(const string& gameSelected,unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string, string>>& otherGameAttributes, unordered_map<string, bool>& publishers, unordered_map<string, bool>& genres) {
    vector<pair<float, string>> mostSimilarGames;

    // Weights
    int onlineWeight = 2;
    int publisherWeight = 1;
    int genreWeight = 4;
    int consoleWeight = 1;
    int maxPointsPossible = 0;
    float similarityThreshold = .750;
    myPriorityQueue test;

    // Finding max points of genre to figure out similarity rate by checking through the attributes to see if it's a genre, add corresponding weight.
    for (auto & iter : gameAttributes[gameSelected]) {
        if (genres.find(iter.first) != genres.end())
            maxPointsPossible = maxPointsPossible + genreWeight;
    }
    maxPointsPossible = maxPointsPossible + onlineWeight + publisherWeight + consoleWeight;

    // Points Calculation
    // First is the gameMap, second is attributeMap, second[value] is value of attribute
    auto gamesIter = gameAttributes.begin();
    // game                     attribute               value
    auto selectionIter = gameAttributes[gameSelected].begin();

    // For all the attributes in our game, if they are found in the attributes of other games, depending on what they are, they get added to pq and when extracting, if bigger than threshold, insert to vector
    for (gamesIter = gameAttributes.begin(); gamesIter != gameAttributes.end(); gamesIter++) {
        int points = 0;

        for (selectionIter = gameAttributes[gameSelected].begin();
             selectionIter != gameAttributes[gameSelected].end(); selectionIter++) {
            // Online
            if (gamesIter->second.find(selectionIter->first) != gamesIter->second.end() && selectionIter->first == "Online") {
                points = points + onlineWeight;
            }
            // Publisher
            if (gamesIter->second.find(selectionIter->first) != gamesIter->second.end() && publishers.find(selectionIter->first) != publishers.end()) {
                points = points + publisherWeight;
            }
            // Genre
            if (gamesIter->second.find(selectionIter->first) != gamesIter->second.end() && genres.find(selectionIter->first) != genres.end()) {
                points = points + genreWeight;
            }
        }
        // Console
        if (otherGameAttributes[gameSelected]["Console"] == otherGameAttributes[gamesIter->first]["Console"])
            points = points + consoleWeight;

        // Review Scores are bonus, not part of max points; weights adjusted here.
        if (gamesIter->second["Review Score"] > 89)
            points = points + 2;
        else if (gamesIter->second["Review Score"] > 79)
            points = points + 1;
        else if (gamesIter->second["Review Score"] > 69)
            points = points + 0;
        else if (gamesIter->second["Review Score"] >= 50)
            points = points - 1;
        else if (gamesIter->second["Review Score"] < 50) {
            points = points - 3;
        }

        float similarityRatio = (float) points / (float) maxPointsPossible;

        // Insert to pq, so we can sort based on max similarity when given to vector
        if (similarityRatio > similarityThreshold && gamesIter->first != gameSelected) {
            test.insert(make_pair(similarityRatio, gamesIter->first));
        }
    }

    mostSimilarGames.reserve(test.getSize());
    for (int i = 0 ; i < test.getSize(); i++ ) {
        mostSimilarGames.emplace_back(test.extractMax());
   }

    return mostSimilarGames;
}

void runUnorderedMapTaggedSearch() {
    vector<pair<float , string>> games;
    vector<string> allTags;
    vector<int> tagSelection;
    unordered_map<string, bool> genres;
    unordered_map<string, bool> publishers;
    unordered_map<string, unordered_map<string, int>> gameAttributes;
    unordered_map<string, unordered_map<string, string>> otherGameAttributes;
        // otherGameAttributes stores anything that is not an int

    readFile(gameAttributes, otherGameAttributes, publishers, genres, allTags);

    // print out all the tags and let user keep selecting until they are done
    int selection = -1;
    cout << "\nChoose Tags: " << endl;
    // 0-3 are tags used for other section
    for (int i = 4; i < allTags.size() - 2; i++) {
        if (i == 4)
            cout << "Publishers: " << endl;
        if (i == 24)
            cout << "Year, Rating & Misc: " << endl;
        if(i == 45)
            cout << "Genre Tags: " << endl;

        cout << "   " << i - 3 <<". " << allTags[i] << endl;
    }


    cout << "\nEnter Selection (-1 Done): ";
    cin >> selection;
    cin.get();

    while (selection != -1) {
        tagSelection.push_back(selection);
        cout << "Next Selection (-1 Done): ";
        cin >> selection;
    }

    games = taggedSearch(gameAttributes, otherGameAttributes, tagSelection, allTags);

    if (games.empty()) {
        cout << "No similar games found" << endl;
    }
    else {
        int k = 0;
        cout << "\nMost Similar Games: " << endl;

        for (int i = 1; i < games.size(); i++) {
            cout << "   " << i << ". " << games[i].second << " Price: $" << otherGameAttributes[games[i].second]["Usedprice"] << endl;
            k++;
        }
        cout << endl;
    }
    // Once again need to return the price of an item based on the tags that were chosen
}

vector<pair<float, string>> taggedSearch(unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string, string>>& otherGameAttributes, vector<int>& tagSelection, vector<string>& allTags) {
    myPriorityQueue topGames;
    vector<pair<float , string>> mostSimilarGames;
    auto gamesIter = gameAttributes.begin();

    // We look  all games
    for (gamesIter = gameAttributes.begin(); gamesIter != gameAttributes.end(); gamesIter++) {
        float count = 0;
        bool sameTags = true;



        // For each game, for all the tags we have, if we do not find it in the game we are looking , we can skip it
        for (int i : tagSelection) {
            if ((gamesIter->second.find(allTags[i]) == gamesIter->second.end()) && otherGameAttributes[gamesIter->first].find(allTags[i]) == otherGameAttributes[gamesIter->first].end()) {
                sameTags = false;
                break;
            }
            else {
                count = count + 1;
            }

        }

        if (sameTags) {
            topGames.insert(make_pair(count, gamesIter->first));
        }
    }

    for (int i = topGames.getSize(); i > 0; i--) {
        mostSimilarGames.emplace_back(topGames.extractMax());
    }

    return mostSimilarGames;
}






