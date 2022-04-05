#include <bits/stdc++.h>
#include <math.h>
#include <chrono>

using namespace std;

int charAt(string str, int d)
{
    if (d < str.size())
        return str.at(d);
    else
        return 0;
}

void MSDRadixSort(vector<string> &str, int lo, int hi, int d)
{
    if (hi <= lo)
        return;

    int count[256 + 2] = {0};

    unordered_map<int, string> temp;

    for (int i = lo; i <= hi; i++)
    {
        int c = charAt(str[i], d);
        count[c]++;
    }

    for (int r = 0; r < 256 + 1; r++)
        count[r + 1] += count[r];

    for (int i = hi; i >= lo; i--)
    {
        int c = charAt(str[i], d);
        temp[(count[c] - 1)] = str[i];
        count[c]--;
    }

    for (int i = lo; i <= hi; i++)
        str[i] = temp[i - lo];

    for (int r = 1; r < 256; r++)
        MSDRadixSort(str, lo + count[r], lo + count[r + 1] - 1, d + 1);
}

int main()
{
    vector<string> str;

    while (cin)
    {
        string word;
        cin >> word;
        if (word.size() >= 4)
            str.push_back(word);
    }

    MSDRadixSort(str, 0, str.size() - 1, 0);

    int wordCount = 0;
    int vSize = str.size() - 1;
    for (int i = 0; i <= vSize; i++)
    {
        if ((i != vSize) && (str[i] == str[i + 1]))
            wordCount++;
        else
        {
            wordCount++;
            cout << str[i] << " " << wordCount << "\n";
            wordCount = 0;
        }
    }

    return 0;
}