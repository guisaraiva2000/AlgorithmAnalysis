#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cstring>

using namespace std;

int shops = 0, citizens = 0, avenues = 0, streets = 0, V = 0;
vector<int> *graph;

void process_input(){

    cin >> avenues >> streets;
    cin >> shops >> citizens;

    V = avenues * streets * 2 + 2;

    graph = new vector<int>[V];

    int x, y, n;
    for(int i = 0; i < shops; i++){
        cin >> x >> y;
        n = (y-1) * avenues + x;
        graph[n * 2].push_back(V-1);
    }
    for(int i = 0; i < citizens; i++){
        cin >> x >> y;
        n = (y-1) * avenues + x;
        graph[0].push_back(n * 2 - 1);
    }

    int lat = 1;
    for(int i = 2; i < V - 1; i+=2){
        if(i / 2 >= lat + avenues)
            lat += avenues;

        if(i / 2 + 1 < lat + avenues)
            graph[i].push_back(i + 1);

        if(i / 2 - 1 >= lat)
            graph[i].push_back((i / 2 - 1) * 2 - 1);

        if(i / 2 - avenues >= 1)
            graph[i].push_back((i / 2 - avenues) * 2 - 1);

        if(i / 2  + avenues < V/2 - 1)
            graph[i].push_back((i / 2  + avenues) * 2 - 1);

        graph[i-1].push_back(i);
    }
}

static bool EraseFromUnorderedByIndex(int u, size_t inIndex ){
    if ( inIndex < graph[u].size() ){
        if ( inIndex != graph[u].size() - 1 )
            graph[u][inIndex] = graph[u].back();
        graph[u].pop_back();
        return true;
    }
    return false;
}

bool isEdge(int u, int v){
    for (int & i : graph[u])
        if(i == v)
            return true;

    return false;
}

void deleteNode(int u, int v){
    for (unsigned int i = 0 ; i < graph[u].size(); i++)
        if(graph[u].at(i) == v) {
            EraseFromUnorderedByIndex(u, i);
            return;
        }
}

bool bfs(int s, int t, int parent[]){
    bool visited[V];
    memset(visited, 0, sizeof(visited));
    queue <int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    while (!q.empty()){
        int u = q.front();
        q.pop();

        if(u == t) return true;

        for (int & v : graph[u]){
            if (!visited[v]) {
                parent[v] = u;
                visited[v] = true;
                q.push(v);
            }
        }
    }
    return visited[t];
}

int findDisjointPaths(int s, int t){
    int u, v;
    int parent[V];
    int max_flow = 0;

    while (bfs(s, t, parent)){
        int path_flow = INT_MAX;

        for (v=t; v!=s; v=parent[v]) {
            u = parent[v];
            if(isEdge(u , v))
                path_flow = 1;
            else
                path_flow = 0;
        }

        for (v=t; v != s; v=parent[v]) {
            u = parent[v];
            deleteNode(u, v);
            graph[v].push_back(u);
        }
        max_flow += path_flow;
    }
    return max_flow;
}

int main() {
    process_input();

    int s = 0, t = V - 1;
    cout << findDisjointPaths(s, t) << endl;
    delete[] graph;

    return 0;
}


