#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>

#define MAXN 4000


std::vector< std::vector<int> > vec(4, std::vector<int>(0));

int N;

int main(){
    std::cin >> N;
    for(int ind_alph=0; ind_alph<4; ind_alph++){
        for(int i=0; i<N; i++){
            int tmp;
            std::cin >> tmp;
            (vec[ind_alph]).push_back(tmp);

        }
    }
    for(int i=0; i<4; i++){
        std::sort(vec[i].begin(), vec[i].end());
    }
    
    //ABとCDにわける
    //CDの和を全列挙。4000^2なので空間計算量は大丈夫
    //空間計算量を犠牲に時間計算量を改善するテク
    std::vector<int> sumCD;
    for(int ind_C =0; ind_C<N; ind_C++){
        for(int ind_D=0; ind_D<N; ind_D++){
            sumCD.push_back( vec[2].at(ind_C) + vec[3].at(ind_D) );
        }
    }
    std::sort(sumCD.begin(), sumCD.end());

    //あとはA,Bを全探索
    //C,Dは全組み合わせを列挙しソート済みなので、
    //O(n*n*logn)になる。(定数倍はかかないため。定数倍も酷くなるともちろん影響を及ぼす)
    int ans=0;
    for(int ind_C =0; ind_C<N; ind_C++){
        for(int ind_D=0; ind_D<N; ind_D++){
            int tmp = vec[0].at(ind_C) + vec[1].at(ind_D);
            int offset = 0 - tmp;
            if(std::binary_search(sumCD.begin(), sumCD.end(), offset)) ans++;
        }
    }

    printf("%d\n", ans);
    return 0;
}
