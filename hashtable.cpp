// Lab4 - Classificação e Pesquisa de Dados (INF01124) - 2021/1
// Gabriel Madeira - 00322863

#include <bits/stdc++.h>
#include <math.h>

using namespace std;

struct s_LinkedTree
{
    string info;
    struct s_LinkedTree *next;
};
typedef struct s_LinkedTree Node;

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

Node *newNode(string c, Node *next)
{
    Node *newNode = new Node;
    newNode->info = c;
    newNode->next = next;
    return newNode;
}

void insert(Node *st[], string c, int m)
{
    int i = hashFunc(c, m);
    for (Node *x = st[i]; x != NULL; x = (*x).next)
        if (c == (*x).info)
            return;
    st[i] = newNode(c, st[i]);
}

int search(Node *st[], string c, int m)
{
    int count = 0;
    int i = hashFunc(c, m);
    for (Node *x = st[i]; x != NULL; x = (*x).next)
    {
        count++;
        if (c == (*x).info)
            return count;
    }
    return count;
}

int main(int argc, char **argv)
{

    int m = atoi(argv[1]);
    Node *st[m];
    for (int i = 0; i < m; i++)
    {
        st[i] = NULL;
    }

    char *insertInputFileName = argv[2];
    char *searchInputFileName = argv[3];

    ifstream insertInputFile, searchInputFile;
    insertInputFile.open(insertInputFileName);

    string firstName, lastName, fullName;

    while (insertInputFile >> firstName >> lastName)
    {
        fullName = firstName + " " + lastName;
        insert(st, fullName, m);
    }

    int searchNamesCount = 0;
    int searchSuccessNamesCount = 0;
    int comparationsSum = 0;
    int comparationsMax = 0;
    searchInputFile.open(searchInputFileName);

    while (searchInputFile >> firstName >> lastName)
    {
        searchNamesCount++;
        fullName = firstName + " " + lastName;
        int comparations = search(st, fullName, m);
        if (comparations != -1)
        {
            searchSuccessNamesCount++;
            if (comparations > comparationsMax)
                comparationsMax = comparations;
            comparationsSum += comparations;
        }
        cout << "NOME" << std::setfill('0') << std::setw(2) << searchNamesCount << " " << comparations << "\n";
    }
    float comparationsAverage = (float)comparationsSum / (float)searchSuccessNamesCount;
    cout << "MEDIA " << comparationsAverage << "\n";
    cout << "MAXIMO " << comparationsMax << "\n";

    return 0;
}