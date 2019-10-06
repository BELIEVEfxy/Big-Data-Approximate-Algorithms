#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <cmath>
#include <stack>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
using namespace std;

const int N = 50000;//窗口大小
const int k = log(N)/log(2);//桶的个数

struct EH_bucket{
    int startstamp;//初始时间戳
    int endstamp;//终止z时间戳
    int eps;//1的个数的次幂
};

vector<EH_bucket> v;
int timestamp;//时间戳

void Produce_Data(){//产生1000000数据的数据流
    cout << "Making data stream......" << endl;
    ofstream out;
    out.open("data.txt");
    if(!out){
        cout << "Error out" << endl;
        return;
    }
    int NUM = 1000000;
    srand((unsigned)time(NULL));
    int cnt = 0;
    for(int i = 0;i < NUM; i++){
        int x = rand()%10000;
        if(x <= 3000)
            out << 0 << endl;
        else
            out << 1 << endl;
        if(i < N && x > 3000){
            cnt += 1;
        }
    }
    cout << "The true answer is:" << cnt << endl;
    out.close();
}

void Update(){//新来一个元素，更新桶
    if(v.size() <= 2){//此时只有两个大小为1的桶
        return;
    }
    int cnt[N];//记录大小为i的桶的个数
    memset(cnt,0,sizeof(cnt));
    for(int i = 0;i <= k; i++){
        for(int j = v.size()-1; j >=0 ; j--){
            if(v[j].eps == i){
                cnt[i]++;
            }
            else if(v[j].eps > i)
                break;
        }
        if(cnt[i] > 2){//相同桶的个数超过2，合并旧的两个桶
            stack <EH_bucket> s;
            while(!s.empty())
                s.pop();
            while(v.back().eps <= i){
                EH_bucket temp = v.back();
                v.pop_back();
                s.push(temp);
            }
            EH_bucket first = s.top();
            s.pop();
            EH_bucket second = s.top();
            s.pop();
            
            EH_bucket new_bucket;
            new_bucket.startstamp = first.startstamp;
            new_bucket.endstamp = second.endstamp;
            new_bucket.eps = first.eps + 1;
            s.push(new_bucket);
            while(!s.empty()){
                v.push_back(s.top());
                s.pop();
            }
        }
        
    }
    if(v.front().endstamp < timestamp - N){//若最大桶超过窗口大小，则去掉最大桶
        v.erase(v.begin());
    }
}

int Cal_ans(int x){
    int ans = 1;
    for(int i = 0;i < x; i++){
        ans = ans * 2;
    }
    return ans;
}

int EH(){
    ifstream in;
    in.open("data.txt");
    if(!in){
        cout << "Error in" << endl;
        return 0;
    }
    int x;
    timestamp = 0;
    while(!in.eof()){
        in >> x;
        timestamp ++;
        if(x == 0){
            continue;
        }
        else if(x == 1){
            EH_bucket bucket;//新设一个桶，储存初始时间戳、终止时间戳、和1个数
            bucket.startstamp = timestamp - 1;
            bucket.endstamp = timestamp;
            bucket.eps = 0;
            v.push_back(bucket);
        }
        Update();//更新桶
    }
    int ans = Cal_ans(v.front().eps)/2;
    for(int i = 1;i < v.size(); i++){
        ans += Cal_ans(v[i].eps);
    }
    in.close();
    return ans;
}

int main(){
    Produce_Data();
    cout << "The answer of EH algorithm is:" << EH() << endl;
    return 0;
}
