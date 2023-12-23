/**
 * @file BranBoundTSP.cpp
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief 分支限界法求解TSP问题
 * @date 2023-12-21
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <iomanip>
#include <time.h>
#include <math.h>

using namespace std;

const int N = 100;          // 最大基站数
const int INF = 0x3f3f3f3f; // 无穷大

int bestx[N];               // 记录最佳路径
double bestw = INF;         // 记录最佳路径长度
double down, up;            // 下界和上界
bool visited[N];            // 访问标记
double w[N][N];             // 距离矩阵
double w_min[N][2];         // 记录每个点到其他点的最小边和次小边
int n;                      // 基站数
int start;                  // 起始基站
int f[N];                   // 记录index和基站编号的对应关系
long long L = 0;            // 扫描过的搜索树结点总数
clock_t startTime, endTime; // 记录程序运行时间

struct Node
{
    bool visited[N];  // 访问标记
    vector<int> path; // 访问路径
    double l;         // 当前路径长度
    double lb;        // 目标值的下界
    bool operator<(const Node &a) const
    {
        return lb > a.lb;
    }
};

double dfs(int u, int cnt, double l) // u:当前节点，cnt:已访问节点数，l:当前路径长度
{
    // 贪心算法求上界
    visited[u] = true;
    if (cnt == n)
    {
        if (w[u][start] == INF) // 当前点和起始点之间没有边
        {
            visited[u] = false; // 回溯
            return INF;
        }
        return l + w[u][start];
    }
    double min, ret = INF;
    bool prev[N] = {false};
    int next;
    while (ret >= INF)
    {
        min = INF;
        next = 0;
        for (int i = 1; i <= n; i++)
        {
            if (!visited[i] && w[u][i] < min && !prev[i])
            {
                min = w[u][i];
                next = i;
            }
        }
        if (!next) // 当前点和所有未访问点之间没有边
        {
            visited[u] = false; // 回溯
            return INF;
        }
        prev[next] = true;
        ret = dfs(next, cnt + 1, l + min); // ret = INF时回溯
    }

    return ret;
}

void GetUp()
{
    up = dfs(start, 1, 0);
}

double GetDown(const Node &node)
{
    double ret = 2 * node.l; // 已经走过的路径长度的两倍

    if (node.path.size() >= 2)
    {
        double min = INF;
        for (int i = 1; i <= n; i++) // 从起始点到未访问点的最小边
        {
            if (!node.visited[i] && w[start][i] < min)
                min = w[start][i];
        }
        ret += min;
        min = INF;
        for (int i = 1; i <= n; i++) // 从当前路径最后一个点到未访问点的最小边
        {
            if (!node.visited[i] && w[node.path.back()][i] < min)
                min = w[node.path.back()][i];
        }
        ret += min;
    }

    // 进入/离开未遍历城市时，各未遍历城市带来的最小路径成本
    for (int i = 1; i <= n; i++)
        if (!node.visited[i])
            ret += w_min[i][0] + w_min[i][1];

    return ret / 2;
}

void BranBoundTSP()
{
    // 初始化起始节点
    Node node;
    node.l = 0;
    node.path.push_back(start);
    for (int i = 1; i <= n; i++)
        node.visited[i] = false;
    down = GetDown(node);
    node.lb = down;
    node.visited[start] = true;
    GetUp();
    cout << "下界：" << down << endl;
    cout << "上界：" << up << endl;

    if (down >= up)
        return; // 使用贪心法可以直接得到最优解

    // 将起始点加入优先队列
    priority_queue<Node> q;
    q.push(node);

    while (!q.empty())
    {
        Node t = q.top();
        q.pop();
        if (t.path.size() == n - 1) // 已经找到 n - 1 个点
        {
            L++;
            double lb_min; // 记录当前lb的最小值
            for (int i = 1; i <= n; i++)
            {
                if (!t.visited[i]) // 找到最后一个点
                {
                    t.l += w[t.path.back()][i];
                    lb_min = t.lb;
                    t.lb = t.l + w[i][start];
                    t.path.push_back(i);
                    break;
                }
            }
            if (t.lb < up)
            {
                up = t.lb; // 更新上界

                // 删除优先队列中所有lb大于当前上界的节点
                auto tp = q;
                q = {}; // 清空队列
                while (!tp.empty() && tp.top().lb <= up)
                {
                    q.push(tp.top());
                    tp.pop();
                }

                for (int i = 1; i <= n; i++) // 更新最佳路径
                    bestx[i] = t.path[i - 1];
                bestw = t.lb;       // 更新最佳路径长度
                if (t.lb <= lb_min) // 如果最佳路径长度小于等于当前lb的最小值，说明已经找到最优解
                    return;
            }
        }
        else
        {
            for (int i = 1; i <= n; i++)
            {
                if (!t.visited[i] && w[t.path.back()][i] != INF) // 未访问过且有边
                {
                    L++;
                    Node next = t;
                    next.l += w[t.path.back()][i];
                    next.path.push_back(i);
                    next.visited[i] = true;
                    next.lb = GetDown(next);
                    if (next.lb <= up) // 目标值的下界小于等于上界，才将其加入队列
                        q.push(next);
                }
            }
        }
    }
}

bool ReadData()
{
    string filename = "DATA/tsp_" + to_string(n) + ".txt";
    ifstream file(filename, ios::in);
    if (!file.is_open())
    {
        cerr << "Failed to open file." << endl;
        return false;
    }
    string line;
    int i = 1, j = 1;
    while (getline(file, line))
    {
        stringstream ss(line);
        int a, b;
        double c;
        string _a, _b, _c;
        ss >> _a >> _b >> _c;
        a = stoi(_a), b = stoi(_b), c = stod(_c);
        f[i] = a, f[j] = b; // 记录index和基站编号的对应关系
        if (a == 20)
            start = i;  // 记录起始基站index
        if (c == 99999) // a, b不相邻
            c = INF;
        w[i][j] = c;
        j++;
        if (j > n) // 一行读完
            i++, j = 1;
    }

    // 计算每个点到其他点的最小边和次小边
    for (int i = 1; i <= n; i++)
    {
        double min1 = INF, min2 = INF;
        for (int j = 1; j <= n; j++)
        {
            if (w[i][j] < min1)
            {
                min2 = min1;
                min1 = w[i][j];
            }
            else if (w[i][j] < min2)
            {
                min2 = w[i][j];
            }
        }
        w_min[i][0] = min1;
        w_min[i][1] = min2;
    }

    return true;
}

void PrintResult(int num)
{
    cout << "=========" << num << "=========" << endl;
    cout << "最佳路径为：";
    for (int i = 1; i <= n; i++)
        cout << f[bestx[i]] << " ";
    cout << f[bestx[1]] << endl;
    cout << "最佳路径长度为：" << fixed << setprecision(2) << bestw << endl;
    cout << "扫描过的搜索树结点总数为：" << L << endl;
    cout << "程序运行时间为：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
}

int main()
{
    n = 22;
    if (!ReadData())
        return 1;
    startTime = clock();
    BranBoundTSP();
    endTime = clock();
    PrintResult(n);
}