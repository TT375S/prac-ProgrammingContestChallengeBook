#include <iostream>
#include <climits>
#include <algorithm>
#include <cstring>

#define MAX_n 15
#define EMPTY -1

int n;
int final_state = -1;

int cost_fromTo[MAX_n][MAX_n] = {};
//通った道のパターンは、一つの道につき「通った/通ってない」2通りなので2^n
int dp[1 << MAX_n][MAX_n] = {};

std::string to_binString(unsigned int val)
{
    if( !val )
        return std::string("0");
    std::string str;
    while( val != 0 ) {
        if( (val & 1) == 0 )  // val は偶数か？
            str.insert(str.begin(), '0');  //  偶数の場合
        else
            str.insert(str.begin(), '1');  //  奇数の場合
        val >>= 1;
    }
    return str;
}

void input(){
    std::cin >> n;
    
    for(int i=0; i<n; i++){
        for(int k=0; k<n; k++){
            cost_fromTo[i][k] = INT_MAX/4;
        }
    }
    
    int tmpS=0, tmpE=0;
    //!!!一行でやろうとするとオカシクなる!!!
    while(std::cin >> tmpS >> tmpE ){
        std::cin >> cost_fromTo[tmpS][tmpE];
    }
}

int rec(int p_state, int c_point){
    //すでにメモが記入されているならそのまま返す。それがメモ化再帰
    if(dp[p_state][c_point] >= 0) {
            return dp[p_state][c_point];
    }

    //終了条件達成(これって現在位置が0だって確認する必要ある？)
    //つまり再帰のスタック状、一番深い(浅い)ポイント。よって、0を返し、ここから再帰のスタックをたどることにより計算されていく
//    if(p_state == final_state && c_point == 0) return dp[p_state][c_point] = 0;

    //和をとったあとにminを使うので、オーバーフローしないようにINT_MAXは使わない
    int res = INT_MAX/16;
    for(int point = 0; point < n; point++){
        //既に通ったポイントかの確認
        if(!((p_state >> point) & 1 )) {
            res = std::min(res , rec( p_state | (1<<point), point) + cost_fromTo[c_point][point]);
        }
    }
    printf("(%6s,%d) %d\n",to_binString(p_state).c_str(), c_point, res);
    return dp[p_state][c_point] = res;
}

void solve(){
    input();
    //最終状態は例としてn=5なら11111みたいになる。LSB側1bitが出発地点(0)を表す
    final_state = (1 << n) - 1;
    std::memset(dp, EMPTY, sizeof(dp));
    /*
        勘違いしやすいが、値のイミ的には、dp[11111][0]は「どの点にも未訪問」「現在地が0」である。
        対してdp[00000][0]は、「全ての点に訪問済み」、「現在地が0」である。
        stateのLSB側1bitは、ゴールとなる0(スタートの0とは別扱い)の訪問を表している。
        だからrec(0,0)として値を求めている。
    */
    dp[final_state][0] = 0;
    printf("%d\n", rec(0, 0));
}

int main(){
    solve();
    return 0;
}

