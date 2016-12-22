#include <cstdio>
#include <stack>
#include <cstring>
#include <vector>
#include <cstdlib>
using namespace std;
#define INPUT_FILENAME "DAG_SP_test_command.txt"
const double inf = 1e+9;
struct Node {
    int u,v;
    double w;
    Node (){}
    Node (int _u,int _v,double _w):
        u(_u),v(_v),w(_w){}
};
enum COLOR {WHITE,GRAY,BLACK};
void go(vector<vector<Node> >& graph,int v) {
    printf("%d\n",v);
    for ( int i = 0 ; i < (int)graph[v].size() ; i++ ) 
        go(graph,graph[v][i].v);
}
bool visit(vector<vector<Node> > &graph,vector<int> &color,int now,vector<int>& cycle,vector<int>& path) {
    color[now] = GRAY;
    for ( int i = 0 ; i < (int)graph[now].size() ; i++ ) {
        if ( color[graph[now][i].v] == GRAY ) {
            cycle = path;
            return true;
        }
        path.push_back(graph[now][i].v);
        if ( color[graph[now][i].v] == WHITE && visit(graph,color,graph[now][i].v,cycle,path) ) {
            path.pop_back();
            return true;
        } else {
            path.pop_back();
        }
    }
    color[now] = BLACK;
    return false;
}
bool isCycle(vector<vector<Node> > &graph,vector<int>& color,vector<int>& cycle) {
    for ( int i = 0 ; i < (int)graph.size() ; i++ ) 
        if ( color[i] == WHITE ) {
            vector<int> path;
            path.push_back(i);
            if ( visit(graph,color,i,cycle,path) ) 
                return true;
        }
    return false;
}
void _topologicalSort(vector<vector<Node> > &graph,int now,vector<bool> visited,stack<int>& s) {
    visited[now] = true;
    for ( int i = 0 ; i < (int)graph[now].size() ; i++ ) 
        if ( !visited[graph[now][i].v] ) 
            _topologicalSort(graph,graph[now][i].v,visited,s);
    s.push(now);
}
void topologicalSort(vector<vector<Node> > &graph,stack<int>& s) {
    vector<bool> visited((int)graph.size(),false);
    for ( int i = 0 ; i < (int)graph.size() ; i++ ) 
        if ( visited[i] == false )
            _topologicalSort(graph,i,visited,s);
}
int main() {
    FILE* fp = fopen(INPUT_FILENAME,"r");
    int tc;
    fscanf(fp,"%d",&tc);
    while ( tc-- ) {
        char in[128],out[128];
        fscanf(fp,"%s %s",in,out);
        FILE *fp2 = fopen(in,"r");
        int V,E;
        fscanf(fp2,"%d %d",&V,&E);
        vector<vector<Node> > graph(V);
        while ( E-- ) {
            int u,v;
            double w;
            fscanf(fp2,"%d %d %lf",&u,&v,&w);
            graph[u].push_back(Node(u,v,w));
        }
        int source, sink;
        fscanf(fp2,"%d %d",&source,&sink);
        fclose(fp2);
        vector<int> color((int)graph.size(),WHITE);
        vector<int> cycle;
        if ( isCycle(graph,color,cycle) ) {
            printf("-1\n");
            for ( int i = 0 ; i < (int)cycle.size() ; i++ ) 
                printf("%d\n",cycle[i]);
        } else {
            printf("1\n");
            stack<int> s;
            topologicalSort(graph,s);
            vector<double> d((int)graph.size());
            for ( int i = 0 ; i <(int)graph.size() ; i++ ) 
                d[i] = inf;
            d[source] = 0;
            vector<int> ref((int)graph.size(),-1);
            while ( !s.empty() ) {
                int cur = s.top();s.pop();
                if ( d[cur] != inf ) {
                    for ( int i = 0 ; i < (int)graph[cur].size() ; i++ ) 
                        if ( d[graph[cur][i].v] > d[cur]+graph[cur][i].w ) {
                            d[graph[cur][i].v] = d[cur]+graph[cur][i].w;
                            ref[graph[cur][i].v] = cur;
                        }
                }
            }
            if ( d[sink] == inf ) {
                printf("0 0.0\n");
            } else {
                stack<int> f;
                for ( int now = sink ; now != -1 ; now = ref[now] ) 
                    f.push(now);
                printf("%d %lf\n",(int)f.size(),d[sink]);
                while ( !f.empty() ) {
                    printf("%d\n",f.top());
                    f.pop();
                }
            }
        }
        graph.clear();
    }
    fclose(fp);

    return 0;
}
