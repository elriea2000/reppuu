制空値最適化ツール“烈風拳であります！” v0.1


＊概要
艦これの制空値の最適化が面倒なので作ってみました．
目標の制空値に届くような烈風の載せ方のパターンを提案します．
艦載機の機種は混ざられません．ほぼ烈風専用です．
＃実装したい気持ちは無くはない．でも面倒臭い．
烈風改で染める，なんてこともできますが，今の艦これの実装ではムリですね（笑）夢を見たい人はどうぞ．やりようによれば瑞雲計算機にもなります．瑞雲教の方はどうぞ．


＊使い方
1. 1回reppuu.exeをダブルクリックします．
2. 同フォルダにtmp.confができます．
3. 中身を編集してください．また，適当に名前を変えることをお勧めします．再度手順1をやるとtmp.confが上書きされてしまい，編集内容が消えてしまいます．
4. 作ったコンフィグファイルをreppuu.exeにドラッグ＆ドロップ．
5. 結果が表示されます．


＊結果表示の読み方
・上の方
艦載機数を最高熟練度の烈風を載せた時の制空値．参考までに．

・下の方
コンフィグで設定した数だけ候補が表示されます．一番下がおそらく最適．
使うスロット数が少ないほど正義．次に制空値が高い方が正義.

> ABCDEF G HHH / III

ABCDEF : それぞれの艦の搭載状況が16進数でビット表示されています．
例えば a20 と表示された場合，
一番目の艦の搭載状況は a = 1010 ，すなわち第1スロットと第3スロットに烈風を搭載した状態になります．
以下，
二番目の艦 2 = 0010 第3スロットのみ
三番目の艦 0 = 0000 搭載なし
念のため対応表は別節に記載しておきます．
電卓を起動して，表示＞プログラマにして，16進数にして，表示を打ち込めば分かりやすいかも．

G : 使う総スロット数．

HHH : 総制空値．

III : 上乗せした上での目標制空値．



＊コンフィグファイルの説明
コンフィグファイル中のコメントを読めば大体分かると思います．

・パラメータの記載順は変えないでください．
・イコール記号の後ろにパラメータを書けば読めるはず．
・パラメータへの全角文字は仕様外なので使わないでください．
・行頭を半角#にすればコメント行扱いになります．
・空行を使っても問題ありません．ただし，acnumの中に空行は入れてはいけません．やりたい場合は文頭#にしてください．
・outputlistname以外は，文末に#コメントをつけても大丈夫です．
・outputlistnameに入れる出力ファイル名は，スペース文字不許可です．
・marginrate，maxmarginrate以外の数字は小数点を入れないでください．

かなり手を抜いたので，ソース読める人は穴をついてもらって結構です(笑)


＊Tips
・あるスロットには載せなくないという場合は，該当スロットを0にすればいいかんじです．
　うちの嫁の第一第二を艦攻にしたい時に，0,0,6,3とすると，第三第四スロットしか使わなくなります．
・marginrateいらねーよ，って時には1.0に設定して，targetをお好きに設定するといいかんじです．


＊FAQ
・結果が表示されずにすぐ消えるんだけど
＞コンフィグファイルのpauseOnExitの値を1にしてみてください．

・エラー落ちしてるっぽい
＞たぶん何かのバグです．あいすまぬ．誰か直して．

・使いづらい
＞あいすまぬ．

・艦数を増やすと遅いんだけど
＞あいすまぬ．誰か高速化して．こちらの環境でも6隻で5秒程度かかってます．


＊16進数→2進数
0 -> 0000
1 -> 0001
2 -> 0010
3 -> 0011
4 -> 0100
5 -> 0101
6 -> 0110
7 -> 0111
8 -> 1000
9 -> 1001
a -> 1010
b -> 1011
c -> 1100
d -> 1101
e -> 1110
f -> 1111


＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊

＊動作環境
以下の環境で動作確認．
OS : Windows7
CPU : intel core i5
メモリ : 8 GB

ビルド時環境．
コンパイラ : Visual C++ compiler 16.00.40219.01 (VS2010のコンパイラ)

一応，クロスプラットフォームにプログラムは書いたつもりなので，C++をコンパイルできるならどのOSでも使えると思います．


＊更新履歴
v0.1 - 2016.05.06
とりあえず公開．


＊ライセンス
BSDで配布します．
即ち，これを使ったり改変したところで何かがあっても保障しません．それでもよければご自由に．

This source and related resources are available under BSD License. 

--
Copyright (c) 2016-, suzuryo
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.
* Neither the name of the <organization> nor the names of its contributors 
  may be used to endorse or promote products derived from this software 
  without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
