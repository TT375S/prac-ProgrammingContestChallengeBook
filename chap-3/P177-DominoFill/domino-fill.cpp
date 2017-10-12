#include <iostream>
#include <algorithm>
#include <climits>
#include <cstring>

#define MAX_nm 15
#define BLACK 1
#define WHITE 0

int n, m,M;
int map[MAX_nm][MAX_nm] = {};
//現在、[x][y]に着目している。これより前は全て敷き詰められている。[S]は、(x,y)を含む境界の状態
int dp[MAX_nm][MAX_nm][1<<MAX_nm] = {};


void input(){
    std::cin >>n >>m >>M;
    
    for(int row=0; row<n; row++){
        
        char buf[MAX_nm] = {};
        scanf("%s", buf);
        for(int column=0; column<m; column++){
            switch(buf[column]){
                case '.':
                    map[row][column] = WHITE ;
                    break;
                case 'x':
                    map[row][column] = BLACK;
                    break;
                default:
                    printf("INPUT ERR\n");
                    break;
            }
        }
    }

    printf("%d %d %d\n", n, m, M);
    for(int i=0; i<n;i++){
        for(int j=0; j<m; j++){
            printf("%d", map[i][j]);
        }
        printf("\n");
    }
}

void solve(){
    //あくまで「場合の数」なので、[0][0]かつ教会に1枚もない状態も1。とカウントするみたい
    dp[0][0][0] = 1;
    
    /*
        何をしているか？
        「座標(m,n)まで埋まっていて、なおかつ境界が1<<(m-1)になっている(==はみ出ているタイルなし)」をカウントしたい
        以下の更新の仕方では、下から全部の境界状態を網羅してカウントしていく。i,jは網羅して更新されるが、nextStateはusedState(現在の)から計算する。そのため、[m][n][1<<(m-1)]の値を更新できる==タイルを綺麗に全部埋められる、というわけで、カウントできる。
        [x][y][S]は、その状態(境界状態だけではなく)にたどり着ける、場合の数を表しているのである。１個ずつ辿って更新している。
        だから掛け算じゃなくて足し算になってる
    */
    
    //上行左列から順番に見ていく
    for(int row=0; row<n; row++){
        for(int column=0; column<m; column++){
            //その使用状態も網羅する
            for(int usedState = 0; usedState<(1<<m); usedState++){
                printf("[%d][%d][%d] %d\n", row, column, usedState, dp[row][column][usedState]);
                //着目座標が何もなく空いている
                int isUsedHere = usedState & (1<<column);
                if(isUsedHere || map[row][column] == BLACK){
                    //一つ座標を進めるなら、境界のエリアも変わるのでその中身も変わる。stateは何列が使用中なのかの情報を持っているため。
                    int nextState = usedState & ~(1<<column);
                    //端っこだったら改行するための判定
                    if(column + 1 < m){
                        dp[row][column+1][nextState] += dp[row][column][usedState];
                    }
                    else{
                        dp[row+1][0][nextState] += dp[row][column][usedState];
                    }
                }
                else{
                    //横置きしてみる
                    if(column+1 <m && map[row][column+1]==WHITE && !(usedState & (1<<(column+1))) ){
                        int nextState = usedState | (1<<(column+1));
                        dp[row][column+1][nextState] += dp[row][column][usedState];
                    }
                    //縦置きしてみる
                    if(row+1 <n && map[row+1][column]==WHITE){
                        int nextState = usedState | (1<<(column));
                        //改行判定
                        if(column+1 < m)
                            dp[row][column+1][nextState] += dp[row][column][usedState];
                        else
                            dp[row+1][0][nextState] += dp[row][column][usedState];
                    }
                }
            }
        }

    }
    //最後の列でも特別に処理わけていない。(してるといえばしてるんだが、最後の列はハミ出てる部分のstateを考えなくてよいのに、これでは切り捨てていないで考えている。結果には関係ないが。)
    //そのためstateを二進数で表して100000みたいに(n,m)の注目座標「だけ」が使用状態のstateのときが最後の状態で答えになる。つまりdp[n][m][100000]みたいな最後のマスが埋まっていてそれよりあと(ハミ出ている)は全く未使用の状態が答えになる
    printf("%d\n", dp[n-1][m-1][(1<<(m-1))]);
}


int main(){
    input();
    solve();
    return 0;
}
