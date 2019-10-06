·布隆过滤器的程序为bloom_filter.cpp
·测试数据为“data3.txt”和“numNotIn.txt”
·其中“data3.txt”我将前几行的说明文字删除了；“numNotIn.txt”是由"numNotInSet.cpp"程序生成的所有不在数据集但是在(1,1000000)范围内的数字，用于测试计算实际的误判率，和给定的误判率进行比较，来检验程序的性能。也提供了可用于查询检验的测试用例。