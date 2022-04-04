#include <bits/stdc++.h>
#include <math.h>
#include <chrono>

using namespace std;

vector<int> v;
int recs = 0;
int swaps = 0;

void printV()
{
    for (int g = 0; g < v.size(); g++)
        cout << v[g] << " ";
    cout << "\n";
}

int particiona_lomuto(int i, int f)
{
    int x = v[f];
    int a = i - 1;
    for (int b = i; b < f; b++)
    {
        if (v[b] <= x)
        {
            a++;
            swap(v[a], v[b]);
            // printV();
            swaps++;
        }
    }
    swap(v[a + 1], v[f]);
    // printV();
    swaps++;
    return (a + 1);
}

int particiona_hoare(int i, int f)
{
    int cp = v[i];
    int esq = 1;
    while (i < f)
    {
        if (esq)
        {
            if (cp >= v[f])
            {
                v[i] = v[f];
                // printV();
                swaps++;
                i++;
                esq = 0;
            }
            else
                f--;
        }
        else
        {
            if (cp < v[i])
            {
                v[f] = v[i];
                // printV();
                swaps++;
                f--;
                esq = 1;
            }
            else
                i++;
        }
    }
    int k = i;
    v[k] = cp;
    // printV();
    swaps++;
    return k;
}

int particione(int i, int f, int choosePartType, int partType)
{
    int r;
    if (choosePartType)
    {
        r = rand() % (f - (i + 1) + 1) + (i + 1);
    }
    else
    {
        int mid = floor((i + f) / 2);
        if ((v[i] >= v[mid] && v[f] <= v[mid]) || (v[f] >= v[mid] && v[i] <= v[mid]))
            r = mid;
        else if ((v[mid] >= v[i] && v[f] <= v[i]) || (v[f] >= v[i] && v[mid] <= v[i]))
            r = i;
        else
            r = f;
    }

    swaps++;
    if (partType)
    {
        swap(v[i], v[r]);
        return particiona_hoare(i, f);
    }
    swap(v[f], v[r]);
    return particiona_lomuto(i, f);
}

void quicksort(int i, int f, int choosePartType, int partType)
{
    recs++;
    if (f > i)
    {
        // cout << "--->> ";
        // printV();
        int p = particione(i, f, choosePartType, partType);
        // cout << p << "-" << v[p] << "\n";
        // printV();
        // cout << "-----\n";

        // cout << "---\n";
        // for (int g = 0; g < v.size(); g++)
        //     cout << v[g] << " ";
        // cout << "\n";

        quicksort(i, p - 1, choosePartType, partType);
        quicksort(p + 1, f, choosePartType, partType);
    }
}

void quicksort_start(int i, int f, int choosePartType, int partType)
{
    recs = 0;
    swaps = 0;

    chrono::steady_clock::time_point begin;
    chrono::steady_clock::time_point end;
    float totalTimeInSeconds;
    begin = chrono::steady_clock::now();
    quicksort(i, f, choosePartType, partType);
    end = chrono::steady_clock::now();
    totalTimeInSeconds = float(chrono::duration_cast<chrono::nanoseconds>(end - begin).count() / float(1000000000));

    cout << "TAMANHO ENTRADA " << v.size() << "\n";
    cout << "SWAPS " << swaps << "\n";
    cout << "RECURSOES " << recs << "\n";
    cout << "TEMPO " << fixed << totalTimeInSeconds << "\n";
}

void program1(int choosePartType, int partType)
{
    while (cin)
    {
        int nQtd = 0;
        cin >> nQtd;
        if (!nQtd)
            break;
        v = {};
        for (int i = 0; i < nQtd; i++)
        {
            int nValue;
            cin >> nValue;
            v.push_back(nValue);
        }

        quicksort_start(0, v.size() - 1, choosePartType, partType);
    }
}

int main(int argc, char **argv)
{
    srand(time(0));

    int choosePartType = (int)argv[1][0] - 48;
    int partType = (int)argv[2][0] - 48;
    program1(choosePartType, partType);

    return 0;
}