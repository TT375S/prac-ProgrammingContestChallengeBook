#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <climits>
#include <cmath>
#include <utility>
#include <queue>
using namespace std;

#define MAX_X 100
#define MAX_Y 100
#define EMPTY -1

//----二部グラフお決まり(from P.195)----

//片方の部は、ドア*取りうる時刻の数だけある。もう片方の部は、個人なので最大MAX_X*MAX_Y人。適当に決めた
#define MAX_V MAX_X*MAX_Y

vector<int> G[MAX_V];
int match[MAX_V];
bool isCheckedNode[MAX_V];
bool used[MAX_V];

void add_edge(int u, int v){
    G[u].push_back(v);
    G[v].push_back(u);
}

bool dfs(int v){
    used[v] = true;
    for(int ind_e=0; ind_e < G[v].size(); ind_e++){
        int u=G[v][ind_e], w=match[u];
        if(w<0 || !used[w] && dfs(w)){
            match[v] = u;
            match[u] = v;
            return true;
        }
    }
    return false;
}

int bipartite_matching(int num_v){
    memset(match, EMPTY, sizeof(match) );
    int ans = 0;
    
    for(int ind_v = 0; ind_v <num_v; ind_v++){
        if(match[ind_v] < 0){
            memset(used, 0, sizeof(used));
            if(dfs(ind_v)) ans++;
        }
    }
    
    return ans;
}
//----二部グラフお決まり----

struct vec2{
    int x, y;
public:
    vec2(int a = 0, int b = 0) {
        this->x = a;
        this->y = b;
    }
};

vec2 dirc[4] = {vec2(-1,0), vec2(0,-1), vec2(1, 0), vec2(0, 1)};

int X, Y;
char field[MAX_X][MAX_Y + 1];   //終端文字分、+1

vector<vec2> doors;
vector<vec2> peoples;

int dist[MAX_X][MAX_Y][MAX_X][MAX_Y];   //ドアの座標から人(通路)の座標までの最短距離


bool isEvacuationable(int t){
    int num_d = doors.size();
    int num_p = peoples.size();
    
    int V = t * num_d + num_p;
    
    //インデックス0~(d-1)が時刻=1に対応するドア。
    //インデックスd~(2*d -1)が時刻=2に対応するドア
    //インデックス(t-1)*d ~ (2*t -1)が時刻=tに対応するドア
    //t*d ~ t*d+p-1は個人
    
    //与えられるtごとに2部グラフを作り直す(が、実は前回作ったものを再利用することができる。それは別バージョン)
    for(int ind_node = 0; ind_node < V; ind_node++){G[ind_node].clear();}
    for(int ind_door = 0; ind_door < num_d; ind_door++){
        for(int ind_people=0; ind_people < num_p; ind_people++){
            //ドアからの最短距離 = 時間だけ考えればそのマスにいる人間の最短脱出時間
            int min_time_evac = dist[doors[ind_door].x][doors[ind_door].y][peoples[ind_people].x][peoples[ind_people].y];
            if( min_time_evac > EMPTY){
                for(int time_evac = min_time_evac; time_evac <= t; time_evac++){
                    //時間的には時刻tまでに脱出できる、ドア-個人　間のペアを作る
                    add_edge((time_evac -1) * num_d + ind_door, t * num_d + ind_people);
                }
            }
        }
    }
    
    int num_V = (t*num_d + num_p-1) - 0 +1;    //ノードの数。ドア-時刻ペアの数と、個人の数を合わせるとこうなる
    //人数分、(時刻tまでの時刻-ドア のペア)とマッチさせることができた == 時刻tまでに全員脱出できた
    return (bipartite_matching(num_V) == num_p);
}

//各ドアからの、通路の各地点の最短距離をBFSで求める
void bfs_minDist(int x, int y, int fromDist[MAX_X][MAX_Y]){
    queue<vec2> q;
    fromDist[x][y] = 0;
    q.push(vec2(x, y));
    
    while(!q.empty()){
        vec2 pos = q.front(); q.pop();
        //タテヨコミギヒダリ、4方向に進む
        for(int ind_dirc = 0; ind_dirc < 4; ind_dirc++){
            vec2 nextPos;
            nextPos.x = pos.x + dirc[ind_dirc].x;
            nextPos.y = pos.y + dirc[ind_dirc].y;
            
            bool isInRange =  (0 <= nextPos.x && nextPos.x <= X) && (0 <= nextPos.y && nextPos.y <= Y);
            bool isEmpty = fromDist[nextPos.x][nextPos.y] <= EMPTY;
            bool isAilse = field[nextPos.x][nextPos.y] == '.';
            
            if(isInRange && isEmpty && isAilse){
                fromDist[nextPos.x][nextPos.y] = fromDist[pos.x][pos.y] +1;
                q.push(nextPos);
            }
        }
    }
}

void solve(){
    int n = X*Y;
    doors.clear();
    peoples.clear();
    
    memset(dist, EMPTY, sizeof(dist));
    
    //読み込んだフィールドの解釈
    for(int x=0; x<X; x++){
        for(int y=0; y<Y; y++){
            bool isDoor = field[x][y] == 'D';
            bool isAisle = field[x][y] == '.';
            if(isDoor){
                doors.push_back(vec2(x, y));
                bfs_minDist(x, y, dist[x][y]);
            }else if(isAisle){
                peoples.push_back(vec2(x, y));
            }
            
        }
    }
    
    //二分探索で全員が脱出できる最小の時刻tを絞り込む
    int lb = -1, ub = n+1;
    while(ub -lb>1){
        int mid = (lb+ub)/2;
        if(isEvacuationable(mid)) ub = mid;
        else lb = mid;
    }
    
    if(ub == n+1){
        printf("impossible\n");
    }else{
        printf("%d\n", ub);
    }
}

void input(){
    cin >> X >> Y;
    
    for(int x=0; x<X; x++){
        cin >> field[x];
    }
}


int main(){
    input();
    solve();
    return 0;
}
