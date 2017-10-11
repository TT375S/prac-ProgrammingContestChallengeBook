#include <stdio.h>
#include <iostream>
#include <cmath>
#define MAX_NC 10000

int N, C;
int S[MAX_NC] = {}; //クエリ。ターゲットクレーンのインデックス。入力は1オリジンなのに注意
int A[MAX_NC] = {}; //クエリ。指定角度。度数法。「ターゲットクレーンと、ターゲットクレーンの次のクレーンの間の角度」が、反時計回りで指定される。
int L[MAX_NC] = {};	


double rightAngle[MAX_NC] = {}; //クレーンセットの次（＝右)のクレーンセットが、「現在のクレーンセットにとっての地面に垂直な方向」に対しどれだけ反時計回りに傾いてるかの角度。初期値0(ラジアン)で、つまり地面に対し垂直。(クレーンと次のクレーンの角度自体は問題文にあるように180度。)
double vx[MAX_NC] = {}; //クレーンセットの先端の座標値。クレーンセットの根元が地面に垂直に立っているとした場合の座標。
double vy[MAX_NC] = {};

void input(){
    std::cin >> N >> C;
    for(int i=0; i<N; i++){
        std::cin >> L[i];
    }
    for(int i=0; i<C; i++){
        std::cin >> S[i];
    }
    for(int i=0; i<C; i++){
        std::cin >> A[i];
    }
}

void init(int nodeNum, int left, int right){
    rightAngle[nodeNum] = vx[nodeNum] = 0.0;

    //葉のとき
    if(right - left <= 1){
        vy[nodeNum] = L[left];
    }
    //内部接点の時
    else{
        int rangeMid = (right + left)/2;
        int children_r = 2*nodeNum + 2;
        int children_l = 2*nodeNum + 1;
        
        右の子と左の子を更新
        init(children_r, rangeMid, right);
        init(children_l, left, rangeMid);

        vy[nodeNum] = vy[children_r] + vy[children_l];
    }
}

//targetCrane、currentNodeは0オリジン。
void changeAngle(int targetCrane, double diffRightAngle, int currentNode, int range_l, int range_r){
    //printf(" trace: %d %f %d %d %d\n", targetCrane, diffRightAngle, currentNode, range_l, range_r);
  
    //このノードが受け持つ範囲に、ターゲットクレーンが入ってるかどうかを検査
    if(targetCrane < range_l) return;
    if(targetCrane >= range_l && targetCrane < range_r){
        //printf("  traceChange\n");
        //if(range_r - range_l <=1 ) printf("  leaf!\n");
        int children_l = 2*currentNode + 1;
        int children_r = 2*currentNode + 2;
        int range_m = (range_r + range_l)/2;
        
        //ターゲットノードが現在のノードより左にある...ということはつまり、右側にある我々と、自分より右側にある奴らの角度も+=rightAngleされていくというわけ。
        //絶対角度指定だとスマートに処理が書けない(ターゲットノードを目的角度にするために回した角度分、ターゲットノードの右側のクレーンたちを回さなくてはならないため+=を使うべきなのです)
        //なおこれは、targetCraneがちょうど自分自身(葉)のときも含む。
        
        //前述のようにターゲットクレーンより右か、またはターゲットクレーン自身なら角度に+=してく
        if(targetCrane <= range_m){
            rightAngle[currentNode] += diffRightAngle;
        }
       
        if(  !(range_r - range_l <= 1) ){
            //葉の時は子ノード無いから辿らない
            changeAngle(targetCrane, diffRightAngle, children_l, range_l, range_m);
            changeAngle(targetCrane, diffRightAngle, children_r, range_m, range_r);
       
            //葉のときはvx,vyは完全固定なため変に更新しない
            double _cos = cos(rightAngle[children_l]);
            double _sin = sin(rightAngle[children_l]);
            vx[currentNode] = vx[children_l] + vx[children_r]*_cos - vy[children_r]*_sin;
            vy[currentNode] = vy[children_l] + vx[children_r]*_sin + vy[children_r]*_cos;
        }
    }
}

double prev_rightAngle[MAX_NC] = {};	//前述のように、絶対角度でなく相対角度で指定するため、前回の角度を記録する。

void solve(){
    input();
    init(0, 0, N);

    //180度に初期化。ただしラジアン。
    for(int i=0; i<N; i++) prev_rightAngle[i] = M_PI;
    
    //クエリ処理
    for(int ind_q=0; ind_q<C; ind_q++){
        int targetCrane = S[ind_q] -1; //インデックスを0オリジンに直す
        double targetAngle = (A[ind_q] / 360.0) * (2.0 * M_PI);
        double offsetAngle = targetAngle - prev_rightAngle[targetCrane];
        
        //printf(" %d %f %f\n", targetCrane, targetAngle, offsetAngle);

        changeAngle(targetCrane, offsetAngle, 0, 0, N);
        prev_rightAngle[targetCrane] = targetAngle;

        printf("%.2f %.2f\n", vx[0], vy[0]);
    }
}

int main(){
    solve();
    return 0;
}

