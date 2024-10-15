#include <bits/stdc++.h>
using namespace std;

map<char, int> durations;
map<char, vector<char>> adj;
map<char, bool> vis;
int nodes_count = 0;

vector<char> ans;
void dfs(char node, char parent) {
    vis[node] = true;
    for (auto& child : adj[node]) 
        if (child != parent)
            dfs(child, node);
    ans.push_back(node);
}

void topological_sort() {
    for (auto& node : adj)
        if (!vis[node.first])
            dfs(node.first, '0');
    reverse(ans.begin(), ans.end());
}

map<char, pair<int, int>> times;
map<char, pair<int, int>> new_times;
vector<vector<char>> dfs2(char node, char parent) {
    vector<vector<char>> vec_ans;
    if (adj[node].size() == 0)
        vec_ans.push_back({});
    for (auto& child : adj[node])
        if (child != parent && times[child].first == new_times[child].first && times[child].second == new_times[child].second)
            for (auto& j : dfs2(child, node))
                vec_ans.push_back(j);
    for (auto& j : vec_ans)
        j.push_back(node);
    return vec_ans;
}

int main() {
    cout << "Enter the number of nodes: ";
    cin >> nodes_count;
    for (int node = 0; node < nodes_count; node++) {
        char node_char; cin >> node_char;
        cout << "Enter the number of dependencies: ";
        int number_of_dep; cin >> number_of_dep;
        while (number_of_dep--) {
            char dep; cin >> dep;
            adj[node_char].push_back(dep);
        }
        cout << "Enter the duration of the task: ";
        cin >> durations[node_char];
    }
    topological_sort();
    // for (auto& i : ans) cerr << i << '\n';
    map<char, vector<char>> parents;
    for (auto& node : ans) {
        times[node].second = times[node].first + durations[node];
        for (auto& child : adj[node])
            parents[child].push_back(node),
            times[child].first = max(times[child].first, times[node].second);
    }
    // for (auto& i : adj) {
    //     cerr << i.first << '\n';
    //     for (auto& j : parents[i.first]) cerr << j << ' '; cerr << '\n';
    // }
    // for (auto& i : times) cerr << i.first << '\t' << i.second.first << ' ' << i.second.second << '\n';
    for (int i = 0; i < nodes_count; i++)
        new_times[i + 'A'] = {1e9, 1e9};
    reverse(ans.begin(), ans.end());
    for (auto& node : ans) {
        if (!adj[node].size())
            new_times[node] = times[node];
        new_times[node].first = new_times[node].second - durations[node];
        // cerr << node << ' ' << new_times[node].first << ' ' << new_times[node].second << ' ' << durations[node] << '\n';
        for (auto& parent : parents[node]) 
            new_times[parent].second = min(new_times[parent].second, times[node].first);
    }
    // for (auto& i : new_times) cerr << i.first << '\t' << i.second.first << ' ' << i.second.second << '\n';
    reverse(ans.begin(), ans.end());
    vector<vector<char>> fans;
    for (auto& node : ans) 
        if (new_times[node].first == 0) 
            for (auto& j : dfs2(node, '0'))
                fans.push_back(j);
    cout << "\nSolutions: \n";
    for (auto& i : fans) {
        reverse(i.begin(), i.end());
        for (auto& j : i)
            cout << j << ' '; cout << '\n';
    }
}