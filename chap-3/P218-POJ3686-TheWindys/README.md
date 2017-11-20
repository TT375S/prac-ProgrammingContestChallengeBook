# POJ3686 The Windy's
蟻本の解説がイミワカンネー場合は、  
図付きの<https://amoshyc.github.io/ojsolution-build/poj/p3686.html>  
と、解説がわかりやすい  
<http://blog.csdn.net/qq978874169/article/details/70339287>  
を参照のこと。  

１つの作業場でおもちゃを作成する場合、その終了「時刻」(時間とか書くからワケわかんなくなる)の平均は、  
A = {Z1 + (Z1+Z2) + (Z1+…+Zn)} / Nだ。  
ここでT = {Z1 + (Z1+Z2) + (Z1+…+Zn)} とする。  
書き換えるとT = N * T1 + (N-1) T2 + ..+ 1*Tnである  

これがもし複数の作業場での作業となると…。  
その作業場をインデックスkとする  
その作業場で、おもちゃインデックスiを作るのにかかる時間をZikとすると、Nをおもちゃの総数(ここで作るぶんではなく全体の総数)とすると  
Tk = N * Z1k + (N-1) Z2k + … + Znk  
となる。  
  
***なお注意点として、Z1の方が先に作り始めたオモチャ。一番最近に作り始めたのがZnになる。***  

よって、一つの工場につきn個のノードを作り、そのn個のノードと各オモチャとの間に、Zikを N, N-1, …,, 1 倍したコストをもつキャパが1の辺を張ればよい。  
添字の書き分けのためにnを使ったが、このnはおもちゃの個数のNと同じ。  

そんで最小費用流を流す。  
フローの値は、おもちゃの個数に等しい、3つなら3。  
そうすると3本流れる。  

そうして流したときのコストが、求めるべきT。なのでNでわれ。  


#  参考
<http://blog.csdn.net/qq978874169/article/details/70339287>
<https://amoshyc.github.io/ojsolution-build/poj/p3686.html>
[ベルマンフォード法](https://qiita.com/intatonix/items/8a50556697206ee89bcd)
[フローの話。蟻本のはdist(s,v)をh(v)に積算していくとか書いてある] (https://gist.github.com/satos---jp/711c056cb63af7fbccb569f98a086b06)
[これも同上](https://www.slideshare.net/Drafaer/kmc-advanced-3)
