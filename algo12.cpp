#include<bits/stdc++.h>
using namespace std;

const int K = 26;

struct Vertex {
    int next[K];
    bool leaf = false;
    int p = -1;
    char pch;
    int link = -1;
    int go[K];
    vector<Vertex> Lv;

    Vertex(int p=-1, char ch='$') : p(p), pch(ch) {
        fill(begin(next), end(next), -1);
        fill(begin(go), end(go), -1);
    }
};
struct Steck {
    int node;
    stack<int> S;
};
vector<Vertex> t(1);

void add_string(string const& s) {
    int v = 0;
    for (char ch : s) {
        int c = ch - 'a';
        if (t[v].next[c] == -1) {
            t[v].next[c] = t.size();
            t.emplace_back(v, ch);
        }
        v = t[v].next[c];
    }
    t[v].leaf = true;
}

int go(int v, char ch);

int get_link(int v) {
    if (t[v].link == -1) {
        if (v == 0 || t[v].p == 0)
            t[v].link = 0;
        else
            t[v].link = go(get_link(t[v].p), t[v].pch);
    }
    return t[v].link;
}

int go(int v, char ch) {
    int c = ch - 'a';
    if (t[v].go[c] == -1) {
        if (t[v].next[c] != -1)
            t[v].go[c] = t[v].next[c];
        else
            t[v].go[c] = v == 0 ? 0 : go(get_link(v), ch);
    }
    return t[v].go[c];
} 
void dfs(int node, int par, unordered_map<int,stack<int>> &S, vector<Steck> &S1, vector<vector<int>> &lv, vector<bool> &in_H,vector<bool> &in_S){
    for(int leaf : lv[node]){
        S[leaf].push(node);
        if(!in_S[leaf]){
            in_S[leaf]=true;
            Steck *temp=new Steck();
            temp->node=leaf;
            temp->S=S[leaf];
            S1.push_back(*temp);
        }
    }
    for(int x : t[node].next){
        if(x != -1){
            dfs(x, node, S, S1, lv, in_H,in_S);
        }
    }
    if(t[node].leaf){
        for(auto it : S1){
            in_H[it.S.top()] = 1;
            in_S[it.node]=0;
            //S1.erase(it);
            //ins[x]=0
            //pop sx from s
            for (auto it1 = S1.begin(); it1 != S1.end();it1++){
                if((*it1).node==it.node){
                    S1.erase(it1);
                    break;
                }
            }
        }
        S1.clear();
    }
    else{
        for(int leaf : lv[node]){
            S[leaf].pop();
            if(!S[leaf].empty()&&in_H[S[leaf].top()]==0){
                if(!in_S[leaf]){
                    in_S[leaf]=true;
                    Steck *temp=new Steck();
                    temp->node=leaf;
                    temp->S=S[leaf];
                    S1.push_back(*temp);
                }
            }else{
                if(in_S[leaf]){
                    in_S[leaf]=false;
                    for (auto it = S1.begin(); it != S1.end();it++){
                        if((*it).node==leaf){
                            S1.erase(it);
                            break;
                        }
                    }
                    //pop Sx from S
                }
            }
        }
    }
}

vector<bool> mark_H(vector<vector<int>> &lv, vector<int> &leaves){
    vector<bool> in_H(t.size(), 0);
    vector<bool> in_S(t.size(),0);
    for(int leaf : leaves){
        in_H[leaf] = 1;
    }
    in_H[0] = 1;
    vector<Steck> S1;
    unordered_map<int,stack<int>> S;
    dfs(0, -1, S, S1, lv, in_H,in_S);
    return in_H;
}


int main(){
    vector<string> P = {"aabaa", "aadbd", "dbdaa"};
    for(string str : P){
        add_string(str);
    }

    int no_of_nodes = t.size();
    vector<vector<int>> Lv(no_of_nodes);
    vector<int> leaves;
    for(int i = 0; i < no_of_nodes; i++){
        if(t[i].leaf){
            leaves.push_back(i);
            auto y = i;
            while(y > 0){
                int temp = get_link(y);
                Lv[temp].push_back(i);
                y = temp;
            }
        }
    }
    // for(int i = 0; i < no_of_nodes; i++){
    //     cout << i << ": ";
    //     for(auto lv : Lv[i]){
    //         cout << lv << " ";
    //     }
    //     cout << endl;
    // }
    vector<bool> in_H = mark_H(Lv, leaves);
    for(int x : in_H){
        cout << x << " ";
    }
    cout << endl;
}
