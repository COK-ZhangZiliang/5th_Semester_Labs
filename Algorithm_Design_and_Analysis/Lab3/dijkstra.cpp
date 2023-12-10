/**
 * @file dijkstra.cpp
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief dijkstra算法
 * @date 2023-12-09
 */
#include <iostream>
#include <iomanip>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cmath>

using namespace std;

typedef pair<double, int> PII; // distance, index
const int N = 100;
const int M = 10000;

int h[N], e[M], ne[M], idx;           // 邻接表存储所有边
double w[M];                          // 存储所有边的权重
double dist[N];                       // 存储所有点到源点的距离
bool st[N];                           // 存储每个点的最短距离是否已经确定
int n;                                // 点的数量
unordered_map<int, int> index_id;     // 存储index和基站id的映射关系
unordered_map<int, int> id_index;     // 存储基站id和index的映射关系
unordered_map<int, vector<int>> path; // 存储最短路中的路径

// 添加一条边
void add(int a, int b, double c)
{
    e[idx] = b, ne[idx] = h[a], w[idx] = c, h[a] = idx++;
}

// dijkstra算法
void dijkstra(int src)
{
    for (int i = 1; i <= n; i++)
        dist[i] = 0x3f3f3f3f; // 初始化距离为无穷大
    memset(st, 0, sizeof st);
    dist[src] = 0;
    priority_queue<PII, vector<PII>, greater<PII>> heap; // 使用优先队列优化
    heap.push({0, src});
    path[index_id[src]] = vector<int>(1, index_id[src]); // 初始化路径

    while (!heap.empty())
    {
        auto t = heap.top();
        heap.pop();

        int ver = t.second;
        double distance = t.first;
        if (!st[ver])
        {
            st[ver] = true;
            for (int i = h[ver]; i != -1; i = ne[i])
            {
                int j = e[i];
                if (dist[j] > distance + w[i])
                {
                    dist[j] = distance + w[i];
                    heap.push({dist[j], j});
                    // 更新路径
                    path[index_id[j]] = path[index_id[ver]];
                    path[index_id[j]].push_back(index_id[j]);
                }
            }
        }
    }
}

// 打印结果
void print_result(int src, int dst)
{
    cout << setw(16) << left << "<" + to_string(src) + "-" + to_string(dst) + "> "
         << "distance: ";
    if (fabs(dist[id_index[dst]] - 0x3f3f3f3f) < 1e-6)
        cout << "INF path: NULL" << endl;
    else
    {
        cout << fixed << setprecision(2) << setw(8) << left << dist[id_index[dst]] << "path: ";
        for (int j = 0; j < path[dst].size() - 1; j++)
            cout << path[dst][j] << "->";
        cout << path[dst][path[dst].size() - 1] << endl;
    }
}

int main()
{
    // 读入22个基站的数据
    memset(h, -1, sizeof h);
    idx = 0;
    n = 22;
    ifstream file("DATA/dijkstra_22.txt", ios::in);
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
        index_id[a] = a_id;
        id_index[a_id] = a;
        index_id[b] = b_id;
        id_index[b_id] = b;
        if (c > 0)
            add(a, b, c);
    }

    // 以567443为源点，进行dijkstra算法
    dijkstra(id_index[567443]);
    cout << "=====================================22=====================================\n";
    for (int i = 1; i <= n; i++)
        print_result(567443, index_id[i]);
    cout << endl;
    print_result(567443, 33109);
    cout << endl;

    // 读入42个基站的数据
    memset(h, -1, sizeof h);
    idx = 0;
    n = 42;
    ifstream file2("DATA/dijkstra_42.txt", ios::in);
    if (!file2.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }
    while (getline(file2, line))
    {
        stringstream ss(line);
        int a, b, a_id, b_id;
        double c;
        string _a, _b, _a_id, _b_id, _c;
        ss >> _a >> _b >> _a_id >> _b_id >> _c;
        a = stoi(_a), b = stoi(_b), a_id = stoi(_a_id), b_id = stoi(_b_id), c = stod(_c);
        index_id[a] = a_id;
        id_index[a_id] = a;
        index_id[b] = b_id;
        id_index[b_id] = b;
        if (c > 0)
            add(a, b, c);
    }

    // 以565845为源点，进行dijkstra算法
    dijkstra(id_index[565845]);
    cout << "=====================================42=====================================\n";
    for (int i = 1; i <= n; i++)
        print_result(565845, index_id[i]);
    cout << endl;
    print_result(565845, 565667);

    return 0;
}
