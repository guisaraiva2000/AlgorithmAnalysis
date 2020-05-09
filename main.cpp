#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

typedef struct Node{
    int adj_counter;
    bool isShop, isCitizen;
    int *adj;
} node;

int shops, citizens, avenues, streets, V;
int **mat;
node *vertices;

void make_cap_mat(){

    mat = new int*[V];
    for(int u = 0; u < V; u++)
        mat[u] = new int[V];

    int count, lat = 1;
    for(int i = 1; i < V - 1; i++){
        count = 0;
        if(i >= lat + avenues)
            lat += avenues;

        if (vertices[i].isShop) vertices[i].adj = new int[5];
        else vertices[i].adj = new int[4];

        if(i + 1 < lat + avenues) {
            vertices[i].adj[count++] = i + 1;
            mat[i][i + 1] = 1;
        }
        if(i - 1 >= lat) {
            vertices[i].adj[count++] = i - 1;
            mat[i][i - 1] = 1;
        }
        if(i - avenues >= 1) {
            vertices[i].adj[count++] = i - avenues;
            mat[i][i - avenues] = 1;
        }
        if(i + avenues < V - 1) {
            vertices[i].adj[count++] = i + avenues;
            mat[i][i + avenues] = 1;
        }
        vertices[i].adj_counter = count;
    }

    for(int i = 1; i < V-1; i++) {
        if (vertices[i].isCitizen) {
            mat[0][i] = 1;
            vertices[0].adj[vertices[0].adj_counter++] = i;
        }
        if(vertices[i].isShop) {
            mat[i][V - 1] = 1;
            vertices[i].adj[vertices[i].adj_counter++] = V - 1;
        }
    }
}

void process_input(){

    cin >> avenues >> streets;
    cin >> shops >> citizens;

    V = avenues * streets + 2;

    vertices = new node[V];

    for(int i = 0; i < V; i++){
        vertices[i].adj_counter = 0;
        vertices[i].isShop = false;
        vertices[i].isCitizen = false;
    }

    vertices[0].adj = new int[citizens];

    int x, y, n;
    for(int i = 0; i < shops; i++){
        cin >> x >> y;
        n = (y-1) * avenues + x;
        vertices[n].isShop = true;
    }
    for(int i = 0; i < citizens; i++){
        cin >> x >> y;
        n = (y-1) * avenues + x;
        vertices[n].isCitizen = true;
    }

    make_cap_mat();
}

bool bfs(int s, int t, int* parent){
    vector<bool> visited;
    visited.reserve(V);

    queue <int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;
    int v;

    while (!q.empty()){
        int u = q.front();
        q.pop();

        for (int i = 0; i < vertices[u].adj_counter; i++){
            v = vertices[u].adj[i];
            if (!visited[v] && mat[u][v] > 0) {
                parent[v] = u;
                visited[v] = true;
                if(v == u) return true;
                q.push(v);
            }
        }
    }
    return visited[t];
}

int findDisjointPaths(int s, int t){
    int u, v;
    int *parent = new int[V];
    int max_flow = 0;

    while (bfs(s, t, parent)){
        int path_flow = INT_MAX;

        for (v=t; v!=s; v=parent[v]) {
            u = parent[v];
            path_flow = min(path_flow, mat[u][v]);
        }

        for (v=t; v != s; v=parent[v]) {
            u = parent[v];
            mat[u][v] -= path_flow;
            mat[v][u] += path_flow;
        }
        max_flow += path_flow;
    }
    delete [] parent;
    return max_flow;
}

/*void print_mat(){
    int i,j;
    for(i = 0; i < V ; i++) {
        cout << "{";
        for (j = 0; j < V; j++)
            cout << mat[i][j] << ",";
        cout << "}," << endl;
    }
}*/

int main() {
    process_input();

    cout << findDisjointPaths(0, V-1) << endl;
    //print_mat();

    for(int u = 0; u < V; u++)
        delete[] mat[u];
    delete[] mat;

    for(int i = 0; i < V ; i++)
        delete[] vertices[i].adj;
    delete[] vertices;

    return 0;
}
 /*
           {{0,0,1,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0},
            {0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
            {0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
            {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};*/