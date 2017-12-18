#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include <complex>
using namespace std;

//Gauss-Jordannoの消去法
const double EPS = 1E-8;
typedef vector<double> vec;
typedef vector<vec> mat;

//It Solves Ax=b
//If no solution, this returns zero length array.
vec gauss_jordan(const mat& A, const vec& b){
    int n = A.size();
    mat B(n, vec(n+1));
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            B[i][j] = A[i][j];
        }
    }
    //Add b on tail of the A in order to process it simultaneously.
    for(int i=0; i<n;i++) B[i][n] = b[i];

    for(int i=0; i<n; i++){
        //Take a equation having the i-th large coeffiecient of the target variant to i-th row.
        int pivot = i;
        for(int j=i; j<n; j++){
            if(abs(B[j][i]) >abs(B[pivot][i])) pivot = j;
        }
        swap(B[i], B[pivot]);

        //If no solution or not unique, returns zero length array.
        if(abs(B[i][i]) < EPS) return vec();

        //make the coefficient of the target variant to 1.
        for(int j=i+1; j<=n; j++) B[i][j] /= B[i][i];
        for(int j=0; j<n; j++){
            if(i != j){
                //removing i-th variant from j-th equation
                for(int k=i+1; k<=n; k++) B[j][k] -= B[j][i] * B[i][k];
            }
        }

    }

    vec x(n);
    //The tail of the B is the answer.
    for(int i=0; i<n; i++) x[i] = B[i][n];
    return x;
}
//-----

#define MAX_N 10
#define MAX_M 10
bool DEBUG = true;

char grid[MAX_N][MAX_M+1];
int N,M;

bool can_goal[MAX_N][MAX_M];
int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};

//DFSにより到達可能な場所を調べる
void dfs(int x, int y){
    can_goal[x][y] = true;
    for(int i=0; i<4; i++){
        int nx = x+dx[i], ny=y+dy[i];
        if(0<=nx && nx<N && 0 <= ny && ny < M && !can_goal[nx][ny] && grid[nx][ny] != '#'){
            dfs(nx, ny);
        }
    }
}

void showMat(mat A){
    for(int i=0; i<A.size(); i++){
        for(int k=0; k<A[0].size(); k++){
            printf("%2.0f ", A[i][k]);
        }
        printf("\n");
    }
}

void solve(){
    mat A(N * M, vec(N*M, 0));
    vec b(N*M, 0);
    
    //到達可能な場所を調べて記録
    for(int x=0; x<N; x++){
        for(int y=0; y<M; y++){
            can_goal[x][y] = false;
        }
    }
    dfs(N-1, M-1);

    for(int x=0; x<N; x++){
        for( int y=0; y<M; y++){
            //ゴール、または到達不可能な場合
            if(x==N-1 && y==M-1 || !can_goal[x][y]){
                A[x*M +y][x*M +y] = 1;
                continue;
            }   
            
            //到達可能な場合は、そこに辿りつける周りの4つのセルからここに来る確率を計算する
            int move = 0;
            for(int k=0; k<4;k++){
                int nx = x +dx[k], ny=y+dy[k];
                if(0<=nx && nx < N && 0<=ny && ny<M && grid[nx][ny] == '.'){
                    A[x*M + y][nx*M+ny] = -1;
                    move++;
                }
            }
            if(DEBUG)printf("(x,y)=(%d,%d)\n", x,y);
            if(DEBUG) showMat(A);
            b[x*M+y] = A[x*M+y][x*M+y] = move;
            if(DEBUG)printf("FIN\n");
            if(DEBUG) showMat(A);
        }
    }
    vec res = gauss_jordan(A, b);
    printf("%.8f\n", res[0]);
}

int main(){
    cin >> N >> M;
    for(int i=0; i<N;i++){
        cin >> grid[i];
        if(DEBUG) printf("%s\n", grid[i]);
    }
    solve();
    return 0;
}
