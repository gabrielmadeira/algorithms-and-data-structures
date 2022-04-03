/*
=========================================================================

                    Trabalho Final - FIFA21 - Players
        Classificação e Pesquisa de Dados - INF01124 - 2021/1

                        Integrantes do grupo:
                    Gabriel Madeira (00322863)
                   Henrique Borges Manzke (00326970)

=========================================================================
*/
#include <bits/stdc++.h>
#include "lib/parser.hpp"
#include <chrono>
#include <sstream>
#include <windows.h>

using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
int colisionsCount = 0;
chrono::steady_clock::time_point startTime;
float totalTimeInSeconds()
{
    chrono::steady_clock::time_point endTime = chrono::steady_clock::now();
    return float(chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count() / float(1000000000));
};

// ------------------------------------------- CONSOLE ---------------------------------------------

string prd(const double x, const int decDigits, const int width)
{
    stringstream ss;
    ss << fixed << right;
    ss.fill(' ');            // fill space around displayed #
    ss.width(width);         // set  width around displayed #
    ss.precision(decDigits); // set # places after decimal
    ss << x;
    return ss.str();
}

string prd_string(const string s, const int w)
{
    stringstream ss, spaces;
    int padding = w - s.size(); // count excess room to pad
    for (int i = 0; i < padding; ++i)
        spaces << " ";
    ss << spaces.str() << s; // format with padding
    //if(padding>0 && padding%2!=0)               // if odd #, add 1 space
    //    ss << " ";
    return ss.str();
}

/*! Center-aligns string within a field of width w. Pads with blank spaces
    to enforce alignment. */
string center(const string s, const int w)
{
    stringstream ss, spaces;
    int padding = w - s.size(); // count excess room to pad
    for (int i = 0; i < padding / 2; ++i)
        spaces << " ";
    ss << spaces.str() << s << spaces.str(); // format with padding
    if (padding > 0 && padding % 2 != 0)     // if odd #, add 1 space
        ss << " ";
    return ss.str();
}

void b()
{
    SetConsoleTextAttribute(h, 14);
    cout << " | ";
    SetConsoleTextAttribute(h, 7);
}

void l(int e)
{
    SetConsoleTextAttribute(h, 14);
    cout << " +" << string(e, '-') << "+\n";
    SetConsoleTextAttribute(h, 7);
}

// -------------------- HASH --------------------

struct s_LinkedTree
{
    string c;
    vector<string> vecStr;
    vector<int> vecInt;
    vector<float> vecFlt;
    struct s_LinkedTree *next;
};
typedef struct s_LinkedTree HashNode;

int hashFunc(string c, int m, int funcType) // funcType: 1 = Letra 0 = Numero
{
    int hash = 0;
    if (funcType)
    {
        int lengthC = c.length();
        int p = 122;
        for (int i = 0; i < lengthC; i++)
            hash = (p * hash + c.at(i)) % m;
    }
    else
        hash = (stoi(c)) % m;

    return hash;
}

HashNode *newNode(string c, vector<string> vecStr, vector<int> vecInt, vector<float> vecFlt, HashNode *next)
{
    HashNode *newNode = new HashNode;
    newNode->c = c;
    newNode->vecStr = vecStr;
    newNode->vecInt = vecInt;
    newNode->vecFlt = vecFlt;
    newNode->next = next;
    return newNode;
}

void insert(HashNode *st[], int m, int funcType, string c, vector<string> vecStr, vector<int> vecInt, vector<float> vecFlt)
{
    int i = hashFunc(c, m, funcType);
    int tempAux = 0;
    for (HashNode *x = st[i]; x != NULL; x = (*x).next)
    {
        if (c == (*x).c)
            return;
        tempAux = 1;
    }
    if (tempAux)
        colisionsCount++;
    st[i] = newNode(c, vecStr, vecInt, vecFlt, st[i]);
}

HashNode *search(HashNode *st[], string c, int m, int funcType)
{
    int i = hashFunc(c, m, funcType);
    for (HashNode *x = st[i]; x != NULL; x = (*x).next)
    {
        if (c == (*x).c)
            return x;
    }
    return NULL;
}

// -------------------- HASH --------------------

// -------------------- TRIE --------------------

struct s_TrieNode
{
    char c = 0;
    struct s_TrieNode *left = NULL;
    struct s_TrieNode *mid = NULL;
    struct s_TrieNode *right = NULL;
    string sofifa_id = "n";
};
typedef struct s_TrieNode TrieNode;

TrieNode *get(TrieNode *x, string key, int d)
{
    if (x == NULL)
    {
        cout << "\nnot found\n";
        return NULL;
    }
    char c = key.at(d);
    cout << (*x).c << "-" << (*x).sofifa_id << " ";
    if (c < (*x).c)
        return get((*x).left, key, d);
    if (c > (*x).c)
        return get((*x).right, key, d);
    if (d < key.size() - 1)
        return get((*x).    mid, key, d + 1);

    cout << "\nfound: " << (*x).sofifa_id << "\n";
    return x;
}

vector<string> getPrefixAux(TrieNode *x)
{
    vector<string> nodesWithPrefix;
    if (x == NULL)
        return nodesWithPrefix;

    if ((*x).sofifa_id != "n")
        nodesWithPrefix.push_back((*x).sofifa_id);

    vector<string> nodesWithPrefixLeft = getPrefixAux((*x).left);
    if (!nodesWithPrefixLeft.empty())
        nodesWithPrefix.insert(nodesWithPrefix.end(), nodesWithPrefixLeft.begin(), nodesWithPrefixLeft.end());
    vector<string> nodesWithPrefixMid = getPrefixAux((*x).mid);
    if (!nodesWithPrefixMid.empty())
        nodesWithPrefix.insert(nodesWithPrefix.end(), nodesWithPrefixMid.begin(), nodesWithPrefixMid.end());
    vector<string> nodesWithPrefixRight = getPrefixAux((*x).right);
    if (!nodesWithPrefixRight.empty())
        nodesWithPrefix.insert(nodesWithPrefix.end(), nodesWithPrefixRight.begin(), nodesWithPrefixRight.end());

    return nodesWithPrefix;
}

vector<string> getPrefix(TrieNode *x, string key, int d)
{
    vector<string> nodesWithPrefix;
    if (x == NULL)
    {
        return nodesWithPrefix;
    }
    char c = key.at(d);
    if (c < (*x).c)
        return getPrefix((*x).left, key, d);
    if (c > (*x).c)
        return getPrefix((*x).right, key, d);
    if (d < key.size() - 1)
        return getPrefix((*x).mid, key, d + 1);

    nodesWithPrefix = getPrefixAux((*x).mid);
    if ((*x).sofifa_id != "n")
        nodesWithPrefix.push_back((*x).sofifa_id);

    return nodesWithPrefix;
}

TrieNode *put(TrieNode *x, string key, string sofifa_id, int d)
{
    char c = key.at(d);
    if (x == NULL)
    {
        x = new TrieNode();
        (*x).c = c;
    }
    if (c < (*x).c)
        (*x).left = put((*x).left, key, sofifa_id, d);
    else if (c > (*x).c)
        (*x).right = put((*x).right, key, sofifa_id, d);
    else if (d < key.size() - 1)
        (*x).mid = put((*x).mid, key, sofifa_id, d + 1);
    else
        (*x).sofifa_id = sofifa_id;
    return x;
}

// -------------------- TRIE --------------------

// -------------------- MAIN PROGRAMS --------------------

void playerCommand(TrieNode *trieRoot, HashNode **playersHashTable, int mPlayers, string nameOrPrefix)
{
    vector<string> playersWithPrefix = getPrefix(trieRoot, nameOrPrefix, 0);
    cout << "\n";
    l(114);
    b();
    cout << center("sofifa_id", 10);
    b();
    cout << center("name", 48);
    b();
    cout << center("player_positions", 22);
    b();
    cout << center("rating", 10);
    b();
    cout << center("count", 10);
    b();
    cout << "\n";
    l(114);
    for (int i = 0; i < playersWithPrefix.size(); i++)
    {
        HashNode *player = search(playersHashTable, playersWithPrefix[i], mPlayers, 0);
        b();
        cout << center(playersWithPrefix[i], 10);
        b();
        cout << prd_string((*player).vecStr[0], 48);
        b();
        cout << prd_string((*player).vecStr[1], 22);
        b();
        cout << prd((*player).vecFlt[1], 6, 10);
        b();
        cout << prd((*player).vecInt[0], 0, 10);
        b();
        cout << "\n";
    }
    l(114);
}

void userCommand(HashNode **usersHashTable, int mUsers, HashNode **playersHashTable, int mPlayers, string userID)
{
    HashNode *user = search(usersHashTable, userID, mUsers, 0);
    if (user == NULL)
        return;
    int i = 0;
    cout << "\n";
    l(114);
    b();
    cout << center("sofifa_id", 10);
    b();
    cout << center("name", 48);
    b();
    cout << center("global raiting", 22);
    b();
    cout << center("count", 10);
    b();
    cout << center("rating", 10);
    b();
    cout << "\n";
    l(114);
    while (i < (*user).vecFlt.size())
    {
        HashNode *player = search(playersHashTable, (*user).vecStr[i], mPlayers, 0);
        b();
        cout << center((*user).vecStr[i], 10);
        b();
        cout << prd_string((*player).vecStr[0], 48);
        b();
        cout << prd((*player).vecFlt[1], 6, 22);
        b();
        cout << prd((*player).vecInt[0], 0, 10);
        b();
        cout << prd((*user).vecFlt[i], 1, 10);
        b();
        cout << "\n";
        i++;
    }
    l(114);
}

void topCommand(HashNode **positionsHashTable, int mPositions, HashNode **playersHashTable, int mPlayers, string positionName, int n)
{
    HashNode *position = search(positionsHashTable, positionName, mPositions, 1);
    if (position == NULL)
        return;
    cout << "\n";
    l(114);
    b();
    cout << center("sofifa_id", 10);
    b();
    cout << center("name", 48);
    b();
    cout << center("player_positions", 22);
    b();
    cout << center("rating", 10);
    b();
    cout << center("count", 10);
    b();
    cout << "\n";
    l(114);
    int i = 0;
    while (i < n && i < (*position).vecFlt.size())
    {
        HashNode *player = search(playersHashTable, (*position).vecStr[i], mPlayers, 0);
        b();
        cout << center((*position).vecStr[i], 10);
        b();
        cout << prd_string((*player).vecStr[0], 48);
        b();
        cout << prd_string((*player).vecStr[1], 22);
        b();
        cout << prd((*player).vecFlt[1], 6, 10);
        b();
        cout << prd((*player).vecInt[0], 0, 10);
        b();
        cout << "\n";
        i++;
    }
    l(114);
}

void tagCommand(HashNode **tagsHashTable, int mTags, HashNode **playersHashTable, int mPlayers, vector<string> tags)
{
    HashNode *tag = search(tagsHashTable, tags.back(), mTags, 1);
    if (tag == NULL)
    {
        SetConsoleTextAttribute(h, 12);
        cout << "\nNo players were found with tags that match this input.\n";
        SetConsoleTextAttribute(h, 7);
        return;
    }
    vector<string> tagsPlayerIDs = (*tag).vecStr;
    tags.pop_back();

    while (!tags.empty())
    {
        tag = search(tagsHashTable, tags.back(), mTags, 1);
        if (tag == NULL)
            return;
        vector<string> currentTagPlayerIDs = (*tag).vecStr;

        // todo: sort and then implement a better intersection algorithm
        int i = 0;
        while (i < tagsPlayerIDs.size())
        {
            int found = 0;
            for (int j = 0; j < currentTagPlayerIDs.size(); j++)
            {
                if (tagsPlayerIDs[i] == currentTagPlayerIDs[j])
                {
                    found = 1;
                    break;
                }
            }
            if (found)
                i++;
            else
                tagsPlayerIDs.erase(tagsPlayerIDs.begin() + i);
        }
        tags.pop_back();
    }

    l(114);
    b();
    cout << center("sofifa_id", 10);
    b();
    cout << center("name", 48);
    b();
    cout << center("player_positions", 22);
    b();
    cout << center("rating", 10);
    b();
    cout << center("count", 10);
    b();
    cout << "\n";
    l(114);
    for (int i = 0; i < tagsPlayerIDs.size(); i++)
    {
        HashNode *player = search(playersHashTable, tagsPlayerIDs[i], mPlayers, 0);
        b();
        cout << center(tagsPlayerIDs[i], 10);
        b();
        cout << prd_string((*player).vecStr[0], 48);
        b();
        cout << prd_string((*player).vecStr[1], 22);
        b();
        cout << prd((*player).vecFlt[1], 6, 10);
        b();
        cout << prd((*player).vecInt[0], 0, 10);
        b();
        cout << "\n";
    }
    l(114);
    cout << tagsPlayerIDs.size() << " players found that contain all listed tags.\n\n";
}

void teamCommand(HashNode **tagsHashTable, int mTags, HashNode **playersHashTable, int mPlayers, vector<string> tags)
{
    HashNode *tag = search(tagsHashTable, tags.back(), mTags, 1);
    if (tag == NULL)
        return;
    int i, j, w, pos;
    vector<string> tagsPlayerIDs = (*tag).vecStr;
    vector<string> positionNames = {"CB", "RM", "CDM", "CF", "RWB", "ST", "RW", "CM", "GK", "LB", "LWB", "LM", "RB", "CAM", "LW"};

    int bestPlayersPos[15];
    double bestRating[15];
    for(i=0;i<15;i++)
    {
        bestPlayersPos[i] = -1;
        bestRating[i] = 0;
    }
        
    tags.pop_back();

    while (!tags.empty())
    {
        tag = search(tagsHashTable, tags.back(), mTags, 1);
        if (tag == NULL)
            return;
        vector<string> currentTagPlayerIDs = (*tag).vecStr;

        // todo: sort and then implement a better intersection algorithm
        int i = 0;
        while (i < tagsPlayerIDs.size())
        {
            int found = 0;
            for (int j = 0; j < currentTagPlayerIDs.size(); j++)
            {
                if (tagsPlayerIDs[i] == currentTagPlayerIDs[j])
                {
                    found = 1;
                    break;
                }
            }
            if (found)
                i++;
            else
                tagsPlayerIDs.erase(tagsPlayerIDs.begin() + i);
        }
        tags.pop_back();
    }

    for (i = 0; i < tagsPlayerIDs.size(); i++)
    {
        HashNode *player = search(playersHashTable, tagsPlayerIDs[i], mPlayers, 0);
        
        string positionName;
        string playerPositions = (*player).vecStr[1];
        j = 0;
        while (j < playerPositions.size())
        {
            if (playerPositions[j] != ',')
            {
                positionName.push_back(playerPositions[j]);
                if ((j + 1 == playerPositions.size()) || (playerPositions[j + 1] == ','))
                {
                    for(w = 0; w<15; w++)
                    {
                        if(positionName == positionNames[w])
                        {
                            if(bestPlayersPos[w] == -1) // Se a posição estiver vaga
                            {
                                //cout << "( " << (*player).vecStr[0] << " : " << positionName << " )";
                                bestPlayersPos[w] = i;
                                bestRating[w] = (*player).vecFlt[1];
                            }
                            else if ((*player).vecFlt[1] > bestRating[w] && (*player).vecInt[0] >= 100)
                            {
                                //cout << "( " << (*player).vecStr[0] << " : " << positionName << " )";
                                bestPlayersPos[w] = i;
                                bestRating[w] = (*player).vecFlt[1];
                            }
                        }
                    }
                    positionName = "";
                }
                j++;
            }
            else
                j += 2;
        }
    }        
    //CB RM CDM CF RWB ST RW CM GK LB LWB LM RB CAM LW
    l(114);
    b();
    cout << center("sofifa_id", 10);
    b();
    cout << center("name", 48);
    b();
    cout << center("position", 22);
    b();
    cout << center("rating", 10);
    b();
    cout << center("count", 10);
    b();
    cout << "\n";
    l(114);
    
    for (i = 0; i < 15; i++)
    {
        if(bestPlayersPos[i] != -1)
        {
            HashNode *player = search(playersHashTable, tagsPlayerIDs[bestPlayersPos[i]], mPlayers, 0);
            b();
            cout << center(tagsPlayerIDs[bestPlayersPos[i]], 10);
            b();
            cout << prd_string((*player).vecStr[0], 48);
            b();
            cout << prd_string(positionNames[i], 22);
            b();
            cout << prd((*player).vecFlt[1], 6, 10);
            b();
            cout << prd((*player).vecInt[0], 0, 10);
            b();
            cout << "\n";
        }
        
    }
    l(114);
}

void proccessPlayers(TrieNode *trieRoot, HashNode **playersHashTable, int mPlayers)
{
    for (int i = 0; i < mPlayers; i++)
        playersHashTable[i] = NULL;

    ifstream f("data/players.csv");
    aria::csv::CsvParser parser(f);
    parser.delimiter(',');
    for (auto &row : parser)
    {
        auto field = parser.next_field();
        string sofifa_id = *field.data;

        field = parser.next_field();
        string name = *field.data;

        field = parser.next_field();
        string player_positions = *field.data;

        vector<string> playerStrFields = {name, player_positions}; // [name, player_positions]
        vector<int> playerIntFields = {0};                         // [rating_count]
        vector<float> playerFloatFields = {0, 0};                  // [rating_sum, global_rating]
        insert(playersHashTable, mPlayers, 0, sofifa_id, playerStrFields, playerIntFields, playerFloatFields);
        put(trieRoot, name, sofifa_id, 0);
    }
}

void proccessUsers(HashNode **usersHashTable, int mUsers, HashNode **playersHashTable, int mPlayers)
{
    int userCount = 0;
    for (int i = 0; i < mUsers; i++)
        usersHashTable[i] = NULL;

    ifstream f2("data/rating.csv");
    aria::csv::CsvParser parser2(f2);
    parser2.delimiter(',');
    int count = 0;
    cout << "- Reading ratings"
         << "\n";
    for (auto &row : parser2)
    {
        auto field = parser2.next_field();
        string user_id = *field.data;

        field = parser2.next_field();
        string sofifa_id = *field.data;

        field = parser2.next_field();
        float rating = stof(*field.data);

        if ((count % 1000000 == 0) && (count != 0))
            cout << "-- Ratings count=" << count << " | time=" << totalTimeInSeconds() << "\n";
        count++;

        HashNode *player = search(playersHashTable, sofifa_id, mPlayers, 0);
        HashNode *user = search(usersHashTable, user_id, mUsers, 0);

        if (user == NULL)
        {
            vector<string> usersStrFields = {sofifa_id}; // [sofifa_id1, sofifa_id2, sofifa_id3 .... ] *linked with rating
            vector<int> usersIntFields;
            vector<float> usersFloatFields = {rating}; // [rating1, rating2, rating3 .... ] *ordered by rating
            insert(usersHashTable, mUsers, 0, user_id, usersStrFields, usersIntFields, usersFloatFields);
            userCount++;
        }
        else
        {
            int pos = 0;

            int vecSize = (*user).vecFlt.size();
            if (vecSize < 20 || (*user).vecFlt[vecSize - 1] < rating)
            {

                while (pos < (*user).vecFlt.size() && (*user).vecFlt[pos] > rating)
                    pos++;

                if (pos == (*user).vecFlt.size())
                {
                    (*user).vecFlt.push_back(rating);
                    (*user).vecStr.push_back(sofifa_id);
                }
                else
                {
                    (*user).vecFlt.insert((*user).vecFlt.begin() + pos, rating);
                    (*user).vecStr.insert((*user).vecStr.begin() + pos, sofifa_id);
                }
                if (vecSize >= 20)
                {
                    (*user).vecFlt.pop_back();
                    (*user).vecStr.pop_back();
                }
            }
        }

        (*player).vecInt[0] += 1;
        (*player).vecFlt[0] += rating;
    }
}

void proccessPositions(HashNode **positionsHashTable, int mPositions, HashNode **playersHashTable, int mPlayers)
{
    for (int i = 0; i < mPositions; i++)
        positionsHashTable[i] = NULL;

    for (int i = 0; i < mPlayers; i++)
    {
        if (playersHashTable[i] != NULL)
        {
            HashNode *player = playersHashTable[i];
            while (player != NULL)
            {
                if ((*player).vecInt[0] != 0)
                    (*player).vecFlt[1] = (*player).vecFlt[0] / (float)((*player).vecInt[0]);

                int minRatings = 1000;
                if ((*player).vecInt[0] >= minRatings)
                {
                    float currentPlayerRating = (*player).vecFlt[1];
                    string currentPlayerSofifa_id = (*player).c;
                    int strSize = (*player).vecStr[1].size();
                    string playerPositions = (*player).vecStr[1];
                    string positionName;
                    int j = 0;
                    while (j < playerPositions.size())
                    {
                        if (playerPositions[j] != ',')
                        {
                            positionName.push_back(playerPositions[j]);
                            if ((j + 1 == strSize) || (playerPositions[j + 1] == ','))
                            {
                                HashNode *position = search(positionsHashTable, positionName, mPositions, 1);
                                if (position == NULL)
                                {
                                    vector<string> positionsStrFields = {currentPlayerSofifa_id}; // [sofifa_id1, sofifa_id2, sofifa_id3 .... ]
                                    vector<int> positionsIntFields;
                                    vector<float> positionsFloatFields = {currentPlayerRating}; // [global_rating1, global_rating2, global_rating3 .... ]
                                    insert(positionsHashTable, mPositions, 1, positionName, positionsStrFields, positionsIntFields, positionsFloatFields);
                                }
                                else
                                {
                                    int pos = 0;
                                    while (pos < (*position).vecFlt.size() && (*position).vecFlt[pos] > currentPlayerRating)
                                        pos++;

                                    if (pos == (*position).vecFlt.size())
                                    {
                                        (*position).vecFlt.push_back(currentPlayerRating);
                                        (*position).vecStr.push_back(currentPlayerSofifa_id);
                                    }
                                    else
                                    {
                                        (*position).vecFlt.insert((*position).vecFlt.begin() + pos, currentPlayerRating);
                                        (*position).vecStr.insert((*position).vecStr.begin() + pos, currentPlayerSofifa_id);
                                    }
                                }
                                positionName = "";
                            }
                            j++;
                        }
                        else
                            j += 2;
                    }
                }
                player = (*player).next;
            }
        }
    }
}

void proccessTags(HashNode **tagsHashTable, int mTags)
{
    for (int i = 0; i < mTags; i++)
        tagsHashTable[i] = NULL;

    ifstream f3("data/tags.csv");

    aria::csv::CsvParser parser3(f3);
    parser3.delimiter(',');
    for (auto &row : parser3)
    {
        auto field = parser3.next_field();
        string user_id = *field.data;

        field = parser3.next_field();
        string sofifa_id = *field.data;

        field = parser3.next_field();
        string tagName = *field.data;

        HashNode *tag = search(tagsHashTable, tagName, mTags, 1);
        if (tag == NULL)
        {
            vector<string> tagsStrFields = {sofifa_id}; // [sofifa_id1, sofifa_id2, sofifa_id3 .... ]
            vector<int> tagsIntFields;
            vector<float> tagsFloatFields;
            insert(tagsHashTable, mTags, 1, tagName, tagsStrFields, tagsIntFields, tagsFloatFields);
        }
        else
        {
            int found = 0;
            for (int i = 0; i < (*tag).vecStr.size(); i++)
                if ((*tag).vecStr[i] == sofifa_id)
                    found = 1;
            if (!found)
                (*tag).vecStr.push_back(sofifa_id);
        }
    }
}

void proccessData(TrieNode *trieRoot, HashNode **playersHashTable, int mPlayers, HashNode **usersHashTable, int mUsers, HashNode **positionsHashTable, int mPositions, HashNode **tagsHashTable, int mTags)
{
    cout << "------- Proccess data -------\n\n";

    colisionsCount = 0;
    cout << "Proccess players\n";
    proccessPlayers(trieRoot, playersHashTable, mPlayers);
    cout << "playersHashTable | m=" << mPlayers << " | colisions=" << colisionsCount << " | time=" << totalTimeInSeconds() << "\n\n";

    colisionsCount = 0;
    cout << "Proccess users\n";
    proccessUsers(usersHashTable, mUsers, playersHashTable, mPlayers);
    cout << "usersHashTable | m=" << mUsers << " | colisions=" << colisionsCount << " | time=" << totalTimeInSeconds() << "\n\n";

    colisionsCount = 0;
    cout << "Proccess positions\n";
    proccessPositions(positionsHashTable, mPositions, playersHashTable, mPlayers);
    cout << "positionsHashTable | m=" << mPositions << " | colisions=" << colisionsCount << " | time=" << totalTimeInSeconds() << "\n\n";

    colisionsCount = 0;
    cout << "Proccess tags\n";
    proccessTags(tagsHashTable, mTags);
    cout << "tagsHashTable | m=" << mTags << " | colisions=" << colisionsCount << " | time=" << totalTimeInSeconds() << "\n\n";

    cout << "Data proccess time: " << totalTimeInSeconds() << "\n\n";
    cout << "-----------------------------\n\n";
}

bool isNumber(const string &str)
{
    for (char const &c : str)
    {
        if (std::isdigit(c) == 0)
            return false;
    }
    return true;
}

void console(TrieNode *trieRoot, HashNode **playersHashTable, int mPlayers, HashNode **usersHashTable, int mUsers, HashNode **positionsHashTable, int mPositions, HashNode **tagsHashTable, int mTags)
{
    string top = "top";
    string commandN;

    vector<string> tags;
    int i, tamanho = 0;
    while (true)
    {
        cout << "$ ";

        string command;
        getline(cin, command);

        int end = command.find(' ');

        if (end == -1)
        {
            if (command == "exit\0")
            {
                return;
            }
            if (command == "help\0")
            {
                SetConsoleTextAttribute(h, 11);
                cout << "\nCommands:\n";
                cout << "player prefix = Returns data of all players with that prefix.\n";
                cout << "user X = Returns data of the 20 players best rated by user X.\n";
                cout << "topX 'position' = Returns data of the X best players that play in the 'position'.\n";
                cout << "tags 'tag1' 'tag2' ... = Returns data of all players that have the listed tags. \n";
                cout << "team 'tag' tag2' ... = Returns a list of the best players for position that have the listed tags. \n";
                cout << "exit = Ends the program.\n";
                cout << "help = Shows instructions for all commands.\n\n";
                SetConsoleTextAttribute(h, 7);
            }
            else
            {
                SetConsoleTextAttribute(h, 12);
                cout << "Your command: '" << command << "' is missing arguments/incorrect\n";
                SetConsoleTextAttribute(h, 7);
            }   
            continue;
        }

        string command1 = command.substr(0, end);
        string command2 = command.substr(end + 1, command.length());

        if (command1 == "player\0")
        {
            playerCommand(trieRoot, playersHashTable, mPlayers, command2);
            continue;
        }
        if (command1 == "user\0")
        {
            if (isNumber(command2))
            {
                userCommand(usersHashTable, mUsers, playersHashTable, mPlayers, command2);
            }
            else
            {
                SetConsoleTextAttribute(h, 12);
                cout << "user id's are composed only by numbers\n";
                SetConsoleTextAttribute(h, 7);
            }
            continue;
        }

        if (command1 == "tags\0" || command1 == "team\0")
        {
            tags.clear();
            while (end != -1 && command2.at(0) == '\'')
            {
                end = command2.find(" '"); //Procura o fim da palavra mais a esquerda

                commandN = command2.substr(0, end);                    //Salva a palavra mais a esquerda
                command2 = command2.substr(end + 1, command.length()); //Separa o resto da frase da primeira palavra

                tamanho = commandN.length();
                if (commandN.at(0) == '\'' && commandN.at(tamanho - 1) == '\'' && tamanho > 2)
                {
                    commandN = commandN.substr(1, commandN.length() - 2);
                    tags.push_back(commandN); // Coloca a palavra mais a esquerda no fim da lista
                }
            }
            if (!(tags.empty()))
            {
                if (command1 == "tags\0")
                    tagCommand(tagsHashTable, mTags, playersHashTable, mPlayers, tags);
                else
                    teamCommand(tagsHashTable, mTags, playersHashTable, mPlayers, tags);
            }
            else
            {
                SetConsoleTextAttribute(h, 12);
                cout << "Your 2nd argument is missing the necessary apostrophes -> '' <- around it\n";
                SetConsoleTextAttribute(h, 7);
            }
                
            continue;
        }

        if (command1.length() > 3)
        {
            for (i = 0; i < 3; i++)
            {
                if (command1.at(i) != top.at(i))
                    break;
            }
            commandN = command1.substr(3, command1.length() - 3);
            if (i == 3 && (isNumber(commandN)))
            {

                if (command2.at(0) == '\'' && command2.at(command2.length() - 1) == '\'')
                {
                    command2 = command2.substr(1, command2.length() - 2);
                    topCommand(positionsHashTable, mPositions, playersHashTable, mPlayers, command2, stoi(commandN));
                }
                else
                {
                    SetConsoleTextAttribute(h, 12);
                    cout << "Your 2nd argument is missing the necessary apostrophes -> '' <- around it\n";
                    SetConsoleTextAttribute(h, 7);
                }
                    
                continue;
            }
        }
        SetConsoleTextAttribute(h, 12);
        cout << "The command '" << command << "' has no designed function.\n\0";
        SetConsoleTextAttribute(h, 7);
    };
}

int main()
{
    startTime = chrono::steady_clock::now();

    int mPlayers = 70000;
    HashNode **playersHashTable = new HashNode *[mPlayers]();
    TrieNode *trieRoot = new TrieNode();

    int mUsers = 200000;
    HashNode **usersHashTable = new HashNode *[mUsers]();

    int mPositions = 20;
    HashNode **positionsHashTable = new HashNode *[mPositions]();

    int mTags = 5000;
    HashNode **tagsHashTable = new HashNode *[mTags]();

    proccessData(trieRoot, playersHashTable, mPlayers, usersHashTable, mUsers, positionsHashTable, mPositions, tagsHashTable, mTags);
    console(trieRoot, playersHashTable, mPlayers, usersHashTable, mUsers, positionsHashTable, mPositions, tagsHashTable, mTags);
    return 0;
}