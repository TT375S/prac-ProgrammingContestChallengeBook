# POJ1769 Minimizing maximizer
ind_lastSorter番目までのSorterを使い、最大値が位置pos_maxまで移動するのに必要なSorterの最小の部分列の長さをdp[ind_lastSorter][pos_max]とし、これを更新していくことを考えてしまう。  
dp[ind_lastSorter+1][sorter_t[ind_lastSorter] ] = min(dp[ind_lastSorter][sorter_t[ind_lastSorter] ], min{dp[ind_lastSorter][new_pos_max] | sorter_s[ind_lastSorter] ≦ new_pos_max ≦ sorter_t[ind_lastSorter]} +1 )  
とやって更新する。ややこしい式だが、参考URLで絵付きで解説してあるのでそれを見るとわかりやすい。  
しかし、これはSorterの個数mと同じだけ、全位置(n個ある)についての最小値を調べ更新するため、O(m×n)になる...らしい。(計算量の計算が苦手、直さなくちゃ...!)  
  
mが500000、nが50000になりうるので、とても間に合わない。  

よって、pos_maxに最大値が来るのに必要なSorterの最小の数をdp[pos_max]に保存し、区間の最小値を求めるのにセグメント木を使いdpを更新していくことで,O(m log(n) )で答えが求められる...  
  
まず順当な動的計画法であるdp[ind_lastSorter][pos_max]を使う方法を思いつき、それを表に書いて手で試してみれば、何らかの区間の最小値を高速に求める方法(今回はセグメント木)を使ってdp[pos_max]を更新すれば良いかな...と思いつくことができると思う。  
さもそれができるかのように書いたけど、それが難しいので自分はさっぱり思いつかない。  

注意点としてdpとかセグメント木をすごく大きい整数で初期化したくなるのでINT_MAXを使ってしまうと、更新のときに最小値比較のまえに+1して比較していて、INT_MAX+1は絶対値がすごく大きい負の整数となるので結果がおかしくなる  
INT_MAX/4くらいで初期化するべき？  


#  参考
<https://www.slideshare.net/hcpc_hokudai/advanced-dp-2016>
