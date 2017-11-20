#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <climits>
#include <cmath>
#include <queue>
#include <utility>
using namespace std;

#define MAX_V 100
#define INF INT_MAX/4
#define EMPTY -1

typedef pair<int, int> P;

struct edge {int to, cap, cost, rev;};
vector<edge> G[MAX_V];

struct node {int dist, prevV, prevE;};
vector<node> V;

int h[MAX_V]; //ポテンシャル。ノードs-u間の最短距離と本では説明されているが、なぜかソースコードでは各iの残余グラフ上でのs-u間の最短距離を積算している。理解できませんでした

int min_cost_flow(int ind_startNode, int ind_endNode, int flow){
    int res = 0;

    //ポテンシャルの初期化
    memset(h, 0, sizeof(h));

    //流量が規定に到達するまで流せる経路を繰り返し探してそこに流す
    while(flow > 0){
        //ダイクストラ法でポテンシャルhを更新する...らしい
        
        //ノード初期化。未到達の区別のため距離を無限で初期化さえすれば良い
        for(int i=0; i<V.size(); i++){
            V[i].dist = INF;
            V[i].prevV = EMPTY;
            V[i].prevE = EMPTY;
        }

        priority_queue<P, vector<P>, greater<P> > que;
        
        V[ind_startNode].dist = 0;
        que.push(P(0, ind_startNode));

        while(!que.empty()){
            //一番sからの距離の短いノードを取り出す
            P p = que.top(); que.pop();
            int ind_v = p.second;
            //queに追加された後に、distがより小さく更新されていたら、無視する
            if(V[ind_v].dist < p.first) continue;
            for(int ind_e = 0; ind_e < G[ind_v].size(); ind_e++){
                edge &e = G[ind_v][ind_e];
                bool isFlowable = e.cap > 0;
                bool isMinUpdatable = V[e.to].dist > V[ind_v].dist + e.cost + h[ind_v] - h[e.to];
                if(isFlowable && isMinUpdatable){
                    //ポテンシャルを使うことにより、distは常に0以上になるのでダイクストラ法が使える
                    V[e.to].dist = V[ind_v].dist + e.cost + h[ind_v] - h[e.to];
                    V[e.to].prevV = ind_v;
                    V[e.to].prevE = ind_e;
                    que.push(P(V[e.to].dist, e.to));
                }
            }
        }

        //目的ノードまでの距離がINFなら到達できなかったということ        
        if(V[ind_endNode].dist == INF){
            //規定流量に満たないので不可能として-1を返す
            return -1;
        }

        //hに、このときの残余グラフでのs-u間最短距離を「積算」する。なぜかは不明。本ではh(v) = (s-v間の最短距離)と書かれているのだが...
        for(int i=0; i<V.size(); i++) {h[i] += V[i].dist; }

        //経路の最小容量を求める
        int d = flow;
        for(int ind_v = ind_endNode; ind_v != ind_startNode; ind_v = V[ind_v].prevV){
            d = min(d, G[ V[ind_v].prevV][V[ind_v].prevE].cap);
        }

        flow -= d;
        //通信料金は 毎秒の通信サイズ*通信料単価
        //dist[t]はh[t] - h[s]だが、h[s] == 0なので、dist[t] == h[t]になる。
        res += d * h[ind_endNode];

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

int K, N;
#define MAX_N 1000
int a[MAX_N], b[MAX_N], w[MAX_N];
vector<int> x;

void input(){
    cin >> N >> K;
    for(int i=0; i< N; i++){
        int tmpA, tmpB, tmpW;
        cin >> tmpA >> tmpB >> tmpW;   
        a[i] = tmpA;
        b[i] = tmpB;
        w[i] = tmpW;
        x.push_back(a[i]);
        x.push_back(b[i]);
    }
    sort(x.begin(), x.end());
    x.erase(unique(x.begin(), x.end()), x.end() );
}

int main(){
    input();
    int m = x.size();   //x軸上の座標を表すノードの数
    int s = m;          
    int t = s+1;        

    //ノード。x軸状の座標が全てノードになるのではなく、区間として与えられた座標だけがノードとなる
    //たとえば、区間として(10,20), (1,2), (5,8)が与えられた場合、それらをソートし重複を除き、1,2,5,8,10,20がノードとなる。インデックス0~5に対応する。
    V.resize(t+1);
    
    //sからノード番号0(ノードインデックス)に容量Kコスト0の辺をはる
    add_edge(s, 0, K, 0);
    //ノード番号m-1(ノードインデックス)から容量K、コスト0の辺をはる
    add_edge(m-1, t, K, 0);

    //容量無限でコスト0の辺をx軸上の座標を表す全てのノード間にはる
    for(int ind_node =0; ind_node+1 < m; ind_node++){
        add_edge(ind_node, ind_node+1, INF, 0);
    }
    

    for(int ind_sec=0; ind_sec<N; ind_sec++){
        //与えられた区間に対応する辺を張る
        //コストの符号が反転していることに注意！最小費用流を求めるアルゴリズムを使うため。
        int u = find(x.begin(), x.end(), a[ind_sec]) - x.begin();
        int v = find(x.begin(), x.end(), b[ind_sec]) - x.begin();
        add_edge(u, v, 1, -1*w[ind_sec]);
    }
    
    printf("%d\n", -1*min_cost_flow(s, t, K));

    return 0;
}

