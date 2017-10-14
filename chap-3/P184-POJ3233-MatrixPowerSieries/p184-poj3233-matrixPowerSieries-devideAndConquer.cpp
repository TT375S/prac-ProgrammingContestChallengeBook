#include <iostream>
#include <vector>
#include <stdio.h>
#include <map>

typedef std::vector<std::vector<long long> > matrix;
typedef long long ll;

int n,k,M;
std::map<int, matrix > memo;

void mat_print(const matrix &A){
    for(int r=0; r<A.size(); r++){
        for(int c=0; c<A[0].size(); c++){
            printf("%lld ", A[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}

matrix mat_mul(const matrix &A, const matrix &B){
    ll ansRowSize = A.size();
    ll ansColumnSize = B[0].size();
    ll innerSize = A[0].size();
    
    matrix ANS(ansRowSize, std::vector<ll>(ansColumnSize));
    
    if(A[0].size() != B.size()){
        printf("mat err\n");
        return ANS;
    }
    
    for(ll row = 0; row < ansRowSize; row++){
        for(ll column=0; column< ansColumnSize; column++){
            ll res = 0;
            for(ll inner=0; inner< innerSize; inner++){
                res += A[row][inner] * B[inner][column];
            }
            ANS[row][column] = res;
        }
    }
    
    return ANS;
}

matrix mat_add(const matrix &A, const matrix &B){
    matrix ANS(A.size(), std::vector<ll>(A[0].size() ) ); 
    //行列のサイズが違うと和がとれない
    if(A.size() != B.size() ) return ANS;
    if(A[0].size() != B[0].size() ) return ANS;
    
    for(int row=0; row< A.size(); row++){
        for(int column=0; column< A[0].size(); column++){
            ANS[row][column] = A[row][column] + B[row][column];
        }
    }
    return ANS;
}

matrix mat_pow(const matrix &A, int exp){
    matrix ANS(A.size(), std::vector<ll>(A[0].size(), 0 ) );
    matrix _A(A.size(), std::vector<ll>(A[0].size(), 0) );

    _A = A;

    //n×nの正方行列しか想定してません
    for(int i=0; i<A.size(); i++){
        ANS[i][i] = 1;
    }
    
    //指数をビット表示し、ビットが立ってるときだけ掛け算。nビット目(0オリジン)なら(2^n)乗を掛け算する。
    //例としてexp=21ならば、二進数で10101である
    //なのでA^(2^0) * A^(2^2) * A^(2^4) = A^1 * A^4 * A^16 = A^21となる。
    while(exp>0){
        if( exp& 1){
            ANS = mat_mul(ANS, A);
        }
        //0オリジンでnビット目の時に掛け算する、A^(2^n)を計算
        _A = mat_mul(_A, _A);
        exp >>= 1;
    }
    
    return ANS;
}

matrix rec(const matrix &A, int len){
    if(memo.count(len) > 0){
        return memo[len];
    }
   
    matrix L(n, std::vector<ll>(n) );
    matrix R(n, std::vector<ll>(n) );
    
    R = L = rec(A, len/2);

    matrix B(n, std::vector<ll>(n) );
    B = mat_pow(A, len/2);

    R = mat_mul(B, R);

    //全体の項数が奇数のときは最後の1項が余る。のでそれを計算して足す。
    if(len % 2 != 0){
        R = mat_add(R, mat_pow(A, len) );
    }

    memo[len] = mat_add(L, R);
    return memo[len];
}

int main(){
    std::cin >> n >> k >> M;
    matrix I(n, std::vector<ll>(n) );
    
    for(int row=0; row<n; row++){
        for(int column=0;  column<n; column++){
            if(row == column) I[row][column] = 1;
        }   
    }

    matrix A(n, std::vector<ll>(n));

    for(int row=0; row<n; row++){
        for(int column=0;  column<n; column++){
            std::cin >> A[row][column];   
        }   
    }
    
    memo[0] = I;
    memo[1] = A;

    ////TODO: %Mをやり忘れてる 
    mat_print(rec(A, k) );
    return 0;
}

