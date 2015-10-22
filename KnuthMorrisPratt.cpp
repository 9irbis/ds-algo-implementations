#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<int> computeFailFunction(const string& pattern)
{
    int m = pattern.size();
    vector<int> fail(m);
    fail[0] = 0;
    int i = 1, j = 0;
    while(i < m)
    {
        if (pattern[i] == pattern[j])
        {
            fail[i] = j+1;
            i++;
            j++;
        }
        else if (j > 0)
            j = fail[j-1];
        else
        {
            fail[i] = 0;
            i++;
        }
    }
    return fail;
}

int kMPMatch(const string& text, const string& pattern)
{
    int n = text.size();
    int m = pattern.size();
    vector<int> fail = computeFailFunction(pattern);
    int i = 0, j = 0;
    while(i < n)
    {
        if(pattern[j] == text[i])
        {
            if (j == m-1)
                return i-m+1;
            i++;
            j++;
        }
        else if (j > 0)
            j = fail[j-1];
        else
            i++;
    }
    return -1;
}

int main()
{
    string text, pattern;
    cout << "Enter text string:\n";
    cin >> text;
    cout << "Enter pattern to compare:\n";
    cin >> pattern;
    int ans = kMPMatch(text, pattern);
    if (ans == -1)
        cout << "Pattern not found in text\n";
    else
        cout << "Pattern found starting at index " << ans << endl;
    return 0;
}
