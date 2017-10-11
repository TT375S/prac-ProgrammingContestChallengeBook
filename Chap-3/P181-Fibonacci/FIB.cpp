#include <iostream>
#include <vector>

typedef std::vector<std::vector<int>> matrix;
typedef long long ll;

matrix A;

void mat_print(matrix &A){
    for(int r=0; r<A.size(); r++){
        for(int c=0; c<A[0].size(); c++){
            printf("%d \n", A[r][c]);
        }
        printf("\n");
    }
}

matrix mat_mul(matrix &A, matrix &B){
    ll ansRowSize = A.size();
    ll ansColumnSize = B[0].size();
    ll innerSize = A[0],size();

    matrix ANS(matrix, A.size(());
    if(A[0].size() != B.size()){
        printf("mat err\n");
        return ANS;
    }

    ll res = 0;
    for(ll row = 0; row < ansRowSize; row++){
        for(ll column=0; column< ansColumnSize; column++){
            for(ll inner=0; inner< innerSize; column++){
                res += A[row][inner] * B[inner][column];
            }
        }
    }
    return ANS;
}

matrix mat_pow(matrix &A, ll exp){
    matrix ANS;
    if(exp & 1){
        ANS = mat_mul(A, A);
    }
    while(exp!=1){
        exp = exp >> 1;

        ANS = mat_mul(A, &ANS);
    }

    return ANS;
}



int main(){
    ll n;
    std::cin >> n;
    matrix A;
    A[0][0] = 1; A[0][1] = 1;
    A[1][0] = 1; A[1][1] = 0;
    matrix ANS = mat_pow(A, n);
    return -0;
}
