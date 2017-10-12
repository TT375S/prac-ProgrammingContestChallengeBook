#include <iostream>
#include <climits>
#include <algorithm>
#include <cstring>

#define MAX_n 15

int n;
int final_state = -1;

int cost_fromTo[MAX_n][MAX_n] = {};
//通った道のパターンは、一つの道につき「通った/通ってない」2通りなので2^n
int dp[1 << MAX_n][MAX_n] = {};

//デバッグプリント用
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

//int rec(int p_state, int c_point){
//    //すでにメモが記入されているならそのまま返す。それがメモ化再帰
//    if(dp[p_state][c_point] >= 0) {
//            return dp[p_state][c_point];
//    }
//
//    //和をとったあとにminを使うので、オーバーフローしないようにINT_MAXは使わない
//    int res = INT_MAX/16;
//    for(int point = 0; point < n; point++){
//        //既に通ったポイントかの確認
//        if( (p_state >> point) & 1 ) {
//            //通った場所なら、ソコまでのコストを再帰的に求めていく。
//            res = std::min(res , rec( p_state ^ (1<<point), point) + cost_fromTo[c_point][point]);
//        }
//    }
//    printf("(%6s,%d) %d\n",to_binString(p_state).c_str(), c_point, res);
//    return dp[p_state][c_point] = res;
//}



void solve(){
    input();
    //最終状態は例としてn=5なら11111みたいになる。LSB側1bitが出発地点(0)を表す
    final_state = (1 << n) - 1;
    //今回はdp配列を大きな値で初期化しないとイケナイ。
    //メモ化だと入ってるかどうかを区別するため、絶対ありえない-1を入れていた。
    //今回は普通に最小を取るので、初期化はINF。
    std::memset(dp, INT_MAX/16, sizeof(dp));
   
    for(int i=0; i<= final_state; i++){
        for(int k=0; k<n; k++){
            dp[i][k] = INT_MAX/16;
            printf("dp[%d][%d] = %d\n", i, k, dp[i][k]);
        }
    }
    dp[0][0] = 0;



    //dynamic programmingで計算
    for(int state = 1; state <= final_state; state ++){
        for(int start_p=0; start_p <n; start_p++){
            for(int dest_p = 0; dest_p <n; dest_p++){
                //一つ前の状態に戻ってる、あと後ろの辺のコスト、添字が本は逆（まちがい）
                dp[state][start_p] = std::min(dp[state][start_p], dp[state & ~(1<<dest_p)][dest_p]  + cost_fromTo[dest_p][start_p]);
                printf("dp[%6s][%2d] %d\n", to_binString(state).c_str(), start_p, dp[state][start_p]);
            }
        }
    }

    printf("%d\n", dp[final_state][0]);
}

int main(){
    solve();
    return 0;
}

