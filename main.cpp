#include <bits/stdc++.h>
#include "lib/parser.hpp"
#include <chrono>

using namespace std;

int colisionsCount = 0;
int colisionsCount2 = 0;

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
        // int highChar = 0;
        // for (int i = 0; i < lengthC; i++)
        //     if (c.at(i) > highChar)
        //         highChar = c.at(i);
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
        colisionsCount2++;
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
        return get((*x).mid, key, d + 1);

    cout << "\nfound: " << (*x).sofifa_id << "\n";
    return x;
}

TrieNode *getPrefix(TrieNode *x, string key, int d) {} // todo

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

void console()
{
    while (true)
    {
        cout << "$ ";
        string command;
        cin >> command;

        if (command == "player")
        {
        }
        if (command == "user")
        {
        }
        if (command == "tags")
        {
        }
        if (command == "top")
        {
        } // todo: top<n> structure
    };
}

int main()
{

    // -----------
    chrono::steady_clock::time_point begin;
    chrono::steady_clock::time_point end;
    float totalTimeInSeconds;
    begin = chrono::steady_clock::now();
    // -----------

    TrieNode *trieRoot = new TrieNode();

    int mPlayers = 70000; // 2*19000
    // HashNode *playersHashTable[mPlayers];
    HashNode **playersHashTable = new HashNode *[mPlayers]();
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

        vector<int> playerIntFields = {0}; // [rating_count]

        vector<float> playerFloatFields = {0, 0}; // [rating_sum, global_rating]

        insert(playersHashTable, mPlayers, 0, sofifa_id, playerStrFields, playerIntFields, playerFloatFields);

        put(trieRoot, name, sofifa_id, 0);
    }

    cout << "Colisions count:" << colisionsCount << "\nColisions count 2:" << colisionsCount2 << "\n";

    colisionsCount = 0;
    colisionsCount2 = 0;

    int userCount = 0;
    int mUsers = 580000; // 2 * 140000

    HashNode **usersHashTable = new HashNode *[mUsers]();
    for (int i = 0; i < mUsers; i++)
        usersHashTable[i] = NULL;

    ifstream f2("data/minirating.csv");
    aria::csv::CsvParser parser2(f2);
    parser2.delimiter(',');
    int count = 0;
    for (auto &row : parser2)
    {
        auto field = parser2.next_field();
        string user_id = *field.data;

        field = parser2.next_field();
        string sofifa_id = *field.data;

        field = parser2.next_field();
        float rating = stof(*field.data);

        // if (count % 1000000 == 0)
        // {
        // int emptyPositions = 0;
        // int emptyPositionsParcial = 0;
        // for (int i = 0; i < mUsers; i++)
        // {
        //     if (usersHashTable[i] == NULL)
        //     {
        //         emptyPositions++;
        //         emptyPositionsParcial++;
        //     }
        //     if (i % 25000 == 0)
        //     {
        //         cout << i << ": " << emptyPositionsParcial << " ";
        //         emptyPositionsParcial = 0;
        //     }
        // }
        // cout << "\n";
        // cout << "Empty positions: " << emptyPositions << "\n";
        // cout << "User count: " << userCount << "\n";
        // cout << "Colisions count:" << colisionsCount << "\nColisions count 2:" << colisionsCount2 << "\n";
        // cout << count << "\n";
        // }
        // count++;

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
        }

        (*player).vecInt[0] += 1;
        (*player).vecFlt[0] += rating;

        // users[id]->[[ 41244, 4.6 ], [ 41244, 4.5 ], , [ 41244, 3.5 ]] "412312"->["23123", 4.5]
    }

    int mPositions = 200;
    HashNode **positionsHashTable = new HashNode *[mPositions]();
    for (int i = 0; i < mPositions; i++)
        positionsHashTable[i] = NULL;

    for (int i = 0; i < mPlayers; i++)
    {
        if (playersHashTable[i] != NULL)
        {
            HashNode *player = playersHashTable[i];
            (*player).vecFlt[1] = (*player).vecFlt[0] / (*player).vecInt[0];
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
                    if ((j + 1 == strSize - 1) || (playerPositions[j + 1] == ','))
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

            // while(player.next){
            //     player = player.next;
            //     (*player).vecFlt[1] = (*player).vecFlt[0]/(*player).vecInt[0];

            // }
        }
    }

    int mTags = 5000;
    HashNode **tagsHashTable = new HashNode *[mTags]();
    for (int i = 0; i < mTags; i++)
        tagsHashTable[i] = NULL;

    ifstream f3("data/tags.csv");

    aria::csv::CsvParser parser3(f3);
    parser3.delimiter(',');
    // int count22 = 0;
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
            (*tag).vecStr.push_back(sofifa_id);

        // if ((count22 % 1000 == 0) || (count22 > 38000))
        //     cout << count22 << "\n";
        // count22++;
    }

    // -----------
    end = chrono::steady_clock::now();
    totalTimeInSeconds = float(chrono::duration_cast<chrono::nanoseconds>(end - begin).count() / float(1000000000));
    cout << "Time: " << totalTimeInSeconds << "\n";
    // -----------

    HashNode *query = search(playersHashTable, "257697", mPlayers, 0);
    cout << "\nQuery result:\n"
         << (*query).vecStr[0] << "\n";

    cout << "\nTrie query result:\n";
    get(trieRoot, "Zitong Chen", 0);

    cout << "\nUser query result:\n";
    HashNode *userTest = search(usersHashTable, "118046", mUsers, 0);
    for (int i = 0; i < (*userTest).vecStr.size(); i++)
    {
        cout << (*userTest).vecStr[i] << "-";
        cout << (*userTest).vecFlt[i] << " ";
    }
    cout << "\n";

    cout << "Colisions count:" << colisionsCount << "\nColisions count 2:" << colisionsCount2 << "\n";

    int emptyPositions = 0;
    for (int i = 0; i < mUsers; i++)
    {
        if (usersHashTable[i] == NULL)
        {
            emptyPositions++;
        }
    }
    cout << "Empty positions: " << emptyPositions << "\n";
    cout << "User count: " << userCount << "\n";

    return 0;

    // TrieNode *trieRoot = new TrieNode();

    // put(trieRoot, "test", 44, 0);
    // get(trieRoot, "test", 0);

    // return 0;

    // int argc, char **argv
    // atoi(argv[1]);
    // int m = 38000; // 2*19000
    // HashNode *st[m];
    // for (int i = 0; i < m; i++)
    //     st[i] = NULL;

    // ifstream f("data/minirating.csv");
    // aria::csv::CsvParser parser(f);
    // parser.delimiter(',');
    // for (auto &row : parser)
    // {
    //     for (auto &field : row)
    //     {
    //         cout << field << " | ";
    //     }
    //     cout << endl;
    // }
}