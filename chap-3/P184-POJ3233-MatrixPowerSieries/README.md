# POJ3233
（問題文略）  

## (より)数学的解法
Iをn×n単位行列として  
S_k = I + A + ... + A^(k-1)  
と表すと、  
行列の漸化式が立てられる。  
<img src= "https://www.codecogs.com/eqnedit.php?latex=\binom{A^k}{S_k}&space;=&space;\begin{pmatrix}&space;A&space;&&space;0&&space;\\&space;I&space;&&space;I&&space;\end{pmatrix}^k&space;\binom{1}{0}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\binom{A^k}{S_k}&space;=&space;\begin{pmatrix}&space;A&space;&&space;0&&space;\\&space;I&space;&&space;I&&space;\end{pmatrix}^k&space;\binom{1}{0}" title="\binom{A^k}{S_k} = \begin{pmat<a href="https://www.codecogs.com/eqnedit.php?latex=\binom{A^k}{S_k}&space;=&space;\begin{pmatrix}&space;A&space;&&space;0&&space;\\&space;I&space;&&space;I&&space;\end{pmatrix}^k&space;\binom{1}{0}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\binom{A^k}{S_k}&space;=&space;\begin{pmatrix}&space;A&space;&&space;0&&space;\\&space;I&space;&&space;I&&space;\end{pmatrix}^k&space;\binom{1}{0}" title="\binom{A^k}{S_k} = \begin{pmatrix} A & 0& \\ I & I& \end{pmatrix}^k \binom{1}{0}" />
みたいになるっぽい。by蟻本。  
  
でもこのやり方、自分じゃ思いつかなそう。  
よって次の解法も書いておく。これも結局自分では思いつかなかったけど...。


## より自然に思いつきやすそうな解法
下記の参考URLのサイトから。  
S = A + A^2 + A^3 + A^4 + A^5  
というものを考える。  
S = I( A + A^2) + A^2(A+A^2) + A^5  
と分割できる。  
さらにカッコの中身は
(A + A^2) = A + A(A)  
というふうに、Aまで分割できる。
  
A + A^2 + ... + A^k  
をmemo[k]として記録し、メモ化再帰を行えば良い。  
(参考サイトによるとこれだけではTLEするらしい。よって他にも工夫必要だとか。)  

## 

# 参考
<http://area.hateblo.jp/entry/2017/01/28/023755>
