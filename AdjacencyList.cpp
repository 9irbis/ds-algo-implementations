#include <iostream>
#include <vector>
#include <list>

using namespace std;

int main()
{
    int n, m;
    cout << "Enter number of vertices and edges separated by space: ";
    cin >> n >> m;
    vector<list<int>> graph(n+1);
    cout << "Enter pairs of end vertices of each edge on a new line:\n";
    for(int i=0; i<m; i++)
    {
        int startV, endV;
        cin >> startV >> endV;
        graph[startV].push_back(endV);
        graph[endV].push_back(startV);
    }
    cout << "Structure of our graph is:\n";
    for(int i=1; i<=n; i++)
    {
        cout << "Vertex " << i << " has neighbor(s):";
        for(auto e:graph[i])
            cout << " " << e;
        cout << endl;
    }
    return 0;
}

