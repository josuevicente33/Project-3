#include <iostream>
#include <fstream>
#include <unordered_map>
#include <iomanip>
#include <vector>
#include <list>

#include <string>
#include <algorithm>
#include <regex>
#include<map>
#include<iterator>
#include <bits/stdc++.h>
#define INF 0x3f3f3f3f

using namespace std;

// Alaguvalliappan's Implementation //
class AdjacencyList
{
private:
//Think about what member variables you need to initialize
    map<string, vector<string>> graph;
    map<string, vector<pair<string,double>>> graphing;
    map<string, int> mapping;
    map<string, vector<pair<string,double>>> list;
    map<string, vector<double>> tally;
    map<string, double> product_list;
    vector<string> collection;
    map<string, int> column_names;

    int counter = 0;
public:
    map<string, int>& getColumnNames(){
        return column_names;
    }

    void csvToMap()
    {
        fstream fin;
        fin.open("videogames.csv",ios::in);
        vector<string> row;


        string key,line, word, temp;
        getline(fin,line);
        int tally=-1;
        stringstream s(line);
        while(getline(s,word,','))
        {
            if(tally==-1)
            {
                tally=0;
            }
            else
            {
                column_names[word]=tally;
                tally=tally+1;
            }
        }
        int i=0;
        while(getline(fin,line))
        {
            stringstream s(line);
            int counter = 0;
            while(getline(s,word,','))
            {

                if(counter==0)
                {


                    key=word;
                    counter=1;
                    collection.push_back(word);

                }
                else
                {
                    graph[key].push_back(word);
                    collection.push_back(word);
                }
            }
        }
        /*map<string, vector<string>>::iterator it;
        for (it =  graph.begin(); it != graph.end(); it++)
        {
            cout << it->first << ":";
            for(int i=0;i<it->second.size();i++)
            {
                cout << it->second.at(i) << ",";
            }
            cout << "\n";
        }*/


    }
    void mapToGraph()
    {
        map<string, vector<string>>::iterator it;
        map<string, vector<string>>::iterator it2;
        int index=0;
        vector<int> vect;
        for (it = graph.begin(); it != graph.end(); it++)
        {
            //cout << it->first << " ";
            for (it2 = graph.begin(); it2 != graph.end(); it2++)
            {

                //cout << it->second.at(31) << "\n";
                double weight=0;
                if(it2->first!=it->first)
                {
                    int Publisher, Rating, Console, Genre, Handheld, Multiplayer,Review;
                    //Same Publisher
                    for(int i=3; i < 22; i++)
                    {
                        if(it->second.at(i)=="1" && it2->second.at(i)=="1")
                        {
                            weight+=2;
                        }
                    }

                    //Same Console
                    if(it->second.at(0)==it2->second.at(0))
                    {
                        weight+=1;
                    }

                    //Both Multiplayer or Both Single Player
                    if(stoi(it->second.at(36))>1 && stoi(it2->second.at(36))>1)
                    {
                        weight+=2;
                    }
                    if(stoi(it->second.at(36))==1 && stoi(it2->second.at(36))==1)
                    {
                        weight+=2;
                    }
                    //ESRB Rating
                    for(int i=32; i < 35; i++)
                    {
                        if(it->second.at(i)=="1" && it2->second.at(i)=="1")
                        {
                            weight+=1;
                        }
                    }
                    //Same Genre
                    for(int i=43; i < 145; i++)
                    {
                        if(it->second.at(i)=="1" && it2->second.at(i)=="1")
                        {
                            weight+=4;
                        }
                    }
                    //If it's Handheld or Not
                    if(it->second.at(39)==it2->second.at(39))
                    {
                        weight+=0;
                    }
                    //Different Weights depending on highly the Game is Rated
                    if(stoi(it2->second.at(31))>90)
                    {
                        weight+=2;
                    }
                    else if(stoi(it2->second.at(31))>80)
                    {
                        weight+=1;
                    }
                    else if(stoi(it2->second.at(31))>70)
                    {
                        weight=weight;
                    }
                    else if(stoi(it2->second.at(31))>60)
                    {
                        weight=weight-1;
                    }
                    if(stoi(it2->second.at(31))<=60)
                    {
                        weight=weight-3;
                    }
                }
                if(weight > 11)
                {
                    graphing[it->first].push_back(make_pair(it2->first, 1/weight));
                }
            }
        }

        /*map<string, vector<pair<string,double>>>::iterator it3;
        it3=graphing.find("Shadow of Destiny");
        for(int i=0;i<it3->second.size();i++)
        {
            cout << it3->second.at(i).first  <<" " << it3->second.at(i).second << "\n";
        }*/


    }
    //Taken from Geeks to Geeks
    static bool cmp(pair<string, double>& a,pair<string, double>& b)
    {
        return a.second > b.second;
    }
    static bool cmp2(pair<string, double>& a,pair<string, double>& b)
    {
        return a.second < b.second;
    }
    //Used Geeks for Geeks as reference for this function
    void Search(string source)
    {
        map<string, vector<pair<string,double>>>::iterator it;
        priority_queue<pair<int,string>,vector<pair<int,string>>, greater<pair<int,string>>> pq;
        map<string,double> dist;
        for (it =  graphing.begin(); it != graphing.end(); it++)
        {
            dist[it->first]=1000000000;
        }

        pq.push(make_pair(0, source));
        dist[source] = 0;

        while (!pq.empty())
        {
            string title = pq.top().second;
            pq.pop();

            // 'i' is used to get all adjacent vertices of a
            // vertex
            it=graphing.find(title);
            map<string,double>::iterator it9;
            it9=dist.find(title);

            for(int i=0; i < it->second.size(); i++)
            {
                double weight = it->second.at(i).second;
                string new_title = it->second.at(i).first;
                map<string,double>::iterator it8;
                /*it8=dist.find(new_title);

                if (it8->second > it9->second + weight)
                {
                    it8->second = it9->second + weight;
                    pq.push(make_pair(it8->second, new_title));
                }*/
                if (dist.at(new_title)> dist.at(title) + weight)
                {
                    dist.at(new_title) = dist.at(title) + weight;
                    pq.push(make_pair(dist.at(new_title), new_title));
                }
            }

        }
        vector<pair<string, double>> A;
        vector<pair<string, double>>::iterator it7;
        map<string,double>::iterator it6;
        for (it6 = dist.begin(); it6 != dist.end(); it6++)
        {
            //cout << it6->first << it6->second << "\n";
            A.push_back(make_pair(it6->first, it6->second));
        }
        sort(A.begin(), A.end(), cmp2);
        int counter = 1;
        for (it7 = A.begin()+1; it7 != A.end(); it7++)
        {
            if(counter<20)
            {
                cout << std::fixed;
                cout << setprecision(2);
                cout << counter << "." << it7->first << ": "<< it7->second << "\n";
                counter++;
            }

        }
    }
    void Restrict(string Publisher, string Genre, string Rating, string Console)
    {
        // Title, columns everything else
        map<string,vector<string>>::iterator it2;
        vector<string> restrictedTitles;
        map<string, vector<pair<string,double>>>::iterator it4;
        map<string, vector<pair<string,double>>> graphing_copy;
        map<string,int>::iterator publisher = column_names.find(Publisher);
        map<string,int>::iterator genre = column_names.find(Genre);
        map<string,int>::iterator rating = column_names.find(Rating);
        map<string,int>::iterator console= column_names.find(Console);
        bool requirements=true;

        graphing_copy=graphing;
        for (it2 =  graph.begin(); it2 != graph.end(); it2++) {
            if ((publisher == column_names.end() || (it2->second.at(publisher->second) == "1")) && ( genre == column_names.end() || it2->second.at((column_names.find(Genre)->second)) == "1") &&
                (rating==column_names.end() ||it2->second.at((column_names.find(Rating)->second)) == "1" ) && (console==column_names.end() || (it2->second.at((column_names.find(Console)->second))) == "1"))
            {
                restrictedTitles.push_back(it2->first);
            }
        }

        for (it4 =  graphing_copy.begin(); it4 != graphing_copy.end(); it4++)
        {
            if(find(restrictedTitles.begin(), restrictedTitles.end(), it4->first) == restrictedTitles.end())
            {
                graphing_copy.erase(it4->first);
            }
            else
            {
                for(int i=0; i < it4->second.size(); i++)
                {
                    if(find(restrictedTitles.begin(), restrictedTitles.end(), it4->second.at(i).first) == restrictedTitles.end())
                    {
                        (it4->second).erase(it4->second.begin()+i);
                    }

                }
            }
        }
        PageRank(3,graphing_copy);
    }
    map<string, vector<pair<string,double>>>  Conversion(map<string, vector<pair<string,double>>> list,map<string, vector<double>> tally)
    {
        //Iterate through the map and for each value in the map, iterate over the vector
        //Get a tally for the outdegree of each node in the graph
        //The variable tally will store each node as a key, and the outdegree as the value
        map<string, vector<pair<string,double>>>::iterator it;
        vector<int> vect;
        for (it =  list.begin(); it != list.end(); it++)
        {
            for(int i=0; i < it->second.size(); i++)
            {
                tally[it->second.at(i).first].push_back(1);
            }
        }
        //Iterate through the list once more and for the second element
        //in each pair in the respective vector divide by the number of times
        //the website in the first element of the vector links to another website
        for (it =  list.begin(); it != list.end(); it++)
        {
            for(int i=0; i < it->second.size(); i++)
            {
                it->second.at(i).second=1/double(tally.at(it->second.at(i).first).size());
            }
        }
        return list;

    };
    void PageRank(int n,map<string, vector<pair<string,double>>> graphing_copy)
    {
        PageRankHelper(n, list, product_list, tally,mapping,graphing_copy);
    }

    void PageRankHelper(int n, map<string, vector<pair<string,double>>> list, map<string,double> product_list, map<string, vector<double>> tally,map<string, int> mapping, map<string, vector<pair<string,double>>> graphing_copy)
    {
        list = graphing_copy;
        map<string, vector<pair<string,double>>>::iterator it;
        list = Conversion(list,tally);
        map<string, vector<pair<string,double>>>::iterator it2;
        map<string, double>::iterator it3;
        map<string, double>::iterator it4;
        //Iterate through the length of the list to initialize product_list
        //which is a map that stores the rankings of the webpages.
        //We initialize the ranking of every website to be 1/length of the list or 1/|V|
        for (it =  list.begin(); it != list.end(); it++)
        {
            product_list[it->first]=1/double(list.size());
        }

        while(n>1)
        {
            int count=0;

            double product[product_list.size()];
            //Iterate through the verticies and edges and multiply a given vertex
            //to the product_list to get the ranking for a given vertex for a given iteration
            for (it2 =  list.begin(); it2 != list.end(); it2++)
            {
                product[count]=0;
                for(int i=0; i < it2->second.size(); i++)
                {
                    it3 = product_list.find(it2->second.at(i).first);

                    product[count]+=(it3->second)*(it2->second.at(i).second);
                }
                count++;

            }
            count=0;
            //We update the product_list with the new rankings stored in the product array
            for (it3 =  product_list.begin(); it3 != product_list.end(); it3++)
            {
                product_list[it3->first]=product[count];
                count++;
            }
            n=n-1;
        }
        //Iterate through mapping, find the corresponding entry in product_list
        //and then print out the website and its ranking.
        int counter = 1;
        vector<pair<string, double>> A;
        vector<pair<string, double>>::iterator it7;
        map<string,double>::iterator it6;
        for (it6 = product_list.begin(); it6 != product_list.end(); it6++)
        {
            //cout << it6->first << it6->second << "\n";
            A.push_back(make_pair(it6->first, it6->second));
        }
        sort(A.begin(), A.end(), cmp);
        for (it7 = A.begin(); it7 != A.end(); it7++)
        {
            cout << std::fixed;
            cout << setprecision(2);
            cout << counter << "." << it7->first << ": "<< it7->second << "\n";
            counter++;
        }


    };

//Think about what helper functions you will need in the algorithm
};

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

    pair<float, string>* getQueue() {
        return topGames;
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

void readFile(unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string, string>>& otherGameAttributes,
              unordered_map<string, bool>& publishers, unordered_map<string, bool>& genres, vector<string>& allTags);

void runMostSimilarWithPQ();
void findSimilarGames(const string &gameSelected, unordered_map<string, unordered_map<string, int>> &gameAttributes, unordered_map<string, unordered_map<string, string>> &otherGameAttributes,
                      unordered_map<string, bool> &publishers, unordered_map<string, bool> &genres, myPriorityQueue& queue);

void runPQTaggedSearch();
void taggedSearch(unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string, string>>& otherGameAttributes,
                  vector<int>& tagSelection, vector<string>& allTags, myPriorityQueue& topGames);

int main() {
    int selection = 0;
    vector<pair<float, string>> games;

    cout << "Choose: " << endl;
    cout << "   1. Adjacency List" << endl;
    cout << "   2. Max Heap" << endl;
    cout << "Enter selection (-1 Exit): ";
    cin >> selection;
    cin.get();

    if (selection == 1) {
        string gameChosen;
        cout << "Loading..." << endl;

        AdjacencyList Created_Graph;
        Created_Graph.csvToMap();
        Created_Graph.mapToGraph();

        cout << "Loading Done.\n" << endl;

        cout << "Choose an option" << endl;
        cout << "   1. Find Game similar to: " << endl;
        cout << "   2. Tag Based Search\n" << endl;
        cout << "Enter selection (-1 Exit): ";
        cin >> selection;
        cin.get();

        while (selection != -1) {
            int i = 0;
            int k = 1;
            string tagSelection;
            vector<string> tags = {};

            switch (selection) {
                case 1:
                    // Print names and prices of games most similar to game given
                    cout << "Input Title: ";
                    getline(cin, gameChosen);
                    cout << endl << "Most Similar Games: " << endl;

                    Created_Graph.Search(gameChosen);
                    cout << endl;
                    break;
                case 2:
                    for (auto colIter = Created_Graph.getColumnNames().begin();  colIter != Created_Graph.getColumnNames().end(); colIter++) {
                        cout << "   " << i << ". " << colIter->first << endl;
                        i++;
                    }

                    cout << endl;
                    cout << "Select a publisher or enter \"Any\": ";
                    cin >> tagSelection;
                    tags.emplace_back(tagSelection);

                    cout << "Select a genre or enter \"Any\": ";
                    cin >> tagSelection;
                    tags.emplace_back(tagSelection);

                    cout << "Select a ESRB Rating or enter \"Any\": ";
                    cin >> tagSelection;
                    tags.emplace_back(tagSelection);

                    cout << "Select a console or enter \"Any\": ";
                    cin >> tagSelection;
                    tags.emplace_back(tagSelection);

                    Created_Graph.Restrict(tags[0],tags[1],tags[2],tags[3]);

                    break;
                default:
                    cout << "Not an option" << endl;
                    break;
            }

            cout << "Choose an option" << endl;
            cout << "   1. Find Game similar to: " << endl;
            cout << "   2. Tag Based Search\n" << endl;
            cout << "Enter selection (-1 Exit): ";
            cin >> selection;
            cin.get();
        }
    }
    else if (selection == 2) {
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
                    runPQTaggedSearch();
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
    }
    return 0;
}


void readFile(unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string,
        string>>& otherGameAttributes,unordered_map<string, bool>& publishers,
              unordered_map<string, bool>& genres, vector<string>& allTags) {

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
        myPriorityQueue topGamesQueue;
        findSimilarGames(selection, gameAttributes, otherGameAttributes, publishers, genres, topGamesQueue);

        if (topGamesQueue.getSize() == 0) {
            cout << "No similar games found" << endl;
        }
        else {
            int k = 1;

            cout << "Most Similar Games: " << endl;
            for (int i = topGamesQueue.getSize(); i > 1; i--) {
                pair<float, string> top = topGamesQueue.extractMax();
                cout << "   " << k << ". " << top.second << " Price: $" << otherGameAttributes[top.second]["Usedprice"] << endl;
                k++;
            }
        }

        cout << "\nInput title (-1 Done): ";
        getline(cin, selection);
    }
}

void findSimilarGames(const string& gameSelected,unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string, string>>& otherGameAttributes, unordered_map<string,
        bool>& publishers, unordered_map<string, bool>& genres, myPriorityQueue& topGamesQueue) {

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
        if (similarityRatio > similarityThreshold && gamesIter->first != gameSelected) {
            topGamesQueue.insert(make_pair(similarityRatio, gamesIter->first));
        }
    }
}

void runPQTaggedSearch() {
    vector<string> allTags;
    vector<int> tagSelection = {};
    myPriorityQueue topGamesQueue;
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
        if (selection > 0 && selection < 142)
            tagSelection.push_back(selection + 3);
        else
            cout << "Invalid, try again." << endl;

        cout << "Next Selection (-1 Done): ";
        cin >> selection;
    }

    taggedSearch(gameAttributes, otherGameAttributes, tagSelection, allTags, topGamesQueue);

    if (topGamesQueue.getSize() == 0) {
        cout << "No similar games found" << endl;
    }
    else {
        int k = 1;

        cout << "Most Similar Games: " << endl;
        for (int i = topGamesQueue.getSize(); i > 1; i--) {
            pair<float, string> top = topGamesQueue.extractMax();
            cout << "   " << k << ". " << top.second << " Price: $" << otherGameAttributes[top.second]["Usedprice"] << endl;
            k++;
        }
        cout << endl;
        cout << "Next Option: " << endl;
    }
}

void taggedSearch(unordered_map<string, unordered_map<string, int>>& gameAttributes, unordered_map<string, unordered_map<string, string>>& otherGameAttributes,
                  vector<int>& tagSelection, vector<string>& allTags, myPriorityQueue& topGamesQueue) {
    auto gamesIter = gameAttributes.begin();

    // We look all games
    for (gamesIter = gameAttributes.begin(); gamesIter != gameAttributes.end(); gamesIter++) {
        float count = 0;
        bool sameTags = true;

        // For each game, for all the tags we have, if we do not find it in the game we are looking , we can skip it
        for (int i = 0; i < tagSelection.size(); i++) {
            if ((gamesIter->second.find(allTags[tagSelection[i]]) == gamesIter->second.end()) && otherGameAttributes[gamesIter->first].find(allTags[tagSelection[i]]) == otherGameAttributes[gamesIter->first].end()) {
                sameTags = false;
                break;
            }
            else
                count = count + 1;
        }

        if (sameTags)
            topGamesQueue.insert(make_pair(count, gamesIter->first));
    }
}