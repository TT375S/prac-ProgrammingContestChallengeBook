#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

#define MAX_V 50
#define INF (1<<20)

struct edge{ int to, cap, rev; };

vector<edge> G[MAX_V];
bool isUsedNode[MAX_V];

//隣接リストに辺とその逆辺も追加。
//(ノード間には一個しか辺がないのでこれでok)
void add_edge(int from, int to, int capacity){
    int ind_e_from = (int)G[from].size() ;
    int ind_e_to = (int)G[to].size();
    
    G[from].push_back( (edge){to, capacity, ind_e_to});
    G[to].push_back( (edge){from, 0, ind_e_from});
}

//sからの流出量を増やせる、増加パスをDFSにより探し、増加流量を返す。
//探索済み箇所を外部変数であるused配列に持つので、dfs呼び出し前にリセットを忘れずに。
int dfs(int origin, int target, int flow){
    if(origin == target) return flow;
    
    isUsedNode[origin] = true;
    
    for(int ind_edge=0; ind_edge < G[origin].size(); ind_edge++){
        edge &currentEdge = G[origin][ind_edge];
        
        bool isNotTriedAndReachableNode = !isUsedNode[currentEdge.to] && currentEdge.cap > 0;
        if(isNotTriedAndReachableNode){
            //流量は、パスの中で最小の容量を持つ通信路によって決定される。
            int cuurentMinCapacity =min(flow, currentEdge.cap);
            int minCapacityInFlowPath = dfs(currentEdge.to, target, cuurentMinCapacity);
            
            bool isReachableToTarget = minCapacityInFlowPath > 0;
            if(isReachableToTarget){
                currentEdge.cap -= minCapacityInFlowPath;
                G[currentEdge.to][currentEdge.rev].cap += minCapacityInFlowPath;
                return minCapacityInFlowPath;
            }
        }
    }
    //全ての探索は失敗、ターゲットへ至る増加パスなし。
    return 0;
}

//sからtへの最大流量を求める
int max_flow(int start, int target){
    int flow = 0;
    int newFlow = -1;
    
    while(newFlow != 0){
        memset(isUsedNode, 0, sizeof(isUsedNode));
        newFlow = dfs(start, target, INF);
        flow += newFlow;
    }
    
    return flow;
}

int s=0, t = 0, v=0;

//s:開始ノード,t:目的地ノード,v:ノードの数,あと辺のつなぎ方を隣接行列で受け取る
void input(){
    cin >> s >>  t >> v;
    
    for(int row = 0; row < v; row++){
        for(int col=0; col < v; col++){
            int tmpCap;
            cin >> tmpCap;
            if(tmpCap == 0) continue;   //隣接リストで持つので繋がってないところは無視
            add_edge(row, col, tmpCap);
        }
    }
}

int main(){
    input();
    printf("%d\n", max_flow(s, t) );
    return 0;
}

