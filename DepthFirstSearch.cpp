#include <iostream>
#include <list>

using namespace std;

class Graph{
private:
    int V;
    list<int> *adj;
    void dfsUtil(int start, bool visited[]);

public:
    Graph(int n);
    void addEdge(int v, int w);
    void dfs();
};

Graph::Graph(int n)
{
    V = n;
    adj = new list<int>[V];
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
    adj[w].push_back(v);
}

void Graph::dfsUtil(int start, bool visited[])
{
    visited[start] = true;
    cout << start << " ";
    for(auto it=adj[start].begin(); it!=adj[start].end(); ++it)
    {
        if (!visited[*it])
            dfsUtil(*it, visited);
    }
}

void Graph::dfs()
{
    bool *visited = new bool[V];
    for(int i=0; i<V; i++)
        visited[i] = false;
    for(int i=0; i<V; i++)
        if(!visited[i])
            dfsUtil(i, visited);
}

int main()
{
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(1, 3);
    g.addEdge(0, 2);
    g.addEdge(0, 4);
    g.addEdge(2, 4);
    cout << "DFS traversal of Graph is:\n";
    g.dfs();
    cout << endl;
    return 0;
}

