#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

#define MAX_V 50
#define INF (1<<20)


struct edge{ int to, cap, rev; };

vector<edge> G[MAX_V];
int level[MAX_V];
int iter[MAX_V];

//隣接リストに辺とその逆辺も追加。
//(ノード間には一個しか辺がないのでこれでok)
void add_edge(int from, int to, int capacity){
    int ind_e_from = (int)G[from].size() ;
    int ind_e_to = (int)G[to].size();
    
    G[from].push_back( (edge){to, capacity, ind_e_to});
    G[to].push_back( (edge){from, 0, ind_e_from});
}

//BFSで開始ノードからの各ノードの最短距離(開始ノードを根としたときの根付き木でいう階層)を調べる。
//距離が遠くなる方向に探索するための工夫。だけど、結局、目的ノードまで繋がっているかは判断しないのでそこは無駄がまだある。
//そのムダをなくすには目的ノードからの距離を調べるようにすれば良い...らしい
//(参考URL:http://topcoder.g.hatena.ne.jp/Mi_Sawa/20140311/1394730336)
void bfs(int s){
    memset(level, -1, sizeof(level));
    queue<int> node_que;
    level[s] = 0;
    
    node_que.push(s);
    while(!node_que.empty()){
        int currentNode = node_que.front();
        node_que.pop();
        
        //ここから辿れる全てのノードの階層(sからの最短距離)を更新
        for(int ind_edge=0; ind_edge<G[currentNode].size(); ind_edge++){
            edge &currentEdge = G[currentNode][ind_edge];
            
            bool isPassableEdge = currentEdge.cap > 0;
            bool isReachedNode = level[currentEdge.to] >= 0;
            if(isPassableEdge && !isReachedNode){
                level[currentEdge.to] = level[currentNode] +1;
                node_que.push(currentEdge.to);
            }
        }
    }
}

//sからの流出量を増やせる、増加パスをDFSにより探し、増加流量を返す。
//探索済み箇所を外部変数であるused配列に持つので、dfs呼び出し前にリセットを忘れずに。
int dfs(int origin, int target, int flow){
    if(origin == target) return flow;

    //iter[ノードインデックス]にどの辺まで見たのかを記録する。次回は、その辺以降を調べることで無駄を省く
    for(int &ind_edge = iter[origin]; ind_edge < G[origin].size(); ind_edge++){
        edge &currentEdge = G[origin][ind_edge];
        
        bool isReachableNode = currentEdge.cap > 0;
        //常にターゲットノードへ向かう方向に辿る(逆戻りしない)。こうすると最短のパスが見つかる。(Dinic法は最短の増加パスを見つけてそこに流していく、らしい)
        //大なりイコールではなく、等号なしの「大なり」なことに注意
        //(大なりイコールでも良いのでは？と思わなくも無いがよくわかりません。最短、という条件があるからかな？1つだけ用意したテストケースでは動いたけど遅くなるのかも)
        bool isFrontwardNode = level[origin] < level[currentEdge.to];
        if(isReachableNode && isFrontwardNode){
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
    
    while(1){
        //後につづくDFSでなるべく短いパスを辿るため、startから各ノードへの最短距離をlevel[]に記録
        //この階層構造に基づいて、後のDFSをやる
        bfs(start);
        //startからtargetが辿れなくなったら(=ゴールに至るパスが無くなったら)終わり
        if(level[target] < 0) return flow;
        memset(iter, 0, sizeof(iter));
        //現状で最短の増加パスがなくなるまで探索を続ける
        while((newFlow =dfs(start, target, INF) ) > 0){
                    flow += newFlow;
        }
        //もっと長い増加パスがある。かもしれないのでループ。
        //ので、次のループでまたBFSをやり直し、startから各ノードへの最短距離をlevel[]に記録しなおす
        
        //たとえば、蟻本の例でいうと、最初はノード0がlevel:0、1と2がlevel:1、3と4がlevel:2である。
        //(このとき、3->4はパスに含まれない。levelが等しいから。)
        //例として最短経路である0->2->4の一部の通信路(最初に2->4の5Mbpsの通信路を使い尽くす。)を使い尽くしてしまうと、次にBFSするとノード0がlevel:0、1と2がlevel:1、3がlevel:2、4がlevel:3となる。
    }
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
