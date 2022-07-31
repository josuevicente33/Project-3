#include <iostream>
#include <fstream>
#include <unordered_map>
#include <iomanip>
#include <vector>
#include <queue>
using namespace std;

void readFile(unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string, string>>& otherGameAttributes, unordered_map<string, bool>& publishers, unordered_map<string, bool>& genres, vector<string>& allTags);

vector<pair<float, string>> runUnorderedMapSearch();
vector<pair<float, string>> findSimilarGames(const string& gameSelected,unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string, string>>& otherGameAttributes, unordered_map<string, bool>& publishers, unordered_map<string, bool>& genres);

vector<pair<float, string>> runUnorderedMapTaggedSearch();
vector<string> taggedSearch(unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string, string>>& otherGameAttributes, vector<int>& tagSelection, vector<string>& allTags);


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
                // Currently, returns empty vector of a pair of float, string. Can be modified. Prints its own prices for now
                games = runUnorderedMapSearch();
                break;
            case 2:
                // Currently, returns empty vector of a pair of float, string. Can be modified. Prints its own prices for now
                games = runUnorderedMapTaggedSearch();
                break;
            default:
                cout << "Not an option" << endl;
                break;
        }

        cout << "   1. Find Game similar to: " << endl;
        cout << "   2. Tag Based Search\n" << endl;
        cout << "Enter selection (-1 Exit): ";
        cin >> selection;
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

        // Title - i start at 1 because title is first tag
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

            // For the CVS file I use, genres start at 43
            if (i > 43)
                genres[allTags[i]] = true;

            i++;
        }

        // Publisher, since commas get in way, it is read last as remaining of line
        getline(currentLine, tempData, '\n');
        otherGameAttributes[title][allTags[i]] = tempData;
    }
}

vector<pair<float, string>> runUnorderedMapSearch() {
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

    return {};
}

vector<pair<float , string>> findSimilarGames(const string& gameSelected,unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string, string>>& otherGameAttributes, unordered_map<string, bool>& publishers, unordered_map<string, bool>& genres) {
    vector<pair<float, string>> mostSimilarGames;
    priority_queue<pair<float, string>> mostSimilarQueue;

    // Weights
    int onlineWeight = 2;
    int publisherWeight = 1;
    int genreWeight = 4;
    int consoleWeight = 1;
    int maxPointsPossible = 0;
    float similarityThreshold = .750;

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
        if (similarityRatio > similarityThreshold && gamesIter->first != gameSelected)
            mostSimilarQueue.push(make_pair(similarityRatio, gamesIter->first));
    }

    for (int i = 0; i < mostSimilarQueue.size(); i++ ) {
        mostSimilarGames.emplace_back(mostSimilarQueue.top());
        mostSimilarQueue.pop();
    }

    return mostSimilarGames;
}

vector<pair<float, string>> runUnorderedMapTaggedSearch() {
    vector<string> games;
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
    int k = 1;
    cout << "\nChoose Tags: " << endl;
    for (int i = 4; i < allTags.size() - 2; i++) {
        if (i == 4)
            cout << "Publishers" << endl;
        if (i == 24)
            cout << "Release Year" << endl;
        if (i == 45)
            cout << "Genres" << endl;


        cout << "   " << k <<". " << allTags[i] << endl;
        k++;
    }


    cout << "\nEnter Selection (-1 Done): ";
    cin >> selection;

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
        cout << "\nMost Similar Games: " << endl;

        for (int i = 1; i < games.size(); i++) {
            cout << "   " << i << ". " << games[i] << " Price: $" << otherGameAttributes[games[i]]["Usedprice"] << endl;
            i++;
        }
        cout << endl;
    }
    // Once again need to return the price of an item based on the tags that were chosen
    return {};
}

vector<string> taggedSearch(unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string, string>>& otherGameAttributes, vector<int>& tagSelection, vector<string>& allTags) {
    vector<string> mostSimilarGames;
    auto gamesIter = gameAttributes.begin();

    // We look at all games
    for (gamesIter = gameAttributes.begin(); gamesIter != gameAttributes.end(); gamesIter++) {
        bool sameTags = true;

        // For each game, for all the tags we have, if we do not find it in the game we are looking at, we can skip it
        for (int i: tagSelection) {
            if ((gamesIter->second.find(allTags[i]) == gamesIter->second.end()) && otherGameAttributes[gamesIter->first].find(allTags[i]) == otherGameAttributes[gamesIter->first].end()) {
                sameTags = false;
                break;
            }
        }

        if (sameTags) {
            mostSimilarGames.push_back(gamesIter->first);
        }
    }

    return mostSimilarGames;
}






