# P206 POJ3057 evacuation
蟻本の通りですが...  
ある時刻Tまでに全ての人が脱出できるか考えます。　　
ある一つのドアに着目します。このドアから時刻t (t<=T)において脱出可能なのは、時刻0からtまでの間にこのドアにたどり着ける人々、つまりこのドアから距離t以内にいる人々です。そして、1秒につき1人しか出られないので、脱出可能な人数は0~1人です。(みんな早く出たいので、0人ということはないでしょうが...)  
時刻とドアのペアを一つ選ぶと、それに対応する脱出可能な人間の集合が決まります。  
このペアと個人に関する二部グラフの最大マッチングを求め、人間の数と等しいか比べることで、時刻Tまでに全員が脱出可能かが判定できます。  
あとは、Tを順次変えて脱出可能かどうか調べることで、Tを2分探索で絞り込み、答えを得ます。  

## advanced
使っている二部マッチングの最大マッチングを求めるアルゴリズムは、片方の部を順にインデックスが小さい方から見ていき、増加パスがあるかどうか探す。  

どんなに掛かっても、時刻X * Yまでには全員脱出できる(ドアが一つしかない障害物のない部屋に人間がみっちり詰まってるのとかを想像)。  
時刻X * Yまでの二部グラフを作り(実際に作るのは隣接リスト)、最大マッチングを求める。ループの中で毎回、現在のマッチング数が人間の数と等しいかチェックしていく。  
t=1に対応する(時刻-ドア)ペアはノードインデックス0~(num_d -1), t=2に対応するペアはnum_d ~ (2*num_d-1)というように、  
 (時刻-ドア)ペアは時刻の小さい順でその中でドアのインデックスが小さい順に並んでいるので、マッチング数が人間の数と同じくなったとき(==全員脱出可能)の最後に見た(時刻-ドア)ペアの時刻が、全員脱出最小時刻となる。  


#  参考
参考になる日本語サイトと英語サイト発見ならず！  
