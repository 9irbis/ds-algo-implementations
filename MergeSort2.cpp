// non recursive, faster merge sort for sorting array of elements

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void merge(vector<int>& in, vector<int>& out, int b, int m)
{
    int i = b;
    int j = b+m;
    int n = in.size();
    int e1 = min(b+m, n);
    int e2 = min(b+2*m, n);
    int k = b;
    while (i < e1 && j < e2)
    {
        if (in[i] < in[j])
            out[k++] = in[i++];
        else
            out[k++] = in[j++];
    }
    while(i < e1)
        out[k++] = in[i++];
    while(j < e2)
        out[k++] = in[j++];
}

void mergeSort(vector<int>& v)
{
    int n = v.size();
    vector<int> v2(n);
    vector<int>* in = &v;
    vector<int>* out = &v2;
    for(int m=1; m<n; m*=2)
    {
        for(int b=0; b<n; b+=2*m)
        {
            merge(*in, *out, b, m);
        }
        swap(in, out);
    }
}

int main()
{
    cout << "Enter numbers to be sorted\n";
    int x;
    vector<int > v;
    while (cin >> x)
        v.push_back(x);
    cout << "Your numbers after sorting:\n";
    mergeSort(v);
    for(auto val:v)
        cout << val << " ";
    cout << "\n";
    return 0;
}
