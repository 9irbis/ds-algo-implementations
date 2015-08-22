// Implementation of the in-place heap sort algorithm in an array

#include <iostream>
#include <vector>
#include <utility>
using namespace std;

// this algorithm has two phases. In the first phase, we move from left to right
// along the array and move an imaginary boundary between the heap and the array.
// in any ith iteration, elements 0, 1, 2, ... i-1 are in the heap while i+1, i+2,
// .... n-1 are in the array. In every iteration i, we add the (i-1)th element
// to the heap and move it upwards until the heap order property is satisfied.

// In phase two, we move from right to left and move the imaginary boundary
// along. In any ith iteration, we swap the values at root(0) and (n-i)th index
// thereby storing the highest element at (n-i)th position in its correct place.
//  the new root is then moved downwards until the heap order property is
//  satisfied.
void inplaceheapSort(vector<int>& nums)
{
    // phase one
    for(int i=2; i<=nums.size(); i++)
    {
        int v = i-1;
        while(v != 0)
        {
            int u = (v-1)/2;
            if (nums[u] >= nums[v]) 
                break;
            else
            {
                swap(nums[u],nums[v]);
                v = u;
            }
        }
    }
    
    // phase two
    for(int i=1; i<nums.size(); i++)
    {
        swap(nums[0], nums[nums.size()-i]);
        int u = 0;
        while (2*u+1 < nums.size()-i)
        {
            int v = 2*u+1;
            if (2*u+2 < nums.size()-i && nums[2*u+2] > nums[v])
                v = 2*u+2;
            if (nums[u] >= nums[v])
                break;
            else
            {
                swap(nums[u], nums[v]);
                u = v;
            }
        }
    }

}

int main()
{
    vector<int> nums;
    int x;
    cout << "Enter the numbers in your array: ";
    while (cin >> x)
        nums.push_back(x);
    cout << "\nBefore sorting:\n";
    for(auto& a:nums)
        cout << a << " ";
    cout << "\n";
    inplaceheapSort(nums);
    cout << "After sorting:\n";
    for(auto& a:nums)
        cout << a << " ";
    cout << "\n";
    return 0;
}
