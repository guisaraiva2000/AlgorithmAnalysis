#include <iostream>
#include <list>
#include <queue>
#include <string.h>
#include <climits>

using namespace std;

typedef struct Node{
    int id, adj_counter, pi;
    bool isShop, isCitizen, visited;
    int *closest_shop, *adj;
} node;

int shops, citizens, avenues, streets, V;
int *cit;
int *sh;
int *parents;
node *vertices;


void process_input(){

    scanf( "%d %d", &avenues, &streets);
    scanf( "%d %d", &shops, &citizens);

    V = avenues * streets;
    vertices = new node[V];

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

        if(i + 1 < lat + avenues)
            vertices[i].adj[count++] = i + 1;
        if(i - 1 >= lat)
            vertices[i].adj[count++] = i - 1;
        if(i - avenues >= 0)
            vertices[i].adj[count++] = i - avenues;
        if(i + avenues < V)
            vertices[i].adj[count++] = i + avenues;
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
int bfs(node s, int t) {
    queue<int> q;
    vector<bool> used;
    vector<int> parent;
    used.reserve(V);
    parent.reserve(V);

    int n;

    q.push(s.id);
    used[s.id] = true;
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int i = 0; i < vertices[v].adj_counter; i++) {
            n = vertices[v].adj[i];
            if (!used[n]) {
                used[n] = true;
                q.push(n);
                parent[n] = v;
            }
        }
    }
    return used[t];
}


// Returns tne maximum number of edge-disjoint paths from s to t.
// This function is copy of forFulkerson() discussed at http://goo.gl/wtQ4Ks
int findDisjointPaths(node s, int t)
{
    int u, v;

    parents = new int[V];  // This array is filled by BFS and to store path

    int max_flow = 0;  // There is no flow initially

    // Augment the flow while tere is path from source to sink
    while (bfs(s, t))
    {
        // Find minimum residual capacity of the edges along the
        // path filled by BFS. Or we can say find the maximum flow
        // through the path found.
        int path_flow = INT_MAX;

        for (v = t; v != s.id; v = parents[v])
        {
            u = parents[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        // update residual capacities of the edges and reverse edges
        // along the path
        for (v=t; v != s.id; v=parent[v])
        {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }

        // Add path flow to overall flow
        max_flow += path_flow;
    }

    // Return the overall flow (max_flow is equal to maximum
    // number of edge-disjoint paths)
    return max_flow;
}

int main() {
    process_input();

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
