// Boyer Moore pattern matching algorithm
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

vector<int> buildLastFunction(const string& pattern)
{
    const int N_ASCII = 128;
    vector<int> last(N_ASCII);
    for(int i=0; i<N_ASCII; i++)
        last[i] = -1;
    for(int i=0; i<pattern.size(); i++)
        last[pattern[i]] = i;
    return last;
}

int bMMatch(const string& text, const string& pattern)
{
    vector<int> last = buildLastFunction(pattern);
    int n = text.size();
    int m = pattern.size();
    int i = m-1, j = m-1;
    if (i > n-1)
        return -1;
    do{
        if (text[i] == pattern[j])
        {
            if(j == 0)
                return i;
            else
            {
                i--;
                j--;
            }
        }
        else
        {
            i = i + m - min(j, 1 + last[text[i]]);
            j = m - 1;
        }
    }while(i<=n-1);
    return -1;
}

int main()
{
    string text, pattern;
    printf("Enter text string:\n");
    cin >> text;
    printf("Enter pattern to match:\n");
    cin >> pattern;
    int ans = bMMatch(text, pattern);
    if (ans == -1)
        printf("text doesn't contain pattern\n");
    else
        printf("pattern found in text starting at index %d\n", ans);
    return 0;
}
