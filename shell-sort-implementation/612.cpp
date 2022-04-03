#include <bits/stdc++.h>

using namespace std;

int insertionSort(string v)
{
    int countSwaps = 0;
    for (int i = 0; i < v.size(); i++)
    {
        if ((i != 0) && (v[i] < v[i - 1]))
        {
            int j = i;
            while ((j != 0) && (v[j] < v[j - 1]))
            {
                countSwaps++;
                swap(v[j], v[j - 1]);
                j--;
            }
        }
    }
    return countSwaps;
}

vector<string> insertionSort2(vector<int> v, vector<string> vecStrings)
{
    for (int i = 0; i < v.size(); i++)
    {
        if ((i != 0) && (v[i] < v[i - 1]))
        {
            int j = i;
            while ((j != 0) && (v[j] < v[j - 1]))
            {
                swap(v[j], v[j - 1]);
                swap(vecStrings[j], vecStrings[j - 1]);
                j--;
            }
        }
    }
    return vecStrings;
}

int main()
{
    int nTests;
    cin >> nTests;

    for (int i = 0; i < nTests; i++)
    {
        int nStringSize, nStrings;
        cin >> nStringSize;
        cin >> nStrings;
        vector<string> vecStrings;
        vector<int> vecNSwaps(nStrings);
        for (int j = 0; j < nStrings; j++)
        {
            string currentString;
            cin >> currentString;
            vecStrings.push_back(currentString);
            vecNSwaps[j] = insertionSort(currentString);
        }
        vecStrings = insertionSort2(vecNSwaps, vecStrings);
        for (int j = 0; j < nStrings; j++)
            cout << vecStrings[j] << "\n";
        if (i != nTests - 1)
            cout << "\n";
    }
}