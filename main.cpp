#include <bits/stdc++.h>
#include "parser.hpp"

using namespace std;

// -------------------- HASH --------------------

struct s_LinkedTree
{
    string info;
    struct s_LinkedTree *next;
};
typedef struct s_LinkedTree HashNode;

int hashFunc(string c, int m)
{
    int highChar = 0;
    for (int i = 0; i < c.length(); i++)
        if (c.at(i) > highChar)
            highChar = c.at(i);

    int p = highChar + 1;
    int hash = 0;
    for (int i = 0; i < c.length(); i++)
        hash = (p * hash + c.at(i)) % m;

    return hash;
}

HashNode *newNode(string c, HashNode *next)
{
    HashNode *newNode = new HashNode;
    newNode->info = c;
    newNode->next = next;
    return newNode;
}

void insert(HashNode *st[], string c, int m)
{
    int i = hashFunc(c, m);
    for (HashNode *x = st[i]; x != NULL; x = (*x).next)
        if (c == (*x).info)
            return;
    st[i] = newNode(c, st[i]);
}

int search(HashNode *st[], string c, int m)
{
    int count = 0;
    int i = hashFunc(c, m);
    for (HashNode *x = st[i]; x != NULL; x = (*x).next)
    {
        count++;
        if (c == (*x).info)
            return count;
    }
    return count;
}

// -------------------- HASH --------------------

// -------------------- TRIE --------------------

struct s_TrieNode
{
    char c = 0;
    struct s_TrieNode *left = NULL;
    struct s_TrieNode *mid = NULL;
    struct s_TrieNode *right = NULL;
    int info = -1;
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
    cout << (*x).c << "-" << (*x).info << " ";
    if (c < (*x).c)
        return get((*x).left, key, d);
    if (c > (*x).c)
        return get((*x).right, key, d);
    if (d < key.size() - 1)
        return get((*x).mid, key, d + 1);

    cout << "\nfound: " << (*x).info << "\n";
    return x;
}

TrieNode *put(TrieNode *x, string key, int info, int d)
{
    char c = key.at(d);
    if (x == NULL)
    {
        x = new TrieNode();
        (*x).c = c;
    }
    if (c < (*x).c)
        (*x).left = put((*x).left, key, info, d);
    else if (c > (*x).c)
        (*x).right = put((*x).right, key, info, d);
    else if (d < key.size() - 1)
        (*x).mid = put((*x).mid, key, info, d + 1);
    else
        (*x).info = info;
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

    TrieNode *trieRoot = new TrieNode();

    put(trieRoot, "test", 44, 0);

    get(trieRoot, "test", 0);
    get(trieRoot, "tes", 0);
    put(trieRoot, "tes", 33, 0);
    get(trieRoot, "test", 0);
    get(trieRoot, "tes", 0);

    put(trieRoot, "gabriel", 55, 0);
    put(trieRoot, "abelha", 22, 0);

    get(trieRoot, "gabriel", 0);
    get(trieRoot, "test", 0);
    get(trieRoot, "tes", 0);
    get(trieRoot, "abel", 0);
    get(trieRoot, "abelha", 0);
    put(trieRoot, "abel", 11, 0);
    get(trieRoot, "abel", 0);
    get(trieRoot, "bolha", 0);
    put(trieRoot, "palhaco", 88, 0);
    get(trieRoot, "palhaco", 0);

    return 0;

    // int argc, char **argv
    // atoi(argv[1]);
    int m = 38000; // 2*19000
    HashNode *st[m];
    for (int i = 0; i < m; i++)
        st[i] = NULL;

    ifstream f("data/minirating.csv");
    aria::csv::CsvParser parser(f);
    parser.delimiter(',');
    for (auto &row : parser)
    {
        for (auto &field : row)
        {
            cout << field << " | ";
        }
        cout << endl;
    }
}