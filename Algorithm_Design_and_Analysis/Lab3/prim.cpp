/**
 * @file prim.cpp
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief prim算法
 * @date 2023-12-10
 */
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cmath>

using namespace std;

const int N = 100;
const double INF = 0x3f3f3f3f;

double g[N][N], dist[N]; // 邻接矩阵存储所有边
bool st[N];              // 判断点是否已经加入最小生成树
int mst[N];              // 存储最小生成树
int n;                   // 点的数量

double prim()
{
    memset(mst, -1, sizeof mst);
    memset(st, 0, sizeof st);
    for (int i = 1; i <= n; i++)
        dist[i] = INF;
    double res = 0;

    for (int i = 0; i < n; i++)
    {
        int t = -1;
        for (int j = 1; j <= n; j++)
            if (!st[j] && (t == -1 || dist[t] > dist[j]))
                t = j;

        // 图不连通
        if (i && fabs(dist[t] - INF) < 1e-6)
            return INF;

        // 加上树边权重
        if (i)
            res += dist[t];

        st[t] = true;

        // 更新其他点到最小生成树的距离
        for (int j = 1; j <= n; j++)
        {
            if (dist[j] > g[t][j] && !st[j])
            {
                dist[j] = g[t][j];
                mst[j] = t;
            }
        }
    }

    return res;
}

int main()
{
    // 读入22个基站的数据
    n = 22;
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
            g[a][b] = c;
        else
            g[a][b] = INF;
    }

    double res = prim();
    cout << "===========22===========\n";
    if (fabs(res - INF) < 1e-6)
        puts("Impossible!");
    else
    {
        cout << "Cost: " << fixed << setprecision(2) << res << endl;
        int cnt = 0;
        for (int i = 2; i <= n; i++)
        {
            cout << setw(6) << left << to_string(i) + "-" + to_string(mst[i]);
            cnt++;
            if (cnt % 4 == 0)
                cout << endl;
        }
    }
    cout << endl
         << endl;

    // 将最小生成树的结果写入文件
    ofstream file2("RESULT/prim_22_res.txt", ios::out);
    if (!file2.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }
    for (int i = 2; i <= n; i++)
        file2 << mst[i] << "\n";

    // 读入42个基站的数据
    n = 42;
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
            g[a][b] = c;
        else
            g[a][b] = INF;
    }

    res = prim();
    cout << "===========42===========\n";
    if (fabs(res - INF) < 1e-6)
        puts("Impossible!");
    else
    {
        cout << "Cost: " << fixed << setprecision(2) << res << endl;
        int cnt = 0;
        for (int i = 2; i <= n; i++)
        {
            cout << setw(6) << left << to_string(i) + "-" + to_string(mst[i]);
            cnt++;
            if (cnt % 4 == 0)
                cout << endl;
        }
    }
    cout << endl;

    // 将最小生成树的结果写入文件
    ofstream file4("RESULT/prim_42_res.txt", ios::out);
    if (!file4.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }
    for (int i = 2; i <= n; i++)
        file4 << mst[i] << "\n";

    return 0;
}
