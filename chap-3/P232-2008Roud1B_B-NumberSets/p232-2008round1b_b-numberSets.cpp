#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <climits>
#include <cmath>
#include <queue>
#include <utility>
using namespace std;
typedef long long ll;

#define INF INT_MAX/4
#define EMPTY -1

//---union-find木---
#define MAX_N 100
int par[MAX_N];    //parent
int t_rank[MAX_N];   //木の深さ

//0~nまでを収容可能に初期化
void init(int n){
    for(int i=0; i < n; i++){
        //最初は全部、自分自身が親==バラバラなグループ
        par[i] = i;
        t_rank[i] = 0;
    }
}

//xの所属する木の根を返す
int find(int x){
    if(par[x] == x){
        return x;
    }else{
        int root = find(par[x]);
        par[x] = root;
        return root;
    }
}

//xとyの所属している木を統合することにより、xとyを同じグループにする
//rankが小さいほうが、大きいほうに統合される。統合されるときは、統合「される」側の木の根の親が、統合「する」側の木の根になる。
//rankが等しいときのみ、rankが増える。
void unite(int x, int y){
    int root_x = find(x);
    int root_y = find(y);

    if(root_x == root_y) return;

    if(t_rank[root_x] < t_rank[root_y]){
        par[root_x] = root_y;
    }else{
        par[root_y] = root_x;
        //ランクが増えるのは、同じ深さの木を併合したときだけ。（違う高さの時は、root直下に取り込まれるため取り込んだ側の高さは変わらない。)
        if(t_rank[root_x] == t_rank[root_y]){
            t_rank[root_x]++;
        }
    }
}

//同じ木（集合）に所属しているかを、木の根が同じかどうかで判定する
bool isSame(int x, int y){
    //rootが同じなら、同じグループだということ
    return find(x) == find(y);
}
//---union-find木ココマデ---

int prime[1000000];
int num_p;

//---素数を求める
bool isPrime[1000000];
void culc_prime(){
    //エラトステネスの篩(http://arc360.info/algo/prime2.html)
    const int n = 1000000;
    isPrime[0] = false;
    isPrime[1] = false;
    int count = 0;
    for(int i=2;i<n;i++)
    {
        isPrime[i] = true;
    }
    for(int i=2; (i * i) <n;i++)
    {
        if(isPrime[i] == true)
        {
            for(int y = (i << 1);y<n;y += i)
            {
                if(isPrime[y] == true)
                {
                    isPrime[y] = false;
                    count++;
                }
            }
        }
    }

    //素数として配列にコピー
    int ind = 0;
    for(int i=0; i<n; i++){
        if(isPrime[i]){
            prime[ind] = i;
            ind++;
        }
    }
    num_p = ind;
}
//---素因数を求めるココマデ---


ll A, B, P;

void solve(){
    int len = B-A+1;
    init(len);    //union-find treeの初期化

    for(int ind_p = 0; ind_p < num_p; ind_p++){
        if(prime[ind_p] >= P){
            ll start = (A + prime[ind_p] -1)/prime[ind_p] * prime[ind_p]; //A以上の、prime[ind_p]の倍数
            ll end = B/prime[ind_p] * prime[ind_p];                       //B以下の、prime[ind_p]の倍数
            
            for(ll multiples_p = start; multiples_p <= end; multiples_p += prime[ind_p]){
                int ind_minimum_multiples_p = start - A;  //区間[A,B]の中で、一番小さいprime[ind_p]の倍数のインデックス。
                int ind_multiples_p = multiples_p -A;     //区間[A,B]の中の、prime[ind_p]の倍数のインデックス
                unite(ind_minimum_multiples_p, ind_multiples_p);
            }

        }
    }

    int num_trees = 0;
    for(ll numInRange = A; numInRange <= B; numInRange++){
        int ind_num = numInRange - A;
        bool isRoot = find(ind_num) == ind_num;
        //根の数だけ木がある、つまりグループがある
        if(isRoot) num_trees++;
    }
    printf("%d\n", num_trees);
}

int main(){
    cin >> A >> B >> P;
    culc_prime();

    solve();
    return 0;
}
