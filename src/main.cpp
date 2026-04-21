#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n; vector<int> bit;
    Fenwick(int n=0): n(n), bit(n+1,0) {}
    void init(int n_) { n = n_; bit.assign(n+1,0); }
    void add(int idx, int delta){ for(; idx<=n; idx += idx & -idx) bit[idx] += delta; }
    int sumPrefix(int idx){ int s=0; for(; idx>0; idx -= idx & -idx) s += bit[idx]; return s; }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if(!(cin>>n)) return 0;
    vector<pair<long long,long long>> pts(n);
    for(int i=0;i<n;++i){ cin>>pts[i].first>>pts[i].second; }

    // Order by x to get x-index positions (1..n)
    vector<int> byx(n); iota(byx.begin(), byx.end(), 0);
    sort(byx.begin(), byx.end(), [&](int a,int b){
        if(pts[a].first!=pts[b].first) return pts[a].first<pts[b].first;
        return pts[a].second<pts[b].second;
    });
    vector<int> xpos(n);
    for(int i=0;i<n;++i) xpos[byx[i]] = i+1;

    // Build processing order by y increasing
    vector<int> ord(n); iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int a,int b){
        if(pts[a].second!=pts[b].second) return pts[a].second<pts[b].second;
        return pts[a].first<pts[b].first;
    });

    // Maintain a set of active x positions; for each point at x, count the number
    // of right-to-left maxima in the prefix [1..x-1] by scanning backwards over the
    // active positions using predecessor jumps (simulated via an ordered set).
    // To do this efficiently, we also maintain the y-rank at each position.

    vector<int> yrank_at(n+1, 0); // y-order index when inserted (1-based by y increasing)
    long long ans = 0;
    std::set<int> active; // ordered x positions present

    int yr = 0;
    for(int id : ord){
        ++yr;
        int x = xpos[id];
        // Count right-to-left maxima in [1..x-1]
        if(!active.empty()){
            auto it = active.lower_bound(x);
            if(it != active.begin()){
                // start from the immediate predecessor
                --it;
                int current_max = 0;
                while(true){
                    int pos = *it;
                    int vy = yrank_at[pos];
                    if(vy > current_max){
                        ++ans;
                        current_max = vy;
                    }
                    if(it == active.begin()) break;
                    --it;
                    if(*it >= x) break; // safety, though set ensures ordering
                }
            }
        }
        // insert current position
        active.insert(x);
        yrank_at[x] = yr;
    }

    cout << ans << '\n';
    return 0;
}
