#include <iostream>
#include <list>
#include <queue>
#include <string.h>
#include <climits>

#define X 25
using namespace std;

typedef struct Node{
    int id, adj_counter, pi;
    bool isShop, isCitizen, visited;
    int *closest_shop, *adj;
} node;

int shops, citizens, avenues, streets, V;
int *cit;
int *sh;
int **mat;
node *vertices;

void make_cap_mat(){
    V = avenues * streets + 2;

    vertices = new node[V];

    mat = new int*[V];
    for(int u = 0; u < V; u++)
        mat[u] = new int[V-2];

    int i;
    for(i = 0; i < V; i++){
        vertices[i].id = i;
        vertices[i].adj = new int[4];
        vertices[i].adj_counter = 0;
        vertices[i].isShop = false;
        vertices[i].isCitizen = false;
        vertices[i].visited = false;
    }

    int count, lat = 1;
    for(i = 1; i < V - 1; i++){
        count = 1;
        if(i >= lat + avenues)
            lat += avenues;

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

    for(i = 1; i < V; i++){
        if (vertices[i].isCitizen) {
            mat[0][i] = 1;
            vertices[0].adj[vertices[0].adj_counter++] = i;
        }
        if(vertices[i].isShop) {
            mat[V][i] = 1;
            vertices[V].adj[vertices[V].adj_counter++] = i;
        }
    }

}

void process_input(){

    scanf( "%d %d", &avenues, &streets);
    scanf( "%d %d", &shops, &citizens);

    make_cap_mat();

    sh = new int[shops];
    cit = new int[citizens];

    int x, y, n;
    for(int i = 0; i < shops; i++){
        scanf("%d %d", &x, &y);
        n = (y-1) * avenues + x - 1;
        sh[i] = n;
        vertices[n].isShop = true;
    }
    for(int i = 0; i < citizens; i++){
        scanf("%d %d", &x, &y);
        n = (y-1) * avenues + x - 1;
        cit[i] = n;
        vertices[n].isShop = true;
    }
}


/* Returns true if there is a path from source 's' to sink 't' in
  residual graph. Also fills parent[] to store the path */
bool bfs(int rGraph[X][X], int s, int t, int parent[])
{
    // Create a visited array and mark all vertices as not visited
    bool visited[V];
    memset(visited, 0, sizeof(visited));

    // Create a queue, enqueue source vertex and mark source vertex
    // as visited
    queue <int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;
    int v;

    // Standard BFS Loop
    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int i = 0; i < vertices[u].adj_counter; i++){
            v = vertices[u].adj[i];
            if (!visited[v]  && rGraph[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;

            }
        }
    }

    // If we reached sink in BFS starting from source, then return
    // true, else false
    return visited[t];
}

void print_mat(){
    int i,j;
    for(i = 0; i < V ; i++) {
        for (j = 0; j < V ; j++)
            cout << mat[i][j] << " - ";
        cout << "\n";
    }
}

int main() {
    process_input();



    return 0;
}
