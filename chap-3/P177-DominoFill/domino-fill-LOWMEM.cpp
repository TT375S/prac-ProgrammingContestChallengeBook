#include <iostream>
#include <algorithm>
#include <climits>
#include <cstring>

#define MAX_nm 15
#define BLACK 1
#define WHITE 0

int n, m,M;
int map[MAX_nm][MAX_nm] = {};
//[y][x-1]と[y][x]の状態。本当は、「一個前」だから[y][x]と[y+1][0]のときもある。
//currが[y][x-1]で、rightが[y][x]にあたる。right_dpはcurr_dpより進んだ(より右の)の座標なのでright。わかりにくくてゴメン
int right_dp[1<<MAX_nm] = {};
int curr_dp[1<<MAX_nm] = {};

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
    //これは[0][0]かつ境界状態が0、つまり何にも置いてない状態
    right_dp[0] = 1;
    /*
     何をしているか？
     「座標(m,n)まで埋まっていて、なおかつ境界が1<<(m-1)になっている(==はみ出ているタイルなし)」をカウントしたい
     以下の更新の仕方では、下行右列、つまり右下端っこから全部の境界状態を網羅してカウントしていく。i,jは網羅して更新されるが、curr_stateはright_stateから計算する。そのため、[n][m][1<<(m-1)]の値を更新できる==タイルを綺麗に全部埋められる、というわけで、カウントできる。
     [y][x][S]は、その状態(境界状態だけではなく)にたどり着ける、場合の数を表しているのである。１個ずつ辿って更新している。
     だから掛け算じゃなくて足し算になってる
     */
    
    //逆順注意！！！
    for(int row=n-1; row>=0; row--){
        for(int column=m-1; column>=0; column--){
            for(int curr_state = 0; curr_state<(1<<m); curr_state++){
                printf("[%d][%d][%d] curr %d\n", row, column, curr_state, right_dp[curr_state]);
                
                //着目座標が何もなく空いている
                int isUsedHere = curr_state & (1<<column);
                if(isUsedHere || map[row][column] == BLACK){
                    //一つ座標を進めるなら、境界のエリアも変わるのでその中身も変わる。stateは何列が使用中なのかの情報を持っているため。
                    int prev_state = curr_state & ~(1<<column);
                    //着目座標がすでに埋まっているなら、一個前の[y][x]の情報を[y][x-1]に入れる
                    curr_dp[curr_state] = right_dp[prev_state];
                }
                else{
                    
                    //本来ならres%Mとすべきだが、なんか問題でM与えられてないので考慮していない
                    //横置きしてみる
                    if(column+1 <m && map[row][column+1]==WHITE && !(curr_state & (1<<(column+1))) ){
                        int prev_state = curr_state | (1<<(column+1));
                        curr_dp[curr_state] += right_dp[prev_state];
                    }
                    //縦置きしてみる
                    if(row+1 <n && map[row+1][column]==WHITE){
                        int prev_state = curr_state | (1<<(column));
                        curr_dp[curr_state] += right_dp[prev_state];
                    }
                }
            }
            std::swap(curr_dp, right_dp);
        }
    }
    
    //最後の列でも特別に処理わけていない。(してるといえばしてるんだが、最後の列はハミ出てる部分のstateを考えなくてよいのに、これでは切り捨てていないで考えている。結果には関係ないが。)
    //i,jを全部回したため、現在いる(最後に処理された)セルは[n][m]である。
    //その処理の最後にcurrとrightをスワップしたので、right_dpに[n][m]の情報が入っている。ので注目座標だけが使用状態でそれ以降未使用の状態を考えれば良いright_dp[1<<(m-1)]?
    //と思いきや答えはcurr_dpの中であった。なぜ...?理解できませんでした。
    
    //printf("%d\n", right_dp[0]);
    printf("%d\n", curr_dp[1<<(m-1)]);
    //printf("%d\n", right_dp[1<<(m-1)]);
    //printf("%d\n", curr_dp[0]);
}


int main(){
    input();
    solve();
    return 0;
}
