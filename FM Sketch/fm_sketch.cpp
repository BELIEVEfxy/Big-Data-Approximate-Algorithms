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
const int NUM = 50000;//Ԫ�صķ�Χ
const double eps = 0.01;
const double delta = 0.005;
const int big_prime = 1e9+7;//ÿ�������x�ķ�ΧΪ[1,big_prime]����x/big_prime��ʾ[0,1]֮��������
int W;
int d;
double **hashTable;
int **a;//��ϣ������ϵ��
int **b;//��ϣ�����ĳ���

void init(){//��ʼ��������W��d�Ĵ�С���������ڴ�
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
	for(int i= 0; i < d; i++){//��ϣ������ĳ�ʼֵΪ1����[0,1]�е����ֵ��
        for(int j = 0; j < W; j++){
            hashTable[i][j] = 1;
        }
	}
}

void Produce_hash(){//������ϣ����
    cout << "process hash function...." << endl;
	for(int i = 0; i < d; i++){
		for(int j = 0; j < W; j++){//��ÿһ��FM Sketch����һ����ϣ������ϵ���ͳ���
			a[i][j] = rand()%big_prime;
			b[i][j] = rand()%big_prime;
		}
	}
}

void FM_Sketch(int n){
	for(int i = 0; i < d; i++){
		for(int j = 0; j < W; j++){
			//�ڵ�[i][j]��FM Sketch�ж�����n���й�ϣh(n)
			long long int num=(long long int)a[i][j] * (long long int)n%big_prime + (long long int)b[i][j];
			double x = ((double)(num%big_prime)) / ((double)(big_prime));//x�ķ�ΧΪ[0,1]
			hashTable[i][j] = min(hashTable[i][j], x);//���бȽϣ�ѡ��С��
		}
	}
}

int Get_Ans(){//������
	double mean[d];
	for(int i = 0; i < d; i++){
		double w_mean = 0;
		for(int j = 0; j < W; j++){
			w_mean += hashTable[i][j];
		}
		mean[i]=((double)w_mean)/((double)(W));//��ÿһ��W��FM Sketch��ƽ����
	}
	sort(mean, mean+d);//����
	int ans = (int)((double)1 + (double)1 / mean[(d-1)/2]);//��d�е���λ����������ʵֵ
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
		s.insert(x);//ȥ�أ�����ʵԪ�ظ���
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
