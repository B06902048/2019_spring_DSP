## B06902048	資工二	李峻宇

1. Environment : 這次作業我都是在 CSIE Workstation 寫和執行的。

2. How to Execute : Makefile 會把 train.c 和 test.c 編譯成 train 和 test 。 這兩個檔案的執行方式和作業的PDF檔要求一樣。

3. 在這次作業中，透過實作讓我更了解了HMM到底是什麼東西，不過可惜的是我們做的是 discrete 而非 continuous 。在寫作業的時候我發現，那些 pi, a, b, alpha, ...的製作過程就跟演算法課學到的 DP 很像，這樣想像其實會覺得 HMM 其實沒那麼難，而且非常厲害。

   另外我覺得很有趣的地方是，當我們的 iteration 跑到 10 次，做出來的 model 其實會看到 pi, a, b 裡面有些項為零，如果 iteration 到 1000 次，為零的項又更多了。我想也就代表這個聲音的"特色"吧。

   這次作業有用到一點機器學習的技巧，不免就讓人想試試看不同的 iteration 做出來的   testing_data1.txt 的正確率是如何。我發現，iteration設定為10的時候，正確定大約只有54%，但是iteration設定到50時，正確率可以達到82.4%，接下來就卡關了，設定為100時約有84%，設定為500時約有85%，設定到1000時可以到87%，再高的數字會使得正確率降低(或是說浮動？)。簡單來說我自己嘗試了很多數字，iteration為1000時就是最高了，但其實也才比設定為50多了5%左右。