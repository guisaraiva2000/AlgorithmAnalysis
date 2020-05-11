#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cstring>

using namespace std;

int shops = 0, citizens = 0, avenues = 0, streets = 0, V = 0;
vector<int> *graph;

// Reads the input and converts grid to graph
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

// Deletes node by given its index
static bool deleteByIndex(int u, size_t inIndex ){
    if ( inIndex < graph[u].size() ){
        if ( inIndex != graph[u].size() - 1 )
            graph[u][inIndex] = graph[u].back();
        graph[u].pop_back();
        return true;
    }
    return false;
}

// Check if v is adjacent to u
bool isEdge(int u, int v){
    for (int & i : graph[u])
        if(i == v)
            return true;

    return false;
}

// Deletes element v from u's list
void deleteNode(int u, int v){
    for (unsigned int i = 0 ; i < graph[u].size(); i++)
        if(graph[u].at(i) == v) {
            deleteByIndex(u, i);
            return;
        }
}

/* Returns true if there is a path from source 's' to sink 't'
 * and fills pi[] to store the path */
bool bfs(int s, int t, int pi[]){
    bool visited[V];
    memset(visited, 0, sizeof(visited));
    queue <int> q;
    q.push(s);
    visited[s] = true;
    pi[s] = -1;

    while (!q.empty()){
        int u = q.front();
        q.pop();

        if(u == t) return true;

        for (int & v : graph[u]){
            if (!visited[v]) {
                pi[v] = u;
                visited[v] = true;
                q.push(v);
            }
        }
    }
    return visited[t];
}

/* Returns the maximum flow from s to t
 * which is the maximum uncrossed paths for
 * the citizens to get the store */
int edmondsKarp(int s, int t){
    int u, v;
    int pi[V];
    int maxFlow = 0;

    while (bfs(s, t, pi)){  // augmented path
        int pathFlow = INT_MAX;

        for (v=t; v!=s; v=pi[v]) {
            u = pi[v];
            if(isEdge(u , v))
                pathFlow = 1; // each edge capacity
            else
                pathFlow = 0;
        }

        for (v = t; v != s; v = pi[v]) {
            u = pi[v];
            deleteNode(u, v);
            graph[v].push_back(u);
        }
        maxFlow += pathFlow;
    }
    return maxFlow;
}

int main() {
    process_input();

    int s = 0, t = V - 1;
    cout << edmondsKarp(s, t) << endl;
    delete[] graph;

    return 0;
}
