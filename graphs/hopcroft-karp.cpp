/* Hopcroft-Karp Algorithm
A robust algorithm designed to solve the Maximum Cardinality Bipartite Matching (MCBM) problem.
It augments along a maximal set of shortest augmenting paths in each phase to ensure O(sqrt V) phases.

Implementation:
- I developed a struct to ensure ease of integration.
- Utilizes 1-based indexing and a dummy node (index 0) strategy to simplify things when searching for
  an augmenting path.

Pros:
- Significantly outperforms the standard Kuhn's algorithm.

Cons:
- Harder to implement since it needs both BFS (for layering) and DFS (for augmenting) in each phase.

Complexity Analysis:
- Time: O(E sqrt(V)).
- Space: O(E + V).
  Where:
    - V: Number of vertices.
    - E: Number of edges.
- Note: The formal proof of the O(E sqrt(V)) time complexity and a brief overview of the algorithm
        are available here: https://github.com/yusuf12360/complexity-analysis/blob/main/graphs/hopcroft-karp.md
*/

#include<bits/stdc++.h>
#define int long long
#define ld long double
#define pii pair<int, int>
#define vi vector<int>
#define vvi vector<vi> 
#define pb push_back
#define fi first
#define se second
#define TII tuple<int, int, int>
#define MT make_tuple
#define mp make_pair
#define ts to_string
#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()
#define MIN(x) *min_element(all(x))
#define MAX(x) *max_element(all(x))
#define lb lower_bound
#define ub upper_bound
#pragma GCC optimize("O3", "unroll-loops")
using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
const int INF = 1e10;

struct HopcroftKarp {
    int n, m;
    vi dist, match_right;
    vvi adj;
    vector<bool> used_left;

    void setup(int num_left, int num_right) {
        n = num_left;
        m = num_right;
        dist.resize(n + 1);
        adj.resize(n + 1);
        used_left.resize(n + 1);
        match_right.resize(m + 1);
    }

    void add_edge(int u, int v) {
        adj[u].pb(v);
    }

    bool bfs() {
        queue<int> q;
        dist[0] = INF;
        
        for(int i = 1; i <= n; i++) {
            if(used_left[i]) dist[i] = INF;
            else dist[i] = 0, q.push(i);
        }

        while(!q.empty()) {
            int u = q.front(); q.pop();
            if(dist[u] >= dist[0]) continue;
            
            for(int v : adj[u]) {
                if(dist[match_right[v]] > dist[u] + 1) {
                    dist[match_right[v]] = dist[u] + 1;
                    q.push(match_right[v]);
                }
            }
        }
        assert(dist[0] <= INF);
        return dist[0] != INF;
    }

    bool dfs(int u) {
        if(!u) return 1;
        
        for(int v : adj[u]) {
            if(dist[match_right[v]] == dist[u] + 1) {
                if(dfs(match_right[v])) {
                    used_left[u] = 1;
                    match_right[v] = u;
                    return 1;
                }
            }
        }
        return 0;
    }

    int mcbm() {
        int matching_size = 0;
        
        while(bfs()) {
            for(int i = 1; i <= n; i++) {
                if(!used_left[i] && dfs(i)) {
                    matching_size++; 
                }
            }
        }
        return matching_size;
    }
};

/* HOW TO USE
1 - Bipartite graph is a graph where vertices can be divided into two disjoint sets (U and V)
    such that every edge connects a vertex in U to one in V.
2 - setup(n, m): Initialize with n nodes on the left and m nodes on the right.
3 - add_edge(u, v): Add an edge from node u on the left to node v on the right.
4 - mcbm(): Returns the maximum matching size for the bipartite graph.

Example usage:
- The code below solves the problem "MATCHING - Fast Maximum Matching" (SPOJ).
- Problem source: https://www.spoj.com/problems/MATCHING/
*/

HopcroftKarp H;
int32_t main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    int n, m, p; cin >> n >> m >> p;
    H.setup(n, m);

    while(p--) {
        int u, v; 
        cin >> u >> v;
        H.add_edge(u, v);
    }
    cout << H.mcbm() << '\n';
    return 0;
}