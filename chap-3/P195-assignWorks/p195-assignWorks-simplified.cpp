#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

#define MAX_V 50
#define INF (1<<20)
#define EMPTY -1
#define MAX_N 10000
int N, K;
bool can[MAX_N][MAX_N];


struct edge{ int to, cap, rev; };

vector<edge> G[MAX_V];
bool isCheckedNode[MAX_V];
int match[MAX_V];

//隣接リスト作成
//(ノード間には一個しか辺がないのでこれでok)
void add_edge(int from, int to, int capacity){
    int ind_e_from = (int)G[from].size() ;
    int ind_e_to = (int)G[to].size();
    
    G[from].push_back( (edge){to, capacity, ind_e_to});
    //今回はいらない
    //G[to].push_back( (edge){from, capacity, ind_e_from});
}

//片方の部のノード(今回はコンピュータ)を与える。それに隣接するもう片方の部のノード(仕事ノード)と繋げるようにDFSをして次々と繋ぎかえる。
//隣接するノードが空くように繋ぎ変えられればtrue,できないならfalseを返す
//呼び出す前にisCheckedNode配列をリセットするのを忘れずに。
bool dfs(int v){
    if(match[v] != EMPTY) return true;  //マッチ相手すでにいるならやめる
    isCheckedNode[v] = true;
    //隣接するノードを全部チェックしていく
    for(int ind_edge=0; ind_edge < G[v].size(); ind_edge++){
        int current_adjacentNode = G[v][ind_edge].to;
        int current_adjacentNode_matchNode = match[current_adjacentNode];
        
        bool isMatched = current_adjacentNode_matchNode != EMPTY;
        
        //(評価の順番を強調するためヤヤコシイ形になってしまいました。( A|| (B&&C)とかなってるときC++ではtrueかfalseか確定した時点で評価を打ち切るので))
        //隣接する奴が何ともマッチされてない場合はマッチしちゃう
        if(!isMatched){
            match[v] = current_adjacentNode;
            match[current_adjacentNode] = v;
            return true;
        }else{
            bool isChecked = isCheckedNode[current_adjacentNode_matchNode];
            if(!isChecked){
                bool isReconnectable = dfs(current_adjacentNode_matchNode);
                if (isReconnectable){
                    //隣接する奴がマッチ済みのときかつ、マッチ相手のマッチ相手を別な奴につなぎ変えられたとき。
                    match[v] = current_adjacentNode;
                    match[current_adjacentNode] = v;
                    return true;
                }
            }
        }
    }
    return false;
}




//s:開始ノード,t:目的地ノード,v:ノードの数,あと辺のつなぎ方を隣接行列で受け取る
void input(){
    cin >> N >> K;
    
    for(int ind_computer=0; ind_computer<N; ind_computer++){
        for(int ind_job=0; ind_job<K; ind_job++){
            cin >> can[ind_computer][ind_job];
        }
    }
}

void solve(){
    memset(match, EMPTY, sizeof(match) );
    
    //接続。コンピュータノード→仕事ノードだけをつなぐ。
    for(int ind_cp=0; ind_cp<N; ind_cp++){
        for(int ind_job=0; ind_job<K; ind_job++){
            if(can[ind_cp][ind_job]){
                add_edge(ind_cp, N + ind_job, 1);
            }
        }
    }
    
    int ans = 0;
    for(int ind_v = 0; ind_v <N; ind_v++){
        //永遠に再帰してしまうのですでにチェック済みのものは区別するため
        memset(isCheckedNode, false, sizeof(isCheckedNode) );
        if(dfs(ind_v)) ans++;
    }
    
    printf("%d\n", ans);
}

int main(){
    input();
    solve();
    return 0;
}

