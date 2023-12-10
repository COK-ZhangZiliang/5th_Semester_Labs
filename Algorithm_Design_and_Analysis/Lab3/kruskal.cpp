/**
 * @file kruskal.cpp
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief kruskal算法
 * @date 2023-12-10
 */
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cmath>
#include <algorithm>

using namespace std;

const int N = 100;
const int M = 10000;
const double INF = 0x3f3f3f3f;

typedef pair<int, int> PII;

struct Edge
{
    int a, b;
    double w;

    bool operator<(const Edge &W) const
    {
        return w < W.w;
    }
} edges[M];

int p[N];   // 并查集
int n, m;   // 点的数量，边的数量
PII mst[N]; // 存储最小生成树

int find(int x)
{
    if (p[x] != x)
        p[x] = find(p[x]);
    return p[x];
}

double kruskal()
{
    sort(edges, edges + m);
    double res = 0;
    int cnt = 0;
    for (int i = 1; i <= n; i++)
        p[i] = i; // 初始化并查集

    for (int i = 0; i < m; i++)
    {
        int a = edges[i].a, b = edges[i].b;
        double w = edges[i].w;

        int p_a = find(a), p_b = find(b);
        // 判断是否已经在一个连通图中
        if (p_a != p_b)
        {
            p[p_a] = p_b;
            res += w;
            mst[cnt] = {a, b};
            cnt++;
        }

        if (cnt == n - 1)
            return res;
    }

    return INF;
}

int main()
{
    // 读入22个基站的数据
    n = 22;
    m = 0;
    ifstream file("DATA/mst_22.txt", ios::in);
    if (!file.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        int a, b, a_id, b_id;
        double c;
        string _a, _b, _a_id, _b_id, _c;
        ss >> _a >> _b >> _a_id >> _b_id >> _c;
        a = stoi(_a), b = stoi(_b), a_id = stoi(_a_id), b_id = stoi(_b_id), c = stod(_c);
        if (c > 0)
            edges[m++] = {a, b, c};
    }

    double res = kruskal();
    cout << "===========22===========\n";
    if (fabs(res - INF) < 1e-6)
        puts("Impossible!");
    else
    {
        cout << "Cost: " << fixed << setprecision(2) << res << endl;
        int cnt = 0;
        for (int i = 0; i < n - 1; i++)
        {
            cout << setw(6) << left << to_string(mst[i].first) + "-" + to_string(mst[i].second);
            if (++cnt % 4 == 0)
                cout << endl;
        }
    }
    cout << endl
         << endl;

    // 将最小生成树的结果写入文件
    ofstream file2("RESULT/kruskal_22_res.txt", ios::out);
    if (!file2.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }
    for (int i = 0; i < n - 1; i++)
        file2 << mst[i].first << " " << mst[i].second << "\n";

    // 读入42个基站的数据
    n = 42;
    m = 0;
    ifstream file3("DATA/mst_42.txt", ios::in);
    if (!file3.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }
    while (getline(file3, line))
    {
        stringstream ss(line);
        int a, b, a_id, b_id;
        double c;
        string _a, _b, _a_id, _b_id, _c;
        ss >> _a >> _b >> _a_id >> _b_id >> _c;
        a = stoi(_a), b = stoi(_b), a_id = stoi(_a_id), b_id = stoi(_b_id), c = stod(_c);
        if (c > 0)
            edges[m++] = {a, b, c};
    }

    res = kruskal();
    cout << "===========42===========\n";
    if (fabs(res - INF) < 1e-6)
        puts("Impossible!");
    else
    {
        cout << "Cost: " << fixed << setprecision(2) << res << endl;
        int cnt = 0;
        for (int i = 0; i < n - 1; i++)
        {
            cout << setw(6) << left << to_string(mst[i].first) + "-" + to_string(mst[i].second);
            if (++cnt % 4 == 0)
                cout << endl;
        }
    }
    cout << endl;

    // 将最小生成树的结果写入文件
    ofstream file4("RESULT/kruskal_42_res.txt", ios::out);
    if (!file4.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }
    for (int i = 0; i < n - 1; i++)
        file4 << mst[i].first << " " << mst[i].second << "\n";

    return 0;
}
