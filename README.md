# Big Data Approximate Algorithms
整理几个大数据近似算法，涉及到随机采样、略图、数据流、滑动窗口算法等，包括布隆过滤器、Walker’s Alias Method、Count Sketch、FM Sketch、二部图判断的流算法、EH算法

## Bloom Filter（布隆过滤器）
布隆过滤器是一个很长的二进制向量和一系列随机映射函数，可以用于检索一个元素是否在一个集合中。它的优点是空间效率和查询时间都比一般的算法要好的多，缺点是有一定的误识别率和删除困难。

## Sampling（采样）
### Weight Sampling
介绍了Walker’s Alias Method，算法原理：
- 令pi = wi/W，排序 p1 <= … <= pn
- 预处理
  - 如果 p1 = 1/n, 结束.
  - 如果 p1 < 1/n, 从pn 中“倒出”1/n – p1 概率给p1 并移除p1
  - 重复以上步骤直到所有概率=1/n
  - 结束时，每个 1/n 概率都包含两个旧概率pi和pj（利用指针指向i和j，并存储pi在1/n概率中所占的百分比）
- 查询
  - 产生一个[1, n]之间的随机整数，找到某个1/n概率
  - 产生一个[0, 1/n]之间的随机数用于确定返回pi或者pj

## Sketch（略图）
- Count-Min Sketch [Cormode and Muthukrishnan 2005]
  - 频率估计、频繁项查询
  - AT&T使用Count-Min略图查询电话记录

- AMS Sketch [Alon et. al. 1999]
  - 频率矩近似、连接大小/点积大小近似

- Count Sketch [Charikar et. al. 2002]、
  - 频率矩近似、连接大小/点积大小近似[Rusu and Dobra 2007]
  - Google使用Count Sketch估计分布式系统中的频率

- FM Sketch [FM85]
  - 估计数据流中不同元素个数

### Count Sketch
算法原理：
- 给定误差参数𝜀
- 随机选取Universal哈希函数h: [m] ➝ [2/𝜀2], 2-wise independent哈希函数g: [m] ➝ {-1, +1} ，构建一个大小为W=2/𝜀2的哈希表

### FM Sketch
算法原理：
- Truly random哈希函数h: [m] ➝ [0,1]
- 新来元素i：𝑓𝑖←_𝑓_𝑖_+1
- 令X = min{X, h(i)}
- 用__𝐹__0_=_1_𝑋_−1来估计估计_𝐹_0_=_{_𝑓_𝑖_: _𝑓_𝑖_>0}_ 

## Data Stream（数据流）
### stream algorithm on bipartite graph
实现判断二部图的流算法。数据流中不断输入图的边，判断图是不是二部图，是二部图输出1算法继续，当非二部图时算法输出0并终止。
算法原理：
![1](https://github.com/BELIEVEfxy/Big-Data-Approximate-Algorithms/blob/master/stream%20algorithm%20on%20bipartite%20graph/1.png)

## 滑动窗口
### EH Algorithm


