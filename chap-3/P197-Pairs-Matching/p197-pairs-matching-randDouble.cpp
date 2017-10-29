#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <climits>
#include <cmath>
using namespace std;

#define MAX_V 50

void printMat(vector<vector<double> > mat);

/*
 rank()は、
 https://qiita.com/TempProg/items/b67a3437ea604291aafe
 からのほぼ引用
 名前空間が衝突するとかなんとかで関数名を変えている。(あと中身も)
 */
int _rank(vector<vector<double> > mat, int n){
    double ltmp[100], tmp, a_tmp[100], b_tmp[100];
    int i, j, k;
    int count;
    int all_zero;
    
    for(i = 0; i < n; i++){
        all_zero = 0;
        if(abs(mat[i][i]) == 0){
            for(j = 0; j < n; j++){
                if(abs(mat[j][i]) != 0){
                    for(k = 0; k < n; k++){
                        tmp = mat[i][k];
                        mat[i][k] = mat[j][k];
                        mat[j][k] = tmp;
                    }
                } else if(j == n - 1)
                    all_zero = 1;
            }
        }
        
        if(!all_zero){
            for(j = i + 1; j < n; j++){
                for(k = 0; k < n; k++){
                    a_tmp[k] = mat[i][k] * mat[j][i];
                    b_tmp[k] = mat[j][k] * mat[i][i];
                }
                for(k = 0; k < n; k++)
                    mat[j][k] = b_tmp[k] - a_tmp[k];
            }
        }
    }
    
    printf("DEBUG:\n");
    printMat(mat);
    
    //double型にしたせいかこのままだとちゃんと数えられない？
   // count = 0;
   // for(i = 0; i < n; i++){
   //     if(mat[i][n] == 0)
   //         count++;
   // }
   // 
   // return (n - count);

    int allZeroRow = 0;
    for(int row =0; row<n; row++){
        for(int col=0; col<n; col++){
            if(abs(mat[row][col]) != 0){
                break;
            }
            if(col == n-1) allZeroRow++;
        }
    }
    return n-allZeroRow;
}
//---引用ココマデ---


void printMat(vector<vector<double> > mat){
    for(int i=0; i< mat.size(); i++){
        for(int k=0; k<mat[0].size(); k++){
            printf("%f ", mat[i][k]);
        }
        printf("\n");
    }
}

int friends[MAX_V][MAX_V];
int N;

int maximumMatching(int V, int relation[MAX_V][MAX_V]){
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> dist(0.001, 1.0);
    //uniform_int_distribution<int> dist(1, 1<<28); //整数でやると仕組み的に溢れちゃいそう？
    
    vector<vector<double> > mat;
    
    mat.resize(V);
    for(int i=0; i < V; i++) mat[i].resize(V);
    //Tutte行列の作成。関係があるところに、乱数を入れる
    for(int row=0; row< V; row++){
        for(int col=0; col< row; col++){
            if(relation[row][col]){
                double tmp = dist(mt);
                mat[row][col] = tmp;
                mat[col][row] = -1* tmp;
            }
        }
    }
    
    printf("DEBUG:\n");
    printMat(mat);

    //tutte行列のランク数は最大マッチングの「頂点数」に等しい。問題はペアの数を聞いているので2で割る
    return _rank(mat, V)/2;
}

void solve(){
    printf("%d\n", maximumMatching(2*N, friends) );
    return;
}

//人数が2*N人。Nと、友達関係の隣接行列を入力として受け取る。
void input(){
    cin >> N;
    
    //人間の人数は2*N
    for(int i=0; i< 2*N; i++){
        for(int k=0; k< 2*N; k++){
            cin >> friends[i][k];
        }
    }
}

int main(){
    input();
    solve();
    return 0;
}

