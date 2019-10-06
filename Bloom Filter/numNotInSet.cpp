#include<cstdio>
#include<cstring>
#include<iostream>
#include<fstream>
#include<set>

using namespace std;
set<int> numNotIn;
int main(){
	for(int i = 1; i <= 1000000; i++){
		numNotIn.insert(i);
	}
	cout << "ok1" << endl;
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
	 	numNotIn.erase(temp);
	}
	cout << "ok2" << endl;
	set<int>:: iterator iter1 = numNotIn.begin();
	set<int>:: iterator iter2 = numNotIn.end();
	ofstream out;
	out.open("numNotIn.txt");
	if(!out){
		cout << "fail to open document!" << endl;
		return 0;
	}
	for(; iter1 != iter2; iter1++){
		out << *iter1 << endl;
	}
	cout << "ok3" << endl;
	return 0;
}
