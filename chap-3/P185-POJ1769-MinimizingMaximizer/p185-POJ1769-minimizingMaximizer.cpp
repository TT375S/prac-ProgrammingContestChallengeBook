#include <stdio.h>
#include <iostream>
#include <cmath>
#include<algorithm>

#define MAX_m 500000
#define MAX_n 50000

int seg_tree[2 * MAX_n -1];
int seg_elementSize = 0;

//n_は葉の数
//セグメントツリー自体は要素数を2のベキ乗に揃える(簡単化のため)
void seg_init(int n_){
    seg_elementSize = 1;
    while(seg_elementSize < n_) seg_elementSize <<= 1;

    for(int i=0; i< 2*seg_elementSize -1; i++) seg_tree[i] = INT_MAX /4;
    printf("DEBUG: seg_elementSize=%d\n", seg_elementSize);
}

//ind_targetNumは0オリジン
//要素の値を更新する。それはセグメント木の葉に位置している。
void seg_update(int ind_targetNum, int num){
    printf("DEBUG: seg_update: call(%d, %d)\n", ind_targetNum, num);

    //最初に葉を更新
    ind_targetNum += seg_elementSize -1;
    seg_tree[ind_targetNum] = num;

    printf("DEBUG: seg_update: ind_targetNum=%d\n", ind_targetNum);

    while(ind_targetNum > 0){
        //ツリーを遡る
        ind_targetNum = (ind_targetNum -1) /2;
        int child_r = ind_targetNum*2 + 2;
        int child_l = ind_targetNum*2 + 1;
        //区間の最小値を更新
        seg_tree[ind_targetNum] = std::min(seg_tree[child_l], seg_tree[child_r]);
        printf("DEBUG: seg_update: MINUPDATE: ind_targetNum=%d, seg_tree=%d\n", ind_targetNum, seg_tree[ind_targetNum]);
                                
    }
}

//[trange_l, trange_r) の範囲の最小値を返す
//後半はノード情報。そうすればノードに範囲の情報を持たせたり、持たせないなら計算して求めたり、をしなくて済む。
int seg_query(int trange_l, int trange_r, int ind_node, int nrange_l, int nrange_r){
    printf("DEBUG: seg_query: call(%3d, %3d, %3d, %3d, %3d)\n", trange_l, trange_r, ind_node, nrange_l, nrange_r);
    
    //まったくレンジの交差なし
    if(nrange_r <= trange_l || trange_r <= nrange_l){
        //printf("DEBUG: seg_query: RANGE NOT CROSS\n");
        return INT_MAX /4;
    }

    //完全に探索レンジがこのノードの担当レンジを包含してる
    if(trange_l <= nrange_l && nrange_r <= trange_r){
        printf("DEBUG: seg_query: RANGE COMPLETELY CROSS : RET=%d\n", seg_tree[ind_node]);
        return seg_tree[ind_node];
    }
    //一部分だけ包含している
    else{
        int child_l = ind_node*2 + 1;
        int child_r = ind_node*2 + 2;
        int nrange_m = (nrange_l + nrange_r)/2;

        //再帰的に求める
        int min_child_l = seg_query(trange_l, trange_r, child_l, nrange_l, nrange_m);
        int min_child_r = seg_query(trange_l, trange_r, child_r, nrange_m, nrange_r);
        
        return std::min(min_child_l, min_child_r); 
    }
}


int n, m;
int sort_s[MAX_m] = {};
int sort_e[MAX_m] = {};

void input(){
    std::cin >> n >> m;

    for(int i=0; i<m; i++){
        std::cin >> sort_s[i] >> sort_e[i];
    }

    printf("DEBUG: n:%d, m:%d\n", n, m);
    for(int i=0; i<m; i++){
        printf("DEBUG: %d %d\n", sort_s[i], sort_e[i]);
    }
}

void solve(){
    int dp[MAX_n + 1] = {};
    
    seg_init(n);
    //全部、無限大で更新しとく。
    std::fill(dp, dp+n+1, INT_MAX /4);
    
    for(int i = 0; i<n+1; i++) dp[i] = INT_MAX /4;

    //その区間(1番目~)の最大値が1番目に移動するような、ソートマシンの部分列の最小の長さ。当たり前だが0。
    dp[1] = 0;
    //1番目~1番目の区間で、最大値が1番目に移動するようなソートマシンの部分列の最小の長さは0。ソートしないでも既に1番目にいるから。
    seg_update(1, 0);
    
    printf("DEBUG: %d\n", seg_query(1, 1+1, 0, 0, seg_elementSize) ) ;
    
    printf("------\n");
    for(int ind_machine = 0; ind_machine<m; ind_machine++){
        printf("DEBUG: ind_machine=%d machine(%d, %d)\n", ind_machine, sort_s[ind_machine], sort_e[ind_machine]); 
        //seg_queryでこのソートマシンがソートする範囲まで来るために使うソートマシンの数の最小値を調べる
        //その最小値に+1(=この1はこのソートマシンを使った分の1)すると、このソートマシンのソートする範囲の右端に区間の最大値が来る
        //dp[ このソートマシンのソートする範囲の右端(=sort_e[ind_machine]) ]には、このソートマシンのソートする範囲の右端に,[1,(このソートマシンのソートする範囲の右端)]の区間の最大値を移動させるのに必要なソートマシンの最小数が入っているので、それと比べて更新する
        //(説明へたくそでごめん)
        int newMin = std::min(dp[ sort_e[ind_machine] ] , seg_query(sort_s[ind_machine], sort_e[ind_machine] +1, 0, 0, seg_elementSize ) + 1);
        printf("DEBUG: min=%d, dp=%d, query+1=%d\n", newMin, dp[sort_e[ind_machine] ], seg_query(sort_s[ind_machine], sort_e[ind_machine] +1, 0, 0, seg_elementSize ) + 1);
        dp[ sort_e[ind_machine] ] = newMin;
        seg_update( sort_e[ind_machine], newMin);       
    }

    std::cout << dp[n] << std::endl;
}

int main(){
    input();
    solve();
    return 0;
}

