#include<cstdio>
#include<string>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<fstream>
#include<cstring>
#include<time.h>
#include<map>
#include<vector>

#define random(x) (rand() % x)

using namespace std;

class BloomFilter{
private:
	double pError;												//false positive rate
	int numHash;												//the number of hash function
	int numBit;													//the number of bits in BitMap
	int numNum;													//the number of tested set, equal to 10000 constantly
	bool *bitMap;												//BitMap
	vector<int> randNum1;                                       //the first set storing rand numbers
	vector<int> randNum2;										//the first set storing rand numbers
	int prime;													//a prime number
	//void selectPrime();										//select a prime
	void initRandNum();											//initialize randNum1 and randNum2
	long long int IntHashFunction(long long int key);			//hash function
public:
	BloomFilter(double pError){
		numNum = 10000;
		double m_n;												//=numBit / numNum
		m_n = -1 * (log(pError)) / (log(2)*log(2));
		numBit = ceil(m_n * numNum);
		numHash = ceil(0.7 * m_n);
		bitMap = new bool[numBit]{false};
	}
	~BloomFilter(){
        delete []bitMap;
	}
	void initBloomFilter(){										//initialize Bloom Filter
		//this -> selectPrime();
		this -> initRandNum();
	}
	void addToBloomFilter(int num);								//insert elements into bitMap
	bool queryInBloomFilter(int num);							//query a given number whether it is in the set
	void printBitMap();											//print the bitMap
};

void BloomFilter::initRandNum(){								//select two random number from (0,67) and (0,23) as a pair, the total number is 'numHash'
	for(int i = 0; i < numHash; i++){
		int a = random(67);
		randNum1.push_back(a);
		for(int j = 1; j < 10; j++){
			a = random(67);
		}
		a = random(23);
		randNum2.push_back(a);
	}
}
long long int BloomFilter::IntHashFunction(long long int key){ //a kind of hash function used in Redis aims to transfer 32-bits integers.
	key += ~(key << 15);
    key ^= (key >> 10);
    key += (key << 3);
    key ^= (key >> 6);
    key += ~(key << 11);
    key ^= (key >> 16);
    return key;
}
void BloomFilter::addToBloomFilter(int num){
	for(int i = 0; i < numHash; i++){							//compute indexes
		long long int index;
		long long int key = ((randNum1[i] * num) + randNum2[i]);//linear function:num->key
		//index = key ^ (key >> 18);
		//index = ((randNum1[i] * num)%prime + randNum2[i]) + (num<<11);
		//index = (((randNum1[i] * num) + randNum2[i])+0xd3a2646c) ^ (((randNum1[i] * num) + randNum2[i])<<9);
		//index = (num+0xd3a2646c) ^ (num<<9);
        index = IntHashFunction(key);
		index %= numBit;
		bitMap[index] = true;
	}
}
bool BloomFilter::queryInBloomFilter(int num){
	bool flag = false;
	for(int i = 0; i < numHash; i++){
        long long int index;
        long long int key = ((randNum1[i] * num) + randNum2[i]);
        index = IntHashFunction(key);
		index %= numBit;
		if(bitMap[index] == false){
			flag = true;
			break;
		}
	}
	if(flag == false){
		return true;
	}else{
		return false;
	}
}
void BloomFilter::printBitMap(){
	ofstream out;
	out.open("BitMap.txt");
	if(!out){
		cout << "fail to open document!" << endl;
		return;
	}
	for(int i = 0; i < numBit; i++){
		if(bitMap[i] == true){
            out << '1' << ' ';
		}else{
            out << '0' << ' ';
		}
	}
	return;
}

int main(){
	 srand((int)time(0));

	 double p;
	 cout << "please enter the false positive rate:" << endl;
	 cin >> p;
	 BloomFilter bf(p);
     bf.initBloomFilter();									//initialize object bf.

	 string filename("data3.txt");
	 ifstream in;
	 in.open(filename.c_str());
	 if(!in){
	 	cout << "fail to open document!" << endl;
	 	return 0;
	 }
	 int temp = 0, t;
	 while(!in.eof()){
	 	in >> temp;
	 	bf.addToBloomFilter(temp);
	 }
	 in.close();
	 cout << "in ok!" << endl;

	 bf.printBitMap();
	 cout << "print ok!" << endl;

	 in.open("numNotIn.txt");							//in this txt:I stored the numbers which are not in the testd set.
	 if(!in){
	 	cout << "fail to open document!" << endl;
	 	return 0;
	 }

	 int conError = 0;									//the number of faults
	 int numOfNotIn = 0;								//total numbers in "numNotIn.txt"
	 while(!in.eof()){
        numOfNotIn++;
	 	int numQuery;
	 	in >> numQuery;
	 	if(numQuery == -1){
	 		break;
	 	}
	 	bool check = bf.queryInBloomFilter(numQuery);
	 	if(check == true){
	 		conError++;
	 	}
	 }
	 double falseRate = (double)conError / ((double)numOfNotIn-1);//cmopute the actual fault rate.
     cout << "the actual false rate is:";
	 cout << falseRate << endl;

	 while(1){
	 	cout << "please enter the number you want to query:(enter -1 to exit)" << endl;
	 	int numQuery;
	 	cin >> numQuery;
	 	if(numQuery == -1){
	 		break;
	 	}
	 	if(bf.queryInBloomFilter(numQuery)){
            cout << numQuery << " is in the set!" << endl;
	 	}else{
            cout << numQuery << " is not in the set!" << endl;
	 	}
	 }
	 return 0;
}

/*void BloomFilter::selectPrime(){
	for(int num = numBit; ;num++){
		if(num % 6 != 1 && num % 6 != 5){
			continue;
		}
		int s;
		s = sqrt(num);
		bool flag = false;
		for(int i = 5; i <= s; i += 6){
			if(num%i == 0 || num%(i+2) == 0){
				flag = true;
				break;
			}
		}
		if(flag == false){
			prime = num;
			return ;
		}
	}
}*/
