#include <iostream>
#include <list>
#include <vector>
#include <queue>
using namespace std;

class Graph{
private:
    int V;
    vector<list<int>> adj;
    void bfsUtil(int start, vector<bool>& visited);
public:
    Graph(int n); 
    void addEdge(int v, int w);
    void bfs();
};

Graph::Graph(int n): V(n), adj(V) {}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
    adj[w].push_back(v);
}

void Graph::bfsUtil(int start, vector<bool>& visited)
{
    queue<int> Q;
    visited[start] = true;
    Q.push(start);
    while(!Q.empty())
    {
        int u = Q.front();
        cout << u << " ";
        Q.pop();
        for(auto it=adj[u].begin(); it!=adj[u].end(); ++it)
        {
            if (!visited[*it])
            {
                visited[*it] = true;
                Q.push(*it);
            }
        }
    }
}

void Graph::bfs()
{
    vector<bool> visited(V);
    for(int i=0; i<V; i++)
    {
        if (!visited[i])
            bfsUtil(i, visited);
    }
}

int main()
{
    Graph g(7);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 3);
    g.addEdge(1, 2);
    g.addEdge(3, 4);
    g.addEdge(5, 6);
    cout << "Breadth first traversal of G is :\n";
    g.bfs();
    cout << endl;
    return 0;
}
