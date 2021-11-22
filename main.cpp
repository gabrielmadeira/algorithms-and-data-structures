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
        // cout << "\nnot found\n";
        return nodesWithPrefix;
    }
    char c = key.at(d);
    // cout << (*x).c << "-" << (*x).sofifa_id << " ";
    if (c < (*x).c)
        return getPrefix((*x).left, key, d);
    if (c > (*x).c)
        return getPrefix((*x).right, key, d);
    if (d < key.size() - 1)
        return getPrefix((*x).mid, key, d + 1);

    nodesWithPrefix = getPrefixAux((*x).mid);
    if ((*x).sofifa_id != "n")
        nodesWithPrefix.push_back((*x).sofifa_id);

    // cout << "\nfound: " << (*x).sofifa_id << "\n";
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
    cout << "\nsofifa_id | name | player_positions | rating | count\n";
    for (int i = 0; i < playersWithPrefix.size(); i++)
    {
        HashNode *player = search(playersHashTable, playersWithPrefix[i], mPlayers, 0);
        cout << playersWithPrefix[i] << " | " << (*player).vecStr[0] << " | " << (*player).vecStr[1] << " | " << (*player).vecFlt[1] << " | " << (*player).vecInt[0] << "\n";
    }
}

void userCommand(HashNode **usersHashTable, int mUsers, HashNode **playersHashTable, int mPlayers, string userID)
{
    HashNode *user = search(usersHashTable, userID, mUsers, 0);
    if (user == NULL)
        return;
    int i = 0;
    cout << "\nsofifa_id | name | global_rating | count | rating\n";
    while (i < 20 && i < (*user).vecFlt.size())
    {
        HashNode *player = search(playersHashTable, (*user).vecStr[i], mPlayers, 0);
        cout << (*user).vecStr[i] << " | " << (*player).vecStr[0] << " | " << (*player).vecFlt[1] << " | " << (*player).vecInt[0] << " | " << (*user).vecFlt[i] << "\n";
        i++;
    }
}

void topCommand(HashNode **positionsHashTable, int mPositions, HashNode **playersHashTable, int mPlayers, string positionName, int n)
{
    HashNode *position = search(positionsHashTable, positionName, mPositions, 1);
    if (position == NULL)
        return;
    int i = 0;
    cout << "\nsofifa_id | name | player_positions | rating | count\n";
    while (i < n && i < (*position).vecFlt.size())
    {
        HashNode *player = search(playersHashTable, (*position).vecStr[i], mPlayers, 0);
        cout << (*position).vecStr[i] << " | " << (*player).vecStr[0] << " | " << (*player).vecStr[1] << " | " << (*position).vecFlt[i] << " | " << (*player).vecInt[0] << "\n";
        i++;
    }
}

void tagCommand(HashNode **tagsHashTable, int mTags, HashNode **playersHashTable, int mPlayers, vector<string> tags)
{
    HashNode *tag = search(tagsHashTable, tags.back(), mTags, 1);
    if (tag == NULL)
        return;
    vector<string> tagsPlayerIDs = (*tag).vecStr;
    tags.pop_back();
    // cout << "{";
    // for (int j = 0; j < tagsPlayerIDs.size(); j++)
    //     cout << tagsPlayerIDs[j] << ", ";
    // cout << "}\n-------------------1-------------------------\n";

    while (!tags.empty())
    {
        tag = search(tagsHashTable, tags.back(), mTags, 1);
        if (tag == NULL)
            return;
        vector<string> currentTagPlayerIDs = (*tag).vecStr;

        // cout << "{";
        // for (int j = 0; j < currentTagPlayerIDs.size(); j++)
        //     cout << currentTagPlayerIDs[j] << ", ";
        // cout << "}\n-------------------2-------------------------\n";

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

    // for (int j = 0; j < tagsPlayerIDs.size(); j++)
    //     cout << tagsPlayerIDs[j] << " ";
    // cout << "\n-------------------3-------------------------\n";

    cout << "\nsofifa_id | player_positions | rating | count\n";
    for (int i = 0; i < tagsPlayerIDs.size(); i++)
    {
        HashNode *player = search(playersHashTable, tagsPlayerIDs[i], mPlayers, 0);
        cout << tagsPlayerIDs[i] << " | " << (*player).vecStr[0] << " | " << (*player).vecStr[1] << " | " << (*player).vecFlt[1] << " | " << (*player).vecInt[0] << "\n";
    }
}

void processPlayers(TrieNode *trieRoot, HashNode **playersHashTable, int mPlayers)
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

        vector<int> playerIntFields = {0}; // [rating_count]

        vector<float> playerFloatFields = {0, 0}; // [rating_sum, global_rating]

        insert(playersHashTable, mPlayers, 0, sofifa_id, playerStrFields, playerIntFields, playerFloatFields);

        put(trieRoot, name, sofifa_id, 0);
    }

    // cout << "Colisions count:" << colisionsCount << "\nColisions count 2:" << colisionsCount2 << "\n";

    // colisionsCount = 0;
    // colisionsCount2 = 0;
}

void processUsers(HashNode **usersHashTable, int mUsers, HashNode **playersHashTable, int mPlayers)
{
    int userCount = 0;
    for (int i = 0; i < mUsers; i++)
        usersHashTable[i] = NULL;

    ifstream f2("data/rating.csv");
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
}

void processPositions(HashNode **positionsHashTable, int mPositions, HashNode **playersHashTable, int mPlayers)
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

                int minRatings = 1000; // change to 1000 CHECK
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

void processTags(HashNode **tagsHashTable, int mTags)
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

        // if (sofifa_id == "202126")
        //     cout << "-------------" << tagName << "\n";

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
        // if ((count22 % 1000 == 0) || (count22 > 38000))
        //     cout << count22 << "\n";
        // count22++;
    }
}

void processData(TrieNode *trieRoot, HashNode **playersHashTable, int mPlayers, HashNode **usersHashTable, int mUsers, HashNode **positionsHashTable, int mPositions, HashNode **tagsHashTable, int mTags)
{
    processPlayers(trieRoot, playersHashTable, mPlayers);
    processUsers(usersHashTable, mUsers, playersHashTable, mPlayers);
    processPositions(positionsHashTable, mPositions, playersHashTable, mPlayers);
    processTags(tagsHashTable, mTags);

    return;
    // Statistics
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
    // cout << "User count: " << userCount << "\n";
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
            if (command == "exit\0") // novo comando para os dois pontos e conveniencia
            {
                return;
            }
            cout << "Your command: '" << command << "' is missing arguments/incorrect\n";
            continue;
        }

        string command1 = command.substr(0, end);
        string command2 = command.substr(end + 1, command.length());

        if (command1 == "player\0")
        {
            // command2 = player
            playerCommand(trieRoot, playersHashTable, mPlayers, command2); //WARNINGMUSTCHECK
            continue;
        }
        if (command1 == "user\0")
        {
            if (isNumber(command2))
            {
                // command2 = user id
                userCommand(usersHashTable, mUsers, playersHashTable, mPlayers, command2); //WARNINGMUSTCHECK
                //cout << "user: " << command2 << "\n";
            }
            else
                cout << "user id's are composed only by numbers\n";
            continue;
        }

        if (command1 == "tags\0")
        {
            tags.clear();
            while (end != -1 && command2.at(0) == '\'')
            {
                end = command2.find(" '"); //Procura o fim da palavra mais a esquerda

                command1 = command2.substr(0, end);                    //Salva a palavra mais a esquerda
                command2 = command2.substr(end + 1, command.length()); //Separa o resto da frase da primeira palavra

                tamanho = command1.length();
                if (command1.at(0) == '\'' && command1.at(tamanho - 1) == '\'' && tamanho > 2)
                {
                    command1 = command1.substr(1, command1.length() - 2);
                    tags.push_back(command1); // Coloca a palavra mais a esquerda no fim da lista
                }
            }
            if (!(tags.empty()))
                tagCommand(tagsHashTable, mTags, playersHashTable, mPlayers, tags);
            //for (int i = 0; i < tags.size(); i++)
            //{
            //    cout << i << stags[i] << "\n";
            //}
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
                    //cout << "command2: " << command2 << "\n";
                    // commandN = number of top positions required top5 = 5
                    // command2 = position
                    topCommand(positionsHashTable, mPositions, playersHashTable, mPlayers, command2, stoi(commandN));
                }
                else
                    cout << "Your 2nd argument is missing the necessary apostrophes -> '' <- around it\n";
                continue;
            }
        }
        cout << "The command '" << command << "' has no designed function.\n\0";
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

    int mPlayers = 70000; // 2*19000
    HashNode **playersHashTable = new HashNode *[mPlayers]();
    TrieNode *trieRoot = new TrieNode();

    int mUsers = 580000; // 2 * 140000
    HashNode **usersHashTable = new HashNode *[mUsers]();

    int mPositions = 200;
    HashNode **positionsHashTable = new HashNode *[mPositions]();

    int mTags = 5000;
    HashNode **tagsHashTable = new HashNode *[mTags]();

    processData(trieRoot, playersHashTable, mPlayers, usersHashTable, mUsers, positionsHashTable, mPositions, tagsHashTable, mTags);

    // for (int i = 0; i < mPositions; i++)
    // {
    //     if (positionsHashTable[i] != NULL)
    //         cout << (*positionsHashTable[i]).c << " ";
    // }
    // cout << "\n";

    // for (int i = 0; i < mTags; i++)
    // {
    //     if (tagsHashTable[i] != NULL)
    //         cout << "|" << (*tagsHashTable[i]).c << "| ";
    // }
    // cout << "\n";

    // playerCommand(trieRoot, playersHashTable, mPlayers, "Frant");
    // userCommand(usersHashTable, mUsers, playersHashTable, mPlayers, "88852");
    // topCommand(positionsHashTable, mPositions, playersHashTable, mPlayers, "GK", 5);
    // topCommand(positionsHashTable, mPositions, playersHashTable, mPlayers, "LW", 10);
    // vector<string> tagsTest = {"Clinical Finisher", "Team Player"};
    // tagCommand(tagsHashTable, mTags, playersHashTable, mPlayers, tagsTest);
    // tagsTest = {"Brazil", "Dribbler", "Playmaker", "Juventus"};
    // tagCommand(tagsHashTable, mTags, playersHashTable, mPlayers, tagsTest);

    // -----------
    end = chrono::steady_clock::now();
    totalTimeInSeconds = float(chrono::duration_cast<chrono::nanoseconds>(end - begin).count() / float(1000000000));
    cout << "Data process time: " << totalTimeInSeconds << "\n";
    // -----------

    console(trieRoot, playersHashTable, mPlayers, usersHashTable, mUsers, positionsHashTable, mPositions, tagsHashTable, mTags);
    return 0;
}