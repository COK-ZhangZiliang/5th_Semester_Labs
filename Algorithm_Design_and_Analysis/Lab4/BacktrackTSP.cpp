/**
 * @file BacktrackTSP.cpp
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief 回溯法求解TSP问题
 * @date 2023-12-21
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <iomanip>

using namespace std;

const int INF = 0x3f3f3f3f;
const int N = 100; // 最大基站数

int bestx[N];               // 记录最佳路径
double bestw = INF;         // 记录最佳路径长度
double cw = 0;              // 当前部分路径长度
int x[N];                   // 当前部分路径
double w[N][N];             // 邻接矩阵存储基站间距离
int n;                      // 基站数
int f[N];                   // 记录index和基站编号的对应关系
int start;                  // 记录起始基站index
long long L = 0;                  // 扫描过的搜索树结点总数
clock_t startTime, endTime; // 记录程序运行时间

void BacktrackTSP(int i)
{
    if (i == n) // 搜索到叶节点
    {
        if (w[x[n - 1]][x[n]] != INF && w[x[n]][start] != INF && cw + w[x[n - 1]][x[n]] + w[x[n]][start] < bestw)
        {
            for (int j = 1; j <= n; j++)
                bestx[j] = x[j];
            bestw = cw + w[x[n - 1]][x[n]] + w[x[n]][start];
        }
    }
    else // 搜索非叶节点
    {
        for (int j = i; j <= n; j++)
        {
            if (w[x[i - 1]][x[j]] != INF && cw + w[x[i - 1]][x[j]] < bestw)
            {
                L++;
                swap(x[i], x[j]);        // 交换，将x[j]加入到当前解中
                cw += w[x[i - 1]][x[i]]; // 更新当前路径长度
                BacktrackTSP(i + 1);     // 递归搜索
                cw -= w[x[i - 1]][x[i]]; // 回溯
                swap(x[i], x[j]);
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
    n = 30;
    if (!ReadData())
        return 1;
    for (int i = 1; i <= n; i++) // 初始化
        x[i] = i;
    swap(x[1], x[start]); // 将起始基站放在第一个位置
    startTime = clock();
    BacktrackTSP(2);
    endTime = clock();
    PrintResult(n);
}