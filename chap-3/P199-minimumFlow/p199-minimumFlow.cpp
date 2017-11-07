#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <climits>
#include <cmath>
using namespace std;

#define MAX_V 100
#define INF INT_MAX/4
#define EMPTY -1

struct edge {int to, cap, cost, rev;};
vector<edge> G[MAX_V];

struct node {int dist, prevV, prevE;};
vector<node> V;

int min_cost_flow(int ind_startNode, int ind_endNode, int flow){
    int res = 0;

    while(flow > 0){
        //長さを無限で初期化
        for(int i=0; i<V.size(); i++){
            V[i].dist = INF;
            V[i].prevV = EMPTY;
            V[i].prevE = EMPTY;
        }
        
        //ベルマンフォード法で、通信料単価が最安の経路を見つける
        V[ind_startNode].dist = 0;
        bool update = true;
        while(update) {
            update = false;
            for(int ind_v = 0; ind_v < V.size(); ind_v++){
                
                bool isReached = V[ind_v].dist != INF;
                if(! isReached) continue;
                for(int ind_e = 0; ind_e < G[ind_v].size(); ind_e++){
                    edge &e = G[ind_v][ind_e];
                    
                    bool isCostUpdatable = V[e.to].dist > V[ind_v].dist + e.cost;
                    if(e.cap > 0 && isCostUpdatable){
                        V[e.to].dist = V[ind_v].dist + e.cost;
                        V[e.to].prevV = ind_v;
                        V[e.to].prevE = ind_e;
                        //ベルマンフォードは更新できる限り続ける
                        update = true;
                    }
                }
            }
        }
        //目的ノードまでのコスト(距離)が未更新なら到達不可
        if(V[ind_endNode].dist == INF){
            return -1;
        }

        //経路の最小容量を求める
        int d = flow;
        for(int ind_v = ind_endNode; ind_v != ind_startNode; ind_v = V[ind_v].prevV){
            d = min(d, G[ V[ind_v].prevV][V[ind_v].prevE].cap);
        }

        flow -= d;
        //通信料金は 毎秒の通信サイズ*通信料単価
        res += d * V[ind_endNode].dist;

        //辺の容量を更新
        for(int ind_v = ind_endNode; ind_v != ind_startNode; ind_v = V[ind_v].prevV){
            edge &e = G[ V[ind_v].prevV][V[ind_v].prevE]; 
            e.cap -= d;
            G[ind_v][e.rev].cap += d;
        }
    }
    return res;
}

void add_edge(int from, int to, int cap, int cost){
    int ind_rev_f = G[to].size();
    int ind_rev_b = G[from].size();

    G[from].push_back((edge){to, cap, cost, ind_rev_f});
    G[to].push_back((edge){from, 0, -1*cost, ind_rev_b});
}

int main(){
    int s, t, f;
    cin >> s >> t >> f;

    int numV, numE;
    cin >> numV;
    cin >> numE;

    V.resize(numV);

    for(int i=0; i<numE; i++){
        int tS, tE, tC, tD;
        cin >> tS >> tE >> tC >> tD;
        add_edge(tS, tE, tC, tD);
    } 
    
    printf("%d\n", min_cost_flow(s, t, f));

    return 0;
}

