#include<cstdio>
#include<iostream>
#include<algorithm>
#include<vector>
#include<fstream>
#include<cmath>
#include<cstdlib>
#include<time.h>
#define random(a,b) (rand()%(b-a+1))+a; //rand number in [a,b]

using namespace std;
const int NUM = 1000;
const double eps = 0.001;
const double delta = 0.00001;
const int prime1 = 379;//prime number
const int prime2 = 509;
const int prime = 911;
const int big_prime = 1e9+7;
int hashSize;//size of one hash table
int d;//the number of hash function
int **hashTable;
int **gTable;
vector<int> a;
vector<int> b;
void Process_g(){
    cout << "process g hash function...." << endl;
	gTable = new int *[d];
	for(int i = 0; i < d; i++){
		gTable[i] = new int[NUM];
	}
	hashTable = new int *[d];
	for(int i = 0; i < d; i++){
        hashTable[i] = new int[hashSize];
	}
	for(int i= 0; i < d; i++){
        for(int j = 0; j < hashSize; j++){
            hashTable[i][j] = 0;
        }
	}

	for(int i = 0; i < d; i++){
		for(int j = 0; j < NUM; j++){
			int r = random(1, 10000);
			if(r <= 5000){
				gTable[i][j] = -1;
			}else{
				gTable[i][j] = 1;
			}
		}
	}
}

void Produce_hash(){
    cout << "process hash function...." << endl;
	for(int i = 0; i < d; i++){
		int temp_a = rand()%big_prime;
		int temp_b = rand()%big_prime;
		a.push_back(temp_a);
		b.push_back(temp_b);
	}
}

void init(){
	hashSize = (int)((double)3 / (double)(eps * eps));
	d = log(double(1)/delta);
	cout << "hashSize: " << hashSize << " d: " << d << endl;
	Process_g();
	Produce_hash();
}

void Count_Sketch(int x, int y){
	for(int i = 0; i < d; i++){
		long long int index = (long long int)a[i] * (long long int)x%big_prime + (long long int)b[i];
		index = (index % big_prime) % hashSize;
		hashTable[i][(int)index] += gTable[i][x] * y;
	}
}

int Find_fi(int n){
	int ans[d];
	for(int i = 0; i < d; i++){
		long long int index = (long long int)a[i] * (long long int)n%big_prime + (long long int)b[i];
		index = (index % big_prime) % hashSize;
		ans[i] = hashTable[i][(int)index] * gTable[i][n];
	}
	sort(ans, ans+d);
	return ans[(d-1)/2];
}

int main(){
	srand((unsigned) time(NULL));
	init();
	string file_name("data.txt");
	ifstream in;
	in.open(file_name.c_str());
	if(!in){
		cout << "Error in opening data.txt" << endl;
		return 0;
	}
	int x,y;
	while(!in.eof()){
		in >> x >> y;
		Count_Sketch(x, y);
	}
	cout << "read finish!" << endl;
	in.close();
	ofstream out;
    out.open("result.txt");
    if(!out){
        cout << "Error in opening result.txt" << endl;
        return 0;
    }
    for(int i = 0;i < NUM; i++){
        int result = Find_fi(i);
        out << i << ' ' << result << endl;
    }
	int n;
	while(1){
        cout << "please enter the number you want to query:(-1 for exit)" << endl;
        cin >> n;
        if(n == -1){
            break;
        }
        if(n<0||n>999){
            cout << "Wrong number! Please enter again!" << endl;
            continue;
        }
        int ans = Find_fi(n);
        cout << "the frequency of this element is:" << ans << endl;
	}

	delete []gTable;
	delete []hashTable;
	return 0;
}
