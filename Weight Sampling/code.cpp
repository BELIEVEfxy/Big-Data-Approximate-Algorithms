#include<cstdio>
#include<iostream>
#include<queue>
#include<vector>
#include<algorithm>
#include<fstream>
#include<string>
#include<iomanip>
#include<cstdlib>
#include<time.h>
#define random(a,b) (rand()%(b-a+1))+a

using namespace std;
const int CNT = 10000;//the total number of elements
struct element_small{//element in small heap
	int index;
	int index_i;
	double weight;
	friend bool operator < (element_small a, element_small b){
		return a.weight > b.weight;
	}
};
struct element_large{//element in large heap
	int index;
	int index_i;
	double weight;
	friend bool operator < (element_large a, element_large b){
		return a.weight < b.weight;
	}
};
struct element{//element in v_list
	int index_old;
	int index_new;
	double weight;
};
bool cmp(element a, element b){
	if(a.weight < b.weight)
		return true;
	return false;
}
vector<element> v_list;//the sequence of elements sorted by wi/W
priority_queue<element_small> q_small;//from small to large,small heap
priority_queue<element_large> q_large;//from large to small,large heap

int main(){
	srand((unsigned)time(NULL));
	string fileName("data9.txt");
	string fileOut("result.txt");
	ifstream in;
	ofstream out;
	//read data9.txt
	in.open(fileName.c_str());
	if(!in){
		cout << "Error in opening files" << endl;
		return 0;
	}
	int a;
	double b;
	double sumWeight = 0;//sum of wi
	struct element_large el;
	struct element_small es;
	struct element e;
	while(!in.eof()){
		in >> a >> b;
		e.index_old = a;
		e.weight = b;
		e.index_new = -1;
		v_list.push_back(e);
		sumWeight += b;
	}
	cout << "read 'data9.txt' finish!" << endl;
	in.close();
	//sort
	for(int i = 0; i < CNT; i++){
		v_list[i].weight = v_list[i].weight / sumWeight;//calculate wi/W and update v_list[i]/weight
	}
	sort(v_list.begin(), v_list.end(), cmp);
	cout << "sort finish!" << endl;
	//calculate average weight
	double averWeight = 1 / (double)CNT;//averWeight = 1 / n
    cout << "averWeight = " << averWeight << endl;

    //update small heap and large heap
	for(int i = 0; i < CNT; i++){
		if(v_list[i].weight < averWeight){//if wi < averWeight, insert it into small heap
			es.index = v_list[i].index_old;
			es.index_i = i;
			es.weight = v_list[i].weight;
			q_small.push(es);
		}else if(v_list[i].weight > averWeight){//if wi > averWeight, insert it into large heap
			el.index = v_list[i].index_old;
			el.index_i = i;
			el.weight = v_list[i].weight;
			q_large.push(el);
		}
	}

	//core section:update every element and make its weight be averWeight
	while(1){
		if(q_small.empty() || q_large.empty()){
			break;
		}

		es = q_small.top();//choose the smallest element
		q_small.pop();//remove the smallest element

		el = q_large.top();//choose the largest element
		q_large.pop();//remove the largest element

		el.weight -= (averWeight - es.weight);//give (1/n-pi) to the smallest one
		es.weight = averWeight;
		v_list[es.index_i].index_new = el.index_i;//add the largest one's index to the smallest's position of v_list[]
		v_list[el.index_i].weight = el.weight;
		if(el.weight - averWeight < 1e-16){//if wi is smaller than averWeight after updating, insert it into small heap
			struct element_small es_temp;
			es_temp.index_i = el.index_i;
			es_temp.index = el.index;
			es_temp.weight = el.weight;
			q_small.push(es_temp);
		}else if(el.weight - averWeight > 1e-16){//if wi is larger than averWeight after updating, still insert it into large heap
			q_large.push(el);
		}

	}
    //just to check v_list[] after updating
	out.open(fileOut.c_str());
	if(!out){
		cout << "Error in opening files" << endl;
		return 0;
	}
	for(int i = 0; i < CNT; i++){
		out << i << ": " << v_list[i].index_old << ' ' << v_list[i].weight << ' ';
		if(v_list[i].index_new == -1){
            out << "-1" << ' ' << "0" << endl;
		}else{
            out << v_list[v_list[i].index_new].index_old << ' ' << averWeight-v_list[i].weight << endl;
		}

	}
    cout << "write finish!" << endl;

    //select
    int ptr1 = random(0, CNT-1);//choose a random number between 0 and n-1
    double ptr2 = (double)(rand()/(double)RAND_MAX)/CNT;//choose a random number between 0 and 1/n
    cout << endl << "The selected number is : ";
    double ans;
    if(ptr2 < v_list[ptr1].weight){
    	ans = v_list[ptr1].index_old;
    }else{
    	ans = v_list[v_list[ptr1].index_new].index_old;
    }
    cout << ans << endl;
	return 0;
}
