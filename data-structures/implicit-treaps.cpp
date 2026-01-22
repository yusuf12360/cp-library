/* TREAP (Tree + Heap)
A versatile data structure that combines Tree (specifically, BST) properties and Heap properties
to ensure an expected height of O(log N).

Implementation:
- The Treap variant used in this code is an Implicit Treap.
- I am capable of using pointers, but I prefer using primitive arrays because they are much faster, 
  with index 0 corresponding to a null pointer.
- Unfortunately, I didn't develop a struct for this code because the problem I'm trying 
  to solve has a strict time limit (0.6 seconds). My testing showed that using 
  a struct causes TLE, so I used global arrays to make the code faster.

Pros:
- Supports all standard Segment Tree operations (Range Query, Range Update, etc.).
- Supports dynamic array operations (Insert, Delete, Move, etc.) which Segment Trees can't do.

Cons:
- While the expected time complexity is O(log N) per operation, Treaps usually have a larger 
  constant factor in their execution time compared to Segment Trees, making them generally 
  slower in practice.

Complexity Analysis:
- Time: Expected O(N + (B + M) log A).
- Space: O(A).
  Where:
    - A: Maximum sequence length at any given time (500,000).
    - B: Total insertions across a single test case (4,000,000).
    - N: Length of initial sequence.
    - M: Number of queries.
    - The node-recycling pool "del" ensures space complexity stays O(A) 
      regardless of the total number of insertions (B).
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
const int N = 5e5 + 5, INF = 1e12;
int l[N], r[N], pri[N], sz[N], val[N], sum[N], pre_mx[N], suf_mx[N], all_mx[N];
int rev[N], sazy[N], same[N], idx = 1;
stack<int> del;
int dec(int x) {
    int ret = idx;
    if(!del.empty()) ret = del.top(), del.pop();
    else idx++;
    
    l[ret] = r[ret] = rev[ret] = sazy[ret] = 0;
    val[ret] = sum[ret] = all_mx[ret] = x;
    pre_mx[ret] = suf_mx[ret] = max(0ll, x);
    pri[ret] = rng();
    sz[ret] = 1;
    return ret;
}
void revv(int p) {
    if(p == 0) return;
    swap(l[p], r[p]);
    swap(pre_mx[p], suf_mx[p]);
    rev[p] ^= 1;
}
void all_same(int p, int x) {
    if(p == 0) return;
    val[p] = x;
    sum[p] = x * sz[p];
    pre_mx[p] = suf_mx[p] = max(0ll, sum[p]);
    all_mx[p] = max(x, sum[p]);
    same[p] = x;
    sazy[p] = 1;
}
void push(int p) {
    if(p == 0) return;
    if(rev[p]) {
        revv(l[p]);
        revv(r[p]);
        rev[p] = 0;
    }
    if(sazy[p]) {
        all_same(l[p], same[p]);
        all_same(r[p], same[p]);
        sazy[p] = 0;
    }
}
void pull(int p) {
    sz[p] = 1 + sz[l[p]] + sz[r[p]];
    sum[p] = val[p] + sum[l[p]] + sum[r[p]];
    pre_mx[p] = max({pre_mx[l[p]], sum[l[p]] + val[p] + pre_mx[r[p]]});
    suf_mx[p] = max({suf_mx[r[p]], sum[r[p]] + val[p] + suf_mx[l[p]]});
    all_mx[p] = max({all_mx[l[p]], all_mx[r[p]], suf_mx[l[p]] + val[p] + pre_mx[r[p]]});
}
void split(int p, int &lef, int &rig, int x) {
    if(p == 0) {lef = rig = 0; return;}
    push(p);
    if(x <= sz[l[p]]) split(l[p], lef, l[p], x), rig = p;
    else split(r[p], r[p], rig, x - sz[l[p]] - 1), lef = p;
    pull(p);
}
void merge(int &p, int lef, int rig) {
    push(lef); push(rig);
    if(!lef || !rig) {p = (lef ? lef : rig); return;}
    if(pri[lef] > pri[rig]) merge(r[lef], r[lef], rig), p = lef;
    else merge(l[rig], lef, l[rig]), p = rig;
    pull(p);
}
void del_all(int p) {
    if(p == 0) return;
    del.push(p);
    del_all(l[p]);
    del_all(r[p]);
}

/* HOW TO USE
1 - The arrays represent a forest (collection of trees). Treat every tree in this forest 
    as a dynamic array represented by a root index.
2 - Use dec(x) to obtain an index representing a new dynamic array with a single element of value x.
3 - Use del_all(p) to completely delete the dynamic array represented by root p.
4 - Lazy propagation:
        - Use revv(p) to reverse the order of the dynamic array represented by root p.
        - Use all_same(p, x) to assign the value x to every element in the dynamic array
          represented by root p.
5 - Manipulation:
        - Use split(p, lef, rig, x) to split the dynamic array represented by root p into two 
          dynamic arrays, with the left side containing x elements and its root assigned to lef, 
          and the right side containing the remaining elements with its root assigned to rig.
        - Use merge(p, lef, rig) to merge two dynamic arrays represented by roots lef and
          rig into a single dynamic array with its root assigned to p.

Example usage:
- The code below solves the problem "P2 - Maintaining a Sequence" (NOI China 2005).
- Problem source: https://dmoj.ca/problem/noi05p2
*/

int32_t main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    all_mx[0] = -INF;
    int n, m; cin >> n >> m;
    int root = 0;
    for(int i = 0; i < n; i++) {
        int in; cin >> in;
        merge(root, root, dec(in));
    }

    while(m--) {
        string type; cin >> type;
        if(type == "INSERT") {
            int pos, tot; cin >> pos >> tot;
            int rg;
            split(root, root, rg, pos);
            while(tot--) {
                int in; cin >> in;
                merge(root, root, dec(in));
            }
            merge(root, root, rg);
        } else if(type == "DELETE") {
            int pos, tot; cin >> pos >> tot;
            int cur, rg;
            split(root, root, cur, pos - 1);
            split(cur, cur, rg, tot);
            del_all(cur);
            merge(root, root, rg);
        } else if(type == "MAKE-SAME") {
            int pos, tot, in; cin >> pos >> tot >> in;
            int cur, rg;
            split(root, root, cur, pos - 1);
            split(cur, cur, rg, tot);
            all_same(cur, in);
            merge(root, root, cur);
            merge(root, root, rg);
        } else if(type == "REVERSE") {
            int pos, tot; cin >> pos >> tot;
            int cur, rg;
            split(root, root, cur, pos - 1);
            split(cur, cur, rg, tot);
            revv(cur);
            merge(root, root, cur);
            merge(root, root, rg);
        } else if(type == "GET-SUM") {
            int pos, tot; cin >> pos >> tot;
            int cur, rg;
            split(root, root, cur, pos - 1);
            split(cur, cur, rg, tot);
            cout << sum[cur] << '\n';
            merge(root, root, cur);
            merge(root, root, rg);
        } else cout << all_mx[root] << '\n';
    }
    return 0;
}