# POJ3686 The Windy's
蟻本の解説がイミワカンネー場合は、  
図付きの<https://amoshyc.github.io/ojsolution-build/poj/p3686.html>  
と、解説がわかりやすい  
<http://blog.csdn.net/qq978874169/article/details/70339287>  
を参照のこと。  

１つの作業場でおもちゃを作成する場合、その終了「時刻」(時間とか書くからワケわかんなくなる)の平均は、  
A = {Z_1 + (Z_1+Z_2) + (Z_1+…+Z_n)} / Nだ。  
ここでT = {Z_1 + (Z_1+Z_2) + (Z_1+…+Z_n)} とする。  
書き換えるとT = N * T_1 + (N-1) T_2 + ..+ 1*T_nである  

これがもし複数の作業場での作業となると…。  
その作業場をインデックスkとする  
その作業場で、おもちゃインデックスiを作るのにかかる時間をZ_ikとすると、Nをおもちゃの総数(ここで作るぶんではなく全体の総数)とすると  
T_k = N * Z_1k + (N-1) Z_2k + … + Z_Nk  
となる。  
  
***なお注意点として、Z_1kの方が先に作り始めたオモチャ。一番最近に作り始めたのがZ_Nkになる。***  
  
よって、一つの工場につきN個のノードを作り、そのN個のノードと各オモチャとの間に、Z_ikを それぞれN, N-1, …, 1 倍したコストをもつ、キャパが1の辺を張ればよい。  
  
そして最小費用流を流す。  
フローの値は、おもちゃの個数に等しい、3つなら3。  
そうすると3本流れる。  

そうして流したときのコストが、求めるべきT。なのでNでわる。  


#  参考
<http://blog.csdn.net/qq978874169/article/details/70339287>
<https://amoshyc.github.io/ojsolution-build/poj/p3686.html>
[ベルマンフォード法](https://qiita.com/intatonix/items/8a50556697206ee89bcd)
[フローの話。蟻本のはdist(s,v)をh(v)に積算していくとか書いてある] (https://gist.github.com/satos---jp/711c056cb63af7fbccb569f98a086b06)
[これも同上](https://www.slideshare.net/Drafaer/kmc-advanced-3)
