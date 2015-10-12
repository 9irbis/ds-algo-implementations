#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void quickSortStep(vector<int>& v, int a, int b)
{
    if (a >= b)
        return;
    int pivot = v[b];
    int l = a;
    int r = b-1;
    while(l <= r)
    {
        while(l <= r && v[l] <= pivot)
            l++;
        while(r >= l && v[r] >= pivot)
            r--;
        if(l < r)
            swap(v[l], v[r]);
    }
    swap(v[l], v[b]);
    quickSortStep(v, a, l-1);
    quickSortStep(v, l+1, b);
}

void quickSort(vector<int>& v)
{
    if (v.size() <= 1)
        return;
    else
        quickSortStep(v, 0, v.size()-1);
}

int main()
{
    cout << "Enter your sequence of nos to be sorted:\n";
    int x;
    vector<int> v;
    while(cin >> x)
        v.push_back(x);
    cout << "Your sequence after sorting:\n";
    quickSort(v);
    for(auto val:v)
        cout << val << " ";
    cout << endl;
}
