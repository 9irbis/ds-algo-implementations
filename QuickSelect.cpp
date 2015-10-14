// Randomized quick select algorithm for finding the kth smallest element in
// an unsorted collection of n elements in O(n) time.

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>
using namespace std;

int quickSelect(vector<int>& S, int k)
{
    if (S.size() == 1)
        return S[0];
    srand(time(NULL));
    int pivot = S[rand() % S.size()];
    vector<int> L, E, G;
    for(auto val : S)
    {
        if (val < pivot)
            L.push_back(val);
        else if (val == pivot)
            E.push_back(val);
        else
            G.push_back(val);
    }
    if (k <= L.size())
        return quickSelect(L, k);
    else if (k <= L.size() + E.size())
        return pivot;
    else
        return quickSelect(G, k-L.size()-E.size());
}

int main()
{
    int n, x, k;
    printf("Enter the size of your sequence:\n");
    cin >> n;
    vector<int> S;
    printf("Enter %d numbers separated by space\n", n);
    for(int i=0; i<n; i++)
    {
        cin >> x;
        S.push_back(x);
    }
    cout << "enter k (to find the kth smallest number):\n";
    cin >> k;
    cout << "Kth smallest no is " << quickSelect(S, k);
    cout << endl;
    return 0;
}
