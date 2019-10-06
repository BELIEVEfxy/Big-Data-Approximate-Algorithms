#define push_back pb
#define make_pair mk
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <set>
using namespace std;

#define N 12

int leader[N + 4] = {-1}; // 先搞一个充分大的数组
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int,int> PII;
typedef pair<double,int>PDI;
const double PI=acos(-1);
const int maxn=128;
const int maxm=128;
const int INF = 0x3f3f3f3f;
using namespace std;
struct node{
    int to,cost;
};

vector<node>g[maxn];
int d[maxn],pa[maxn][30],dep[maxn];

//初始化
void makeSet(){
    for(int i = 0; i < N; i++){
        leader[i] = i; //初始化时,将自己初始化为自己的领导
    }
}
//查找代表元素
int findFather(int n){
    int r = n;
    while(leader[r] != r){
        r = leader[r]; //没找到的话，一直往上找
    }
    return r;
}
//合并
void unitSet(int leaderX, int leaderY){
    leader[leaderX] = leaderY;
}
void dfs(int u, int fa, int deep, int dis){
    d[u] = dis;
    pa[u][0] = fa;
    dep[u] = deep;
    for(int i = 0; i < g[u].size(); i++){
        int v = g[u][i].to;
        int w = g[u][i].cost;
        if(v == fa)
            continue;
        dfs(v,u,deep+1,dis+w);
    }
}
void build(){
    for(int i = 0; i < 12; i++){
        for(int j = 0; j < N; j++){
            if(pa[j][i] < 0)
                pa[j][i+1] = -1;
            else
                pa[j][i+1] = pa[pa[j][i]][i];
        }
    }
}
//寻找共同祖先，倍增法
int LCA(int x,int y){
    if(dep[x] > dep[y])
        swap(x,y);
    for(int i = 0; i <= 12; i++)
        if((dep[y]-dep[x])>>i&1)
            y=pa[y][i];
    if(x == y){
        return x;
    }
    for(int i = 12; i >= 0; i--){
        if(pa[x][i] != pa[y][i]){
            x=pa[x][i];
            y=pa[y][i];
        }
    }
    return pa[x][0];
}
//判断是否存在奇数环，若加入的边的两个顶点之间的距离为偶数，则新加入边后形成奇数环
bool oddCircle(int u, int v){
    dfs(0,1,1,0);
    build();
    int res = LCA(u,v);
    int dis = d[u] + d[v] - 2*d[res];
    if(dis%2 == 0){
        return true;
    }
    return false;
}

int main(){
    makeSet();
    ifstream in;
    in.open("data.txt");
    if(!in){
        cout << "Error in" << endl;
        return 0;
    }
    int u, v;
    int X = 1;
    for(int i=0; i< N; i++) g[i].clear();
    while(!in.eof()){
        in >> u >> v;
        if(X == 1){
            if(findFather(u) != findFather(v)){//若边不在一个集合里，则不会形成环，加入，并输出X
                //加入边
                int w = 1;
                g[u].pb(node{v,w});
                //并查集合并
                unitSet(u, v);
            }else if(oddCircle(u,v) == true){//如果形成奇数环，则不加入该边，X=0
                X = 0;
            }
        }
        cout << X << endl;
    }
    return 0;
}
