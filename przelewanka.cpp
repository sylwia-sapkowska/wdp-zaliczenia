#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <queue>

using namespace std;

int number_of_vessels;
vector<int>volumes, target_volumes;

// Reads input and checks if there is a vessel with 0 or target volume
// (if there's no such vessel, finishing state is unreachable)
void read_input(){
    cin >> number_of_vessels;
    assert(number_of_vessels >= 0);
    volumes.resize(number_of_vessels);
    target_volumes.resize(number_of_vessels);
    bool is_there_any = 0;
    for(int i = 0; i < number_of_vessels; i++){
        cin >> volumes[i] >> target_volumes[i];
        assert(volumes[i] >= 0 && target_volumes[i] >= 0);
        assert(volumes[i] >= target_volumes[i]);
        is_there_any |= (target_volumes[i] == 0 || target_volumes[i] == volumes[i]);
    }
    if (!is_there_any){
        cout << -1 << endl;
        exit(0);
    }
}

// Checks if state is target state and updates distance
void check_and_push(vector<int>&state, map<vector<int>, int>&dist, queue<vector<int>>&q, int d){
    if (state == target_volumes){
        cout << d << endl;
        exit(0);
    }
    if (dist.find(state) == dist.end()){
        dist[state] = d;
        q.push(state);
    }
}

// Performs bfs over all reachable states from starting one (where every vessel is empty)
// and checks if target state is reachable
// State is represented as vector of integers, where i-th element is current amount of water in i-th vessel
void bfs(){
    queue<vector<int>>q;
    vector<int>start_state(number_of_vessels, 0);
    q.push(start_state);
    unordered_map<vector<int>, int>dist;
    dist[start_state] = 0;
    while(!q.empty()){
        auto state = q.front();
        q.pop();
        int d = dist[state];
        // 1. fill i-th vessel
        for(int i = 0; i < number_of_vessels; i++){
            vector<int>new_state = state;
            new_state[i] = volumes[i];
            check_and_push(new_state, dist, q, d + 1);
        }

        // 2. empty i-th vessel
        for(int i = 0; i < number_of_vessels; i++){
            vector<int>new_state = state;
            new_state[i] = 0;
            check_and_push(new_state, dist, q, d + 1);
        }

        //3. pour from i-th to j-th vessel
        for(int i = 0; i < number_of_vessels; i++){
            for(int j = 0; j < number_of_vessels; j++){
                if (i == j) continue;
                vector<int>new_state = state;
                int pour = min(new_state[i], volumes[j] - new_state[j]);
                new_state[i] -= pour;
                new_state[j] += pour;
                check_and_push(new_state, dist, q, d+1);
            }
        }
    }
    cout << -1 << endl;
}

int main(){
    read_input();
    bfs();
    return 0;
}