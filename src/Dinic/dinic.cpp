#include <bits/stdc++.h>
#define IOSTREAM_BOOST true
using namespace std;

#define MAX_N 2 * 200 + 200
#define MAX_M 199 * 200

int n, m;

int source, sink;

pair<int, int> edgeTable[MAX_M];

struct Edge
{
    int id;
    int to;
    int capacityLeft()
    {
        if (id > 0)
            return edgeTable[id].second - edgeTable[id].first;
        else
            return edgeTable[-id].second + edgeTable[-id].first;
    }
    bool isResidual()
    {
        return capacityLeft() > 0;
    }
    void addFlow(int f)
    {
        if (id > 0)
            edgeTable[id].first += f;
        else
            edgeTable[-id].first -= f;
    }
};

vector<Edge> graph[MAX_N];

//bfs
int layer[MAX_N];
queue<int> q;

bool bfs()
{
    for (int i = 0; i < n; i++)
        layer[i] = -1;
    q.push(source);
    layer[source] = 0;
    while (!q.empty())
    {
        int v = q.front();
        q.pop();
        for (Edge e : graph[v])
            if (layer[e.to] == -1 && e.isResidual())
            {
                layer[e.to] = layer[v] + 1;
                q.push(e.to);
            }
    }
    return layer[sink] > -1;
}

//dfs

int nextEdge[MAX_N];

int dfs(int v, int flow)
{
    if (v == sink)
        return flow;
    for (; nextEdge[v] < graph[v].size(); nextEdge[v]++)
    {
        Edge &e = graph[v][nextEdge[v]];
        if (e.isResidual() && layer[e.to] == layer[v] + 1)
        {
            int sent = min(flow, e.capacityLeft());
            int result = dfs(e.to, sent);
            if (result)
            {
                e.addFlow(result);
                return result;
            }
        }
    }
    return 0;
}

int dinic()
{
    int flow = 0;
    while (bfs())
    {
        for (int i = 0; i < n; i++)
            nextEdge[i] = 0;
        while (true)
        {
            int newFlow = dfs(source, INT_MAX);
            if (newFlow)
                flow += newFlow;
            else
                break;
        }
    }
    return flow;
}

int main()
{
#if IOSTREAM_BOOST
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
#endif

    cin >> n >> m;
    source = 0;
    sink = n - 1;

    for (int i = 0; i < m; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        a--;
        b--;
        graph[a].push_back(Edge({i, b}));
        graph[b].push_back(Edge({-i, a}));
        edgeTable[i] = make_pair(0, c);
    }

    cout << dinic() << "\n";

    return 0;
}