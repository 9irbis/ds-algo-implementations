#include <iostream>
#include <list>
#include <vector>
using namespace std;

void merge(list<int>& s, list<int>& s1, list<int>& s2)
{
    typedef typename list<int>::iterator iter;
    iter p1 = s1.begin();
    iter p2 = s2.begin();
    while(p1 != s1.end() && p2 != s2.end())
    {
        if (*p1 < *p2)
            s.push_back(*p1++);
        else
            s.push_back(*p2++);
    }
    while(p1 != s1.end())
        s.push_back(*p1++);
    while(p2 != s2.end())
        s.push_back(*p2++);
}

void mergeSort(list<int>& s)
{
    int n = s.size();
    if (n <= 1)
        return;
    typedef typename list<int>::iterator iter;
    iter p = s.begin();
    list<int> s1, s2;
    for(int i=0; i<n/2; i++) 
        s1.push_back(*p++);
    for(int i=n/2; i<n; i++)
        s2.push_back(*p++);
    s.clear();
    mergeSort(s1);
    mergeSort(s2);
    merge(s, s1, s2);
}

int main()
{
    cout << "Enter numbers to be sorted:\n";
    list<int> nums;
    int x;
    while (cin >> x)
        nums.push_back(x);
    cout << "Your numbers after sorting:\n";
    mergeSort(nums);
    for(auto val : nums)
        cout << val << " ";
    cout << "\n";
    return 0;
}
