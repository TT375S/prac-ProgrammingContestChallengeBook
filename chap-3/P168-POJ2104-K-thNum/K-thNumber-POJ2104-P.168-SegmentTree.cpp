#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include<cmath>
#include<climits>

#define MAX_n 100000
#define MAX_m 5000

#define ST_SIZE (1<<18)-2

int n, m;

int a[MAX_n] = {};
int sorted_a[MAX_n] = {};
int i[MAX_m] = {}, j[MAX_m] ={}, k[MAX_m]={};
std::vector<int> ST_data[500];

void input(){
    std::cin >> n >> m;
    for(int i=0; i<n; i++){
        std::cin >> a[i];
    }
    for(int q=0;q<m; q++){
        std::cin >> i[q] >> j[q] >> k[q];
    }
}

void initST(int nodeNum, int range_l, int range_r){
    //leaf
    if(range_r - range_l == 1){
        ST_data[nodeNum].push_back(a[range_l]);
    }else{
        int range_m = (range_r + range_l)/2;
        int child_l = 2*nodeNum + 1;
        int child_r = 2*nodeNum + 2;

        initST(child_l, range_l, range_m);
        initST(child_r, range_m, range_r);

        ST_data[nodeNum].resize(range_r - range_l);

        std::merge(ST_data[child_l].begin(), ST_data[child_l].end(), ST_data[child_r].begin(), ST_data[child_r].end(), ST_data[nodeNum].begin());
    }

    printf("complete! %d %d %d \n", nodeNum, range_l, range_r);
}

//[range_l, range_r) 
int countLesserThanOrEqual(int tag_l, int tag_r, int target, int nodeNum, int node_l, int node_r){
    //range chacking
    //two range do not completely  cross.
    if(tag_r <= node_l || tag_l >= node_r) return 0;
    //completely contain
    if(tag_l <= node_l && tag_r >= node_r){
        return std::upper_bound(ST_data[nodeNum].begin(), ST_data[nodeNum].end(), target) - ST_data[nodeNum].begin();
    }
    //pertialy contain
    else{
        int child_l = 2*nodeNum + 1;
        int child_r = 2*nodeNum + 2;
        int node_m = (node_r + node_l)/2;
        
        int leftCount = countLesserThanOrEqual(tag_l, tag_r, target, child_l, node_l, node_m );
        int rightCount = countLesserThanOrEqual(tag_l, tag_r, target, child_r, node_m, node_r );
        
        return leftCount + rightCount;
    }
}


void solve(){
    for(int i=0; i<n; i++) sorted_a[i] = a[i];
    std::sort(sorted_a, sorted_a +n);

    for(int ind_q=0; ind_q<m; ind_q++){
        printf("q: (%d, %d, %d)\n", i[ind_q], j[ind_q], k[ind_q]);
        
        //binary search
        int range_l = 0;
        int range_r = n-1;
        int range_m = 0;
        int median = 0;
        
        while(range_l < range_r){
            range_m = (range_r + range_l)/2;
            median = sorted_a[range_m];
            int count = countLesserThanOrEqual(i[ind_q]-1, j[ind_q]-1 + 1, median, 0, 0, n);
            
            //printf("range(%d, %d, %d) searchRange(%d, %d) median:%d count:%d, \n",range_l, range_r, range_m, i[ind_q], j[ind_q],median,  count);
            
            if(count == k[ind_q]){
                range_l = range_r = range_m;
            }
            else if(count < k[ind_q]){
                range_l = range_m;
            }else{
                range_r = range_m;
            }
        }
        printf("%d\n", sorted_a[range_r]);
    }
}

int main(){
    input();
    printf("f1\n");
    initST(0, 0, n);
    printf("f2\n");
    solve();
    return 0;
}
