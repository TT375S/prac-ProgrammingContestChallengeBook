# P188 最大通信量
ネットワークで繋がれた何個かのノード(コンピュータ)がある。ノード間は一方向に通信できる通信路で結ばれており、流せる最大量が通信路ごとにきめられている。ある特定のノードから特定のノードへ、流せる最大のデータ量を求めたい...という問題。  
蟻本にはどういう形で問題が与えられるか書いてないので、スタートノードs、ターゲットノードt、ノード数v、隣接行列が与えられるとした。  
  
Ford-Fulkerson法というのを素直に使えば良い。(参考URL参照)  
残余ネットワークとかいう「現状で流すことができる量」をコストとした有効グラフを考える(by参考URL1)  
もともとは問題では一方向にしか辺がなかったが、残余ネットワークでは「現在流している量」をコストとした逆向きの辺も持つ。  
  
sからtへ至るパスをDFSで計算し、そのパスの流量はパス上の一番容量が小さい通信路になる(ネックになる)。  
そうしたら、パス上の辺から流量分減らす。  
コストが0のときは、その辺は通れないとする。  
(なお、「現在流している量」をコストとした辺を通るときは、その辺からは流している量を減らすという意味になる。)  
このようにして、流せるパスがなくなるまで繰り返す。

#  参考
[Ford-Fulkerson法](http://even-eko.hatenablog.com/entry/2013/08/08/195120)  
[最小カット](http://even-eko.hatenablog.com/entry/2013/08/08/224430)  
[Dinic法1](http://topcoder.g.hatena.ne.jp/Mi_Sawa/20140311/1394730336)  


