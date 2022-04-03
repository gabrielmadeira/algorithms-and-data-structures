#include <bits/stdc++.h>
#include <math.h>
#include <chrono>

using namespace std;

vector<int> ciuraSeq{1, 4, 10, 23, 57, 132, 301, 701, 1577, 3548, 7983, 17961, 40412, 90927, 204585, 460316, 1035711};
string seqNames[] = {"SHELL", "KNUTH", "CIURA"};

int getInitialHP(int n, int seq)
{
    if (seq == 0)
        return ceil(log(n) / log(2));

    if (seq == 1)
        return ceil(log(2 * n + 1) / log(3));

    if (seq == 2)
    {
        int hp = 0;
        while (ciuraSeq[hp] < n)
        {
            hp++;
        }
        return hp;
    }
}

int getHByHP(int hp, int seq)
{
    if (seq == 0)
        return pow(2, hp);
    if (seq == 1)
        return (pow(3, hp) - 1) / 2;
    if (seq == 2)
        return ciuraSeq[hp];
}

vector<int> insertionSort(vector<int> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        if ((i != 0) && (v[i] < v[i - 1]))
        {
            int j = i;
            while ((j != 0) && (v[j] < v[j - 1]))
            {
                swap(v[j], v[j - 1]);
                j--;
            }
        }
    }
    return v;
}

vector<int> shellSortStep(vector<int> v, int h)
{
    for (int i = 0; i < h; i++)
    {
        vector<int> subV;
        for (int j = i; j < v.size(); j += h)
            subV.push_back(v[j]);
        subV = insertionSort(subV);
        int k = 0;
        for (int j = i; j < v.size(); j += h)
        {
            v[j] = subV[k];
            k++;
        }
    }
    return v;
}

vector<int> shellSort(vector<int> v, int seq, int bEnablePrint)
{
    if (bEnablePrint)
    {
        for (int i = 0; i < v.size(); i++)
            cout << v[i] << " ";
        cout << " SEQ=" << seqNames[seq] << '\n';
    }

    int hp = getInitialHP(v.size(), seq) - 1;
    while (getHByHP(hp, seq) >= 1)
    {
        v = shellSortStep(v, getHByHP(hp, seq));

        if (bEnablePrint)
        {
            for (int i = 0; i < v.size(); i++)
                cout << v[i] << " ";
            cout << " INCR=" << getHByHP(hp, seq) << '\n';
        }

        hp--;
    }
    return v;
}

void program2()
{
    while (cin)
    {
        chrono::steady_clock::time_point begin;
        chrono::steady_clock::time_point end;
        float totalTimeInSeconds;

        int nQtd = 0;
        cin >> nQtd;
        if (!nQtd)
            break;
        vector<int> vec;
        for (int i = 0; i < nQtd; i++)
        {
            int nValue;
            cin >> nValue;
            vec.push_back(nValue);
        }

        begin = chrono::steady_clock::now();
        shellSort(vec, 0, 0);
        end = chrono::steady_clock::now();
        totalTimeInSeconds = float(chrono::duration_cast<chrono::microseconds>(end - begin).count() / float(1000000));
        cout << seqNames[0] << "," << nQtd << "," << totalTimeInSeconds << "\n";

        begin = chrono::steady_clock::now();
        shellSort(vec, 1, 0);
        end = chrono::steady_clock::now();
        totalTimeInSeconds = float(chrono::duration_cast<chrono::microseconds>(end - begin).count() / float(1000000));
        cout << seqNames[1] << "," << nQtd << "," << totalTimeInSeconds << "\n";

        begin = chrono::steady_clock::now();
        shellSort(vec, 2, 0);
        end = chrono::steady_clock::now();
        totalTimeInSeconds = float(chrono::duration_cast<chrono::microseconds>(end - begin).count() / float(1000000));
        cout << seqNames[2] << "," << nQtd << "," << totalTimeInSeconds << "\n";
    }
}

int main()
{

    // chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    program2();

    // chrono::steady_clock::time_point end = chrono::steady_clock::now();
    // float totalTimeInSeconds = float(chrono::duration_cast<chrono::microseconds>(end - begin).count()/float(1000000));
    // cout << "Elapsed time: " << totalTimeInSeconds << "s" << "\n";

    return 0;
}