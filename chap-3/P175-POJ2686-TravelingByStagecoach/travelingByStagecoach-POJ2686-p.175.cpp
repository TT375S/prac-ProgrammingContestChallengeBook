#include<iostream>
#include<algorithm>
#define MAX_n 8
#include <climits>

int n, m, a, b;
int ticket[MAX_n] = {};

int route_cost[MAX_n][MAX_n] = {};

double dp[(1<<MAX_n) -1][MAX_n] = {};

//デバッグプリント用。10進数整数を2進数01表示に。どっかからのコピペ。
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
    std::cin >>n >>m >>a >>b;
    //0オリジンに直す
    a -= 1;
    b -= 1;
    for(int i=0; i< n; i++){
        std::cin >> ticket[i];
    }

    for(int i=0; i<MAX_n; i++){
        for(int k=0; k<MAX_n; k++){
            route_cost[i][k] = INT_MAX/16;
        }
    }

    int tmpFrom, tmpTo;
    while(std::cin >> tmpFrom >> tmpTo){
        //添字は0オリジンに直す
        std::cin >> route_cost[tmpFrom-1][tmpTo-1];
        route_cost[tmpTo-1][tmpFrom-1] = route_cost[tmpFrom-1][tmpTo-1];
    }

    printf("%d %d %d %d\n", n, m, a, b);
    for(int i=0; i<n; i++)printf("%d ", ticket[i]);
    printf("\n");

    for(int i=0; i<MAX_n; i++){
        for(int k=0; k<MAX_n; k++){
            
            int tmp = route_cost[i][k] ;
            if(tmp != INT_MAX/16)printf("r[%d[%d] %d\n", i, k, tmp);
        }
    }
}

void solve(){
    for(int i=0; i<=(1<<n)-1; i++){
        for(int k=0; k<MAX_n; k++){
            dp[i][k] = INT_MAX/16;
        }
    }
    dp[(1<<n) -1][a] = 0;
    

    double minResult = INT_MAX/16;
    //全てのチケットの使い方を計算する
    for(int ticket_state = (1<<n)-1; ticket_state >= 0; ticket_state--){
        minResult = std::min(minResult, dp[ticket_state][b]);
        printf("%d min%d\n", ticket_state,minResult);
        //全てのチケットの使い方につき、o_p -> d_p のコストを計算する
        for(int start_p=0; start_p<m; start_p++){
            for(int dest_p=0; dest_p<m; dest_p++){
                //全てのo_p -> d_pにつき、その間をどのチケット(まだ残ってる)を使って移動するか計算する
                for(int ind_usedTicket = 0; ind_usedTicket<n; ind_usedTicket++){
                    //チケットがまだあるか確認
                    if( (ticket_state >>ind_usedTicket) & 1){
                        if(start_p == a && ticket_state == (1<<n) -1) printf("   HERE! r=%d\n", route_cost[start_p][dest_p]);
                        //あるならチケットを消費して移動した計算をする
                        dp[ticket_state & ~(1<<ind_usedTicket)][dest_p] = std::min(dp[ticket_state & ~(1<<ind_usedTicket)][dest_p], dp[ticket_state][start_p] + (double)route_cost[start_p][dest_p]/ticket[ind_usedTicket] );
                        //printf("[%3s][%d] + r[%d][%d] = %f\n", to_binString(ticket_state).c_str(), start_p, start_p, dest_p, dp[ticket_state][start_p] + (double)route_cost[start_p][dest_p]/ticket[ind_usedTicket]);
                        //printf("[%3s][%d] %f\n", to_binString(ticket_state & ~(1<<ind_usedTicket)).c_str(), dest_p, dp[ticket_state & ~(1<<ind_usedTicket)][dest_p]);
                    }
                }
            }
        }
    }

    if(minResult == INT_MAX/16){
        printf("NO ROUTE\n");
        return;
    }

    printf("%f\n", minResult);
}

int main(){
    input();
    solve();
    return 0;
}
