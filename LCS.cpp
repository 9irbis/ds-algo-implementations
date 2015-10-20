// Longest common subsequence problem solution in O(nm) time using dynamic
// programming

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main()
{
    string A, B;
    cin >> A >> B;
    int n = A.size();
    int m = B.size();
    int L[n+1][m+1];
    for(int i=0; i<=n; i++)
        L[i][0] = 0;
    for(int j=0; j<=m; j++)
        L[0][j] = 0;
    for(int i=1; i<=n; i++)
    {
        for(int j=1; j<=m; j++)
        {
            if (A[i-1] == B[j-1])
                L[i][j] = L[i-1][j-1] + 1;
            else
                L[i][j] = max(L[i-1][j], L[i][j-1]);
        }
    }
    int ans = L[n][m];
    cout << "Length of the longest common subsequence is: " << ans << "\n";
    string lcs;
    int i = n, j = m;
    while (i > 0 && j > 0)
    {
        if (A[i-1] == B[j-1])
        {
            lcs.push_back(A[i-1]);
            i--; j--;
        }
        else
        {
            if (L[i-1][j] > L[i][j-1])
                i--;
            else
                j--;
        }
    }
    cout << "And the subsequence is: ";
    for(auto it = lcs.rbegin(); it != lcs.rend(); it++)
        cout << *it;
    cout << endl;
    return 0;
}
