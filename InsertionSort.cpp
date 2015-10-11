#include <iostream>
#include <vector>
using namespace std;

void insertionSort(vector<int>& v)
{
    for(int i=1; i<v.size(); i++)
    {
        int k = v[i];
        int j = i;
        while (j>0 && v[j-1]>k)
        {
            v[j] = v[j-1];
            j--;
        }
        v[j] = k;
    }
}

int main()
{
    vector<int> nums;
    cout << "Enter space separated numbers to be sorted:\n";
    int x;
    while (cin >> x)
        nums.push_back(x);
    cout << "after insertion sort:\n";
    insertionSort(nums);
    for(auto val : nums)
        cout << val << " ";
    cout << endl;
    return 0;
}
