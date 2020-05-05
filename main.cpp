#include <iostream>
#include <list>
#include <queue>
#include <string.h>
#include <climits>

#define X 9
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
    mat = new int*[V];
    for(int u = 0; u < V; u++)
        mat[u] = new int[V];
}

void process_input(){

    scanf( "%d %d", &avenues, &streets);
    scanf( "%d %d", &shops, &citizens);

    V = avenues * streets;
    vertices = new node[V];
    make_cap_mat();

    int i;
    for(i = 0; i < V; i++){
        vertices[i].id = i;
        vertices[i].adj = new int[4];
        vertices[i].closest_shop = new int[V-1];
        vertices[i].adj_counter = 0;
        vertices[i].isShop = false;
        vertices[i].isCitizen = false;
        vertices[i].visited = false;
    }

    int count, lat = 0;
    for(i = 0; i < V; i++){
        count = 0;
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
        if(i - avenues >= 0) {
            vertices[i].adj[count++] = i - avenues;
            mat[i][i - avenues] = 1;
        }
        if(i + avenues < V) {
            vertices[i].adj[count++] = i + avenues;
            mat[i][i + avenues] = 1;
        }
        vertices[i].adj_counter = count;
    }

    sh = new int[shops];
    cit = new int[citizens];

    int x, y, n;
    for(i = 0; i < shops; i++){
        scanf("%d %d", &x, &y);
        n = (y-1) * avenues + x - 1;
        sh[i] = n;
        vertices[n].isShop = true;
    }
    for(i = 0; i < citizens; i++){
        scanf("%d %d", &x, &y);
        n = (y-1) * avenues + x - 1;
        cit[i] = n;
        vertices[n].isCitizen = true;
    }
}


/* Returns true if there is a path from source 's' to sink 't' in
  residual graph. Also fills parent[] to store the path */
bool bfs(int s, int t, int parent[])
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
            if (!visited[v]  && !vertices[v].isCitizen&& !vertices[v].visited)
            {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
                if(!vertices[v].isShop)
                    vertices[v].visited = true;
            }
        }
    }

    // If we reached sink in BFS starting from source, then return
    // true, else false
    vertices[t].visited = visited[t];
    return visited[t];
}

// Returns tne maximum number of edge-disjoint paths from s to t.
// This function is copy of forFulkerson() discussed at http://goo.gl/wtQ4Ks
int findDisjointPaths(int s, int t)
{
    int u, v;

    // Create a residual graph and fill the residual graph with
    // given capacities in the original graph as residual capacities
    // in residual graph
    // residual capacity of edge from i to j (if there
    // is an edge. If rGraph[i][j] is 0, then there is not)
    /*int rGraph[X][X];
    for (u = 0; u < V; u++)
        for (v = 0; v < V; v++)
            rGraph[u][v] = mat[u][v];*/

    int parent[V];  // This array is filled by BFS and to store path

    int max_flow = 0;  // There is no flow initially

    // Augment the flow while tere is path from source to sink
    while (bfs(s, t, parent))
    {
        // Find minimum residual capacity of the edges along the
        // path filled by BFS. Or we can say find the maximum flow
        // through the path found.
        int path_flow = INT_MAX;

        for (v=t; v!=s; v=parent[v])
        {
            u = parent[v];
            path_flow = min(path_flow, mat[u][v]);
        }

        // update residual capacities of the edges and reverse edges
        // along the path
        /*for (v=t; v != s; v=parent[v])
        {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }*/

        // Add path flow to overall flow
        max_flow += path_flow;
    }

    // Return the overall flow (max_flow is equal to maximum
    // number of edge-disjoint paths)
    return max_flow;
}

void reset_visits(){
    for(int i = 0; i < V; i++)
        if(!vertices[i].isShop)
            vertices[i].visited = false;
}

int main() {
    process_input();

    int result = 0;
    for (int i = 0; i < citizens; i++) {
        for (int j = 0; j < shops; j++) {
            if(!vertices[sh[j]].visited) {
                result += findDisjointPaths(cit[i], sh[j]);
            }
        }
        reset_visits();
    }

    cout << result;

   /*cout << findDisjointPaths(3, 9);
    cout << findDisjointPaths(3, 10);
    cout << findDisjointPaths(3, 8);*/


    return 0;
}

/*for(int i = 0; i < citizens; i++)
     bfs(vertices[cit[i]], -1, 0);


for(int i = 0; i < citizens; i++){
    for(int j = 0; j < shops; j++){
        printf("%d - ", vertices[cit[i]].closest_shop[j]);
    }
    cout << cit[i] << "\n";
}*/
