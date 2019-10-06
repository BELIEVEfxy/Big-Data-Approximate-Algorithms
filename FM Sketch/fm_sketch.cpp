#include<cstdio>
#include<iostream>
#include<algorithm>
#include<vector>
#include<fstream>
#include<cmath>
#include<cstdlib>
#include<set>
#include<time.h>
#define random(a,b) (rand()%(b-a+1))+a; //rand number in [a,b]
//max number 50000
using namespace std;
const int NUM = 50000;//元素的范围
const double eps = 0.01;
const double delta = 0.005;
const int big_prime = 1e9+7;//每个随机数x的范围为[1,big_prime]，用x/big_prime表示[0,1]之间的随机数
int W;
int d;
double **hashTable;
int **a;//哈希函数的系数
int **b;//哈希函数的常数

void init(){//初始化，计算W和d的大小，并分配内存
	W = (int)((double)24 / (double)(eps * eps));
	d = log(double(1)/delta);
	cout << "W=" << W << endl;
	cout << "d=" << d << endl;
	hashTable = new double *[d];
	a = new int *[d];
	b = new int *[d];
	for(int i = 0; i < d; i++){
        hashTable[i] = new double[W];
        hashTable[i] = new double[W];
        a[i] = new int[W];
        b[i] = new int[W];
	}
	for(int i= 0; i < d; i++){//哈希表数组的初始值为1，是[0,1]中的最大值。
        for(int j = 0; j < W; j++){
            hashTable[i][j] = 1;
        }
	}
}

void Produce_hash(){//产生哈希函数
    cout << "process hash function...." << endl;
	for(int i = 0; i < d; i++){
		for(int j = 0; j < W; j++){//对每一个FM Sketch产生一个哈希函数的系数和常数
			a[i][j] = rand()%big_prime;
			b[i][j] = rand()%big_prime;
		}
	}
}

void FM_Sketch(int n){
	for(int i = 0; i < d; i++){
		for(int j = 0; j < W; j++){
			//在第[i][j]个FM Sketch中对数字n进行哈希h(n)
			long long int num=(long long int)a[i][j] * (long long int)n%big_prime + (long long int)b[i][j];
			double x = ((double)(num%big_prime)) / ((double)(big_prime));//x的范围为[0,1]
			hashTable[i][j] = min(hashTable[i][j], x);//进行比较，选择小的
		}
	}
}

int Get_Ans(){//计算结果
	double mean[d];
	for(int i = 0; i < d; i++){
		double w_mean = 0;
		for(int j = 0; j < W; j++){
			w_mean += hashTable[i][j];
		}
		mean[i]=((double)w_mean)/((double)(W));//求每一行W个FM Sketch的平均数
	}
	sort(mean, mean+d);//排序
	int ans = (int)((double)1 + (double)1 / mean[(d-1)/2]);//求d行的中位数，估计真实值
	return ans;
}

int main(){
	srand((unsigned) time(NULL));
	init();
	Produce_hash();
	string file_name("data11.txt");
	ifstream in;
	in.open(file_name.c_str());
	if(!in){
		cout << "Error in opening data.txt" << endl;
		return 0;
	}
	int x;
	set<int> s;
	int cnt = 1;
	while(!in.eof()){
		in >> x;
		if(cnt%100==0)
            cout << cnt << ' ';
		FM_Sketch(x);
		s.insert(x);//去重，求真实元素个数
		cnt++;
	}
	cout << "read finish!" << endl;
	in.close();

	cout << endl << "The real number of dataset is " << s.size() << endl;
	int ans = Get_Ans();
	cout << "The answer of FM Sketch is " << ans << endl;
	delete[] hashTable;
	delete[] a;
	delete[] b;
	return 0;
}
