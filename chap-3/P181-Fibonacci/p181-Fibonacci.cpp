#include <iostream>
#include <vector>

typedef std::vector<std::vector<long long>> matrix;
typedef long long ll;

void mat_print(matrix &A){
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

matrix mat_pow(matrix &A, int exp){
    matrix ANS(A.size(), std::vector<ll>(A[0].size(), 0 ) );
    
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
        A = mat_mul(A, A);
        exp >>= 1;
    }
    
    return ANS;
}



int main(){
    int n;
    std::cin >> n;
    matrix A(2, std::vector<ll>(2));
    
    A[0][0] = 1; A[0][1] = 1;
    A[1][0] = 1; A[1][1] = 0;
    
    matrix ANS(2, std::vector<ll>(2));
    
    ANS = mat_pow(A, n);
    printf("%lld\n", ANS[1][0]);
    return 0;
}

