#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include<cmath>
#include<climits>
#define MAX_n 100000
#define MAX_m 5000
//BUCKET_SIZE = sqrt(MAX_n)
#define MAX_BUCKET_SIZE 400


int n, m;

int a[MAX_n] = {};
int sorted_a[MAX_n] = {};
int i[MAX_m] = {}, j[MAX_m] ={}, k[MAX_m]={};
int BUCKET_SIZE = 0;
std::vector<int> bucket[MAX_BUCKET_SIZE];

void input(){
    std::cin >> n >> m;
    for(int i=0; i<n; i++){
        std::cin >> a[i];
    }
    for(int q=0;q<m; q++){
        std::cin >> i[q] >> j[q] >> k[q];
    }
    printf("input succ\n");
    BUCKET_SIZE = sqrt(n);
}

//[range_l, range_r) 
int countLesserThanOrEqual(int range_l, int range_r, int target){
    int count = 0;

    int tmpI = range_l;
    int tmpJ = range_r;
    //count numbers  out of BUCKET space
    while(tmpI % BUCKET_SIZE !=0 && tmpI <= tmpJ ){
        if(a[tmpI] <= target) count++;
        tmpI++;
    }
    //tmpJ should cover BUCKT's left(start) end.
    while(tmpJ % BUCKET_SIZE !=0 && tmpI <= tmpJ){
        tmpJ--;
        if(a[tmpJ] <= target) count++;
    }
    
    //count lesser or equal numbers from in  all bucket which covers the target range .
    for(int ind_b = 0; ind_b < n/BUCKET_SIZE; ind_b++){
        //range checking
        //if this bucket completely covers the range...
        if(ind_b*BUCKET_SIZE >= range_l && ind_b*BUCKET_SIZE + BUCKET_SIZE -1 < range_r){
            count += std::upper_bound(bucket[ind_b].begin(), bucket[ind_b].end(), target) - bucket[ind_b].begin();
        }
    }

    return count;
}

void solve(){
    //distribute each buckets
    for(int ind_a = 0; ind_a < n; ind_a++){
        (bucket[ind_a / BUCKET_SIZE]).push_back(a[ind_a]);
        //merely copy
        sorted_a[ind_a] = a[ind_a];
    }
    //fill last bucket with INT_MAX
    int ind_a=n;
    while(ind_a % BUCKET_SIZE != 0){
        bucket[ind_a / BUCKET_SIZE].push_back(INT_MAX);
        ind_a++;
    }
    //sort each buckets
    for(int ind_bucket = 0; ind_bucket < n/BUCKET_SIZE; ind_bucket++){
        std::sort(bucket[ind_bucket].begin(), bucket[ind_bucket].end());
    }
    std::sort(sorted_a, sorted_a +n);
   
    printf("sorted\n");
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
            int count = countLesserThanOrEqual(i[ind_q]-1, j[ind_q]-1 + 1, median);
            
            printf("range(%d, %d, %d) searchRange(%d, %d) median:%d count:%d, \n",range_l, range_r, range_m, i[ind_q], j[ind_q],median,  count);
            
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
    solve();
    return 0;
}
