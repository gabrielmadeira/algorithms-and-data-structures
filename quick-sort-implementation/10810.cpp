#include <bits/stdc++.h>
#include <math.h>
#include <chrono>

using namespace std;

vector<int> v;
long swaps = 0;

void mergeSort(int i, int f)
{
    if (i < f)
    {
        int mid = (i + f) / 2;

        mergeSort(i, mid);
        mergeSort(mid + 1, f);

        int len1 = mid - i + 1;
        int len2 = f - mid;

        int *left = (int *)malloc((len1 + 1) * sizeof(int));
        int *right = (int *)malloc((len2 + 1) * sizeof(int));
        int a, b;

        for (a = 1; a <= len1; a++)
        {
            left[a] = v[i + a - 1];
        }

        for (b = 1; b <= len2; b++)
        {
            right[b] = v[mid + b];
        }

        a = 1;
        b = 1;
        int k = i;

        while (a <= len1 && b <= len2)
        {
            if (left[a] <= right[b])
            {
                v[k] = left[a];
                a++;
            }
            else
            {
                v[k] = right[b];
                b++;
                swaps += len1 - a + 1;
            }

            k++;
        }

        while (a <= len1)
        {
            v[k++] = left[a++];
        }

        while (b <= len2)
        {
            v[k++] = right[b++];
        }
    }
}

void program1()
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

        swaps = 0;
        mergeSort(0, v.size() - 1);
        cout << swaps << "\n";
    }
}

int main()
{

    program1();

    return 0;
}