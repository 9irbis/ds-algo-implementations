// Bubble sort algorithm implementation. runs in O(n^2) time.

#include <iostream>
#include <vector>
using namespace std;

void bubbleSort(vector<int>& v)
{
    int n = v.size();
    for(int i=0; i<n; i++)          // ith pass through the array
        for(int j=1; j<n-i; j++)    // comparision no. in current pass
        {
            if (v[j-1] > v[j])      // swap
            {
                int temp = v[j-1];
                v[j-1] = v[j];
                v[j] = temp;
            }
        }
}

int main()
{
    cout << "Enter the integers(space separated) you want sorted: ";
    vector<int> v;
    int x;
    while (cin >> x)
    {
        v.push_back(x);
    }
    cout << "Your integers after sorting: ";
    bubbleSort(v);
    for(auto e:v)
        cout << e << " ";
    cout << endl;
    return 0;
}
