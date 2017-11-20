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

int min_cost_flow(int ind_startNode, int ind_endNode, int flow){
    int res = 0;

    //流量が規定に到達するまで流せる経路を繰り返し探してそこに流す
    while(flow > 0){
        //ダイクストラ法
        
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
                bool isMinUpdatable = V[e.to].dist > V[ind_v].dist + e.cost;
                if(isFlowable && isMinUpdatable){
                    V[e.to].dist = V[ind_v].dist + e.cost;
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

        //経路の最小容量を求める
        int d = flow;
        for(int ind_v = ind_endNode; ind_v != ind_startNode; ind_v = V[ind_v].prevV){
            d = min(d, G[ V[ind_v].prevV][V[ind_v].prevE].cap);
        }

        flow -= d;
        //通信料金は 毎秒の通信サイズ*通信料単価
        res += d*V[ind_endNode].dist;

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

#define MAX_N 100
#define MAX_M 100
int N, M;
int Z[MAX_N][MAX_M];
void input(){
    cin >> N >> M;
    for(int i=0; i< N; i++){
        for(int k=0; k<M; k++){
            cin >> Z[i][k];
        }
    }
}

int main(){
    input();
    
    int s = N*M+N;  //スタートノード(ソース)
    int t = s+1;    //エンドノード(シンク)
    int f = N;      //フロー。作るべきおもちゃの数
    
    V.resize(t+1);
    
    //edgeの作成
    /*
        インデックスの付け方
        0,1,..,N-1 : toy
        N,N+1, ..., N+ N-1 : fact1
        N+N+0, N+N+1, ...,N+N+N-1 : fact2
         .
         .
         .
        N+N(i-1), ..., N+N(i-1)+N-1: fact_i
         .
         .
         .
        N*M+0, ..., N*M+(N-1): fact_M
        N*M+N: s
        N*M+N +1: t
    */
    for(int ind_toy=0; ind_toy < N; ind_toy++){
        for(int ind_fact=0; ind_fact < M; ind_fact++){
            for(int factor_N = 1; factor_N <= N; factor_N++){
                //容量1、コストZ_ik * factNの辺を、toyとfactoryの間にはる
                int ind_fact_Nth = N+(ind_fact*N + (factor_N-1));
                int cost_fact_Nth = Z[ind_toy][ind_fact] * factor_N;
                add_edge(ind_toy, ind_fact_Nth, 1, cost_fact_Nth);
            }
        }
    }
    
    //sからtoyへ、容量1、コスト0の辺を張る
    for(int ind_toy=0; ind_toy < N; ind_toy++){
        add_edge(s, ind_toy, 1, 0);
    } 

    //factoryからtへ、容量1、コスト0の辺をはる
    for(int ind_fact=0; ind_fact < M; ind_fact++){
        for(int factor_N = 1; factor_N <= N; factor_N++){
            int ind_fact_Nth = N+(ind_fact*N + (factor_N-1));
            add_edge(ind_fact_Nth, t, 1 ,0);
        }
    }
    
    printf("%f\n", (double)min_cost_flow(s, t, f)/N);

    return 0;
}

