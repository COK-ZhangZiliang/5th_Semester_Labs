/**
 * @file cpair.cpp
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief 平面最近点对算法
 * @version 0.1
 * @date 2023-11-23
 */
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

const int INF = 0x3f3f3f3f;
const int MAX_NUM = 2000; // 最大元素个数
const double PI = 3.1415926535897932384626;
const double R = 6378.137 * 1e3; // 地球半径

typedef struct
{
    int ENODEBID;
    double LATITUDE;
    double LONGITUDE;
} Point;

Point tmp[MAX_NUM];

double distance(Point p1, Point p2);
void merge(Point a[], Point b[], int l, int m, int r, int mode);
void merge_sort(Point a[], int l, int r, int mode);
bool cmp(Point p1, Point p2);
void closest3(Point X[], int l, Point &p1, Point &p2, double &d);
void closest(Point X[], Point Y[], Point Z[], int l, int r, Point &p1, Point &p2, double &d);

int main()
{
    // 读入数据
    ifstream file("DATA/data2.txt", ios::in);
    if (!file.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }

    Point points[MAX_NUM];
    int row_idx = 0;
    string line;
    while (row_idx < MAX_NUM && getline(file, line))
    {
        int pos = line.find(' ');
        points[row_idx].ENODEBID = stoi(line.substr(0, pos));
        line = line.substr(pos + 1);
        pos = line.find(' ');
        points[row_idx].LATITUDE = stod(line.substr(0, pos));
        line = line.substr(pos + 1);
        points[row_idx].LONGITUDE = stod(line);
        row_idx++;
    }
    cout << row_idx << " rows read." << endl;

    file.close();

    Point X[MAX_NUM], Y[MAX_NUM], Z[MAX_NUM];
    merge_sort(points, 0, row_idx - 1, 0); // 按照纬度排序
    for (int i = 0; i < row_idx; i++)
        X[i] = points[i];
    merge_sort(points, 0, row_idx - 1, 1); // 按照经度排序
    for (int i = 0; i < row_idx; i++)
        Y[i] = points[i];

    // 求解最近点对
    Point p1, p2;
    double d;
    closest(X, Y, Z, 0, row_idx - 1, p1, p2, d);
    cout << "The closest pair is: " << p1.ENODEBID << " " << p2.ENODEBID << endl;
    cout << "Point 1: " << p1.LATITUDE << " " << p1.LONGITUDE << endl;
    cout << "Point 2: " << p2.LATITUDE << " " << p2.LONGITUDE << endl;
    cout << "The distance is: " << d << " m" << endl;

    // 求解次近点对
    for (int i = 0; i < row_idx; i++)
    {
        if (p1.ENODEBID == X[i].ENODEBID)
        {
            X[i].LATITUDE = 0;
            X[i].LONGITUDE = 0;
        }
        if (p1.ENODEBID == Y[i].ENODEBID)
        {
            Y[i].LATITUDE = 0;
            Y[i].LONGITUDE = 0;
        }
    }
    closest(X, Y, Z, 0, row_idx - 1, p1, p2, d);
    cout << "The second closest pair is: " << p1.ENODEBID << " " << p2.ENODEBID << endl;
    cout << "Point 1: " << p1.LATITUDE << " " << p1.LONGITUDE << endl;
    cout << "Point 2: " << p2.LATITUDE << " " << p2.LONGITUDE << endl;
    cout << "The distance is: " << d << " m" << endl;

    // 验证结果
    cout << "Verifying..." << endl;
    double min_dist = INF;
    for (int i = 0; i < row_idx; i++)
        for (int j = i + 1; j < row_idx; j++)
        {
            double dist = distance(points[i], points[j]);
            if (dist < min_dist && cmp(points[i], points[j]))
                min_dist = dist;
        }
    cout << "The minimum distance is: " << min_dist << " m" << endl;
    double min_dist2 = INF;
    for (int i = 0; i < row_idx; i++)
        for (int j = i + 1; j < row_idx; j++)
        {
            double dist = distance(points[i], points[j]);
            if (dist < min_dist2 && cmp(points[i], points[j]) && dist != min_dist)
                min_dist2 = dist;
        }
    cout << "The second minimum distance is: " << min_dist2 << " m" << endl;
}

// 计算两点之间的距离
double distance(Point p1, Point p2)
{
    double radLat1 = p1.LATITUDE * PI / 180.0;
    double radLat2 = p2.LATITUDE * PI / 180.0;
    double radLon1 = p1.LONGITUDE * PI / 180.0;
    double radLon2 = p2.LONGITUDE * PI / 180.0;
    return R * acos(cos(radLat1) * cos(radLat2) * cos(radLon1 - radLon2) + sin(radLat1) * sin(radLat2));
}

// 归并排序
void merge_sort(Point a[], int l, int r, int mode)
{
    if (l >= r)
        return;
    int m = (l + r) / 2;
    merge_sort(a, l, m, mode);
    merge_sort(a, m + 1, r, mode);
    merge(a, tmp, l, m, r, mode);
    for (int i = l, j = l; i <= r; i++, j++)
        a[i] = tmp[j];
}

// 归并
void merge(Point a[], Point b[], int l, int m, int r, int mode)
{
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r)
        if ((a[i].LONGITUDE <= a[j].LONGITUDE && mode) || (a[i].LATITUDE <= a[j].LATITUDE && !mode))
            b[k++] = a[i++];
        else
            b[k++] = a[j++];
    while (i <= m)
        b[k++] = a[i++];
    while (j <= r)
        b[k++] = a[j++];
}

// 比较两个点位置是否相同
bool cmp(Point p1, Point p2)
{
    if ((fabs(p1.LATITUDE - p2.LATITUDE) < 1e-8) && (fabs(p1.LONGITUDE - p2.LONGITUDE) < 1e-8))
        return false;
    return true;
}

// 求解三个点中的最近点对
void closest3(Point X[], int l, Point &p1, Point &p2, double &d)
{
    double d1 = distance(X[l], X[l + 1]);
    double d2 = distance(X[l], X[l + 2]);
    double d3 = distance(X[l + 1], X[l + 2]);
    if (!cmp(X[l], X[l + 1]))
        d1 = INF;
    if (!cmp(X[l], X[l + 2]))
        d2 = INF;
    if (!cmp(X[l + 1], X[l + 2]))
        d3 = INF;
    if (d1 <= d2 && d1 <= d3)
    {
        p1 = X[l];
        p2 = X[l + 1];
        d = d1;
    }
    else if (d2 <= d1 && d2 <= d3)
    {
        p1 = X[l];
        p2 = X[l + 2];
        d = d2;
    }
    else
    {
        p1 = X[l + 1];
        p2 = X[l + 2];
        d = d3;
    }
}

/**
 * @brief 递归求解最近点对
 * @param X 按照x坐标排序的点集
 * @param Y 按照y坐标排序的点集
 * @param Z 跨越两个子区域的点集
 * @param l 左边界
 * @param r 右边界
 * @param p1 最近点对中的一个点
 * @param p2 最近点对中的另一个点
 * @param d 最近点对的距离
 */
void closest(Point X[], Point Y[], Point Z[], int l, int r, Point &p1, Point &p2, double &d)
{
    // 只有一个点
    if (r == l)
    {
        p1 = X[l];
        p2 = X[l];
        d = INF;
        return;
    }

    // 只有两个点
    if (r - l == 1)
    {
        p1 = X[l];
        p2 = X[r];
        d = distance(p1, p2);
        if (!cmp(p1, p2))
            d = INF;
        return;
    }

    // 只有三个点
    if (r - l == 2)
    {
        closest3(X, l, p1, p2, d);
        return;
    }

    // 将Y[]以X[]的中位数为界分成两部分
    int m = (l + r) / 2;
    while (m < r && fabs(X[m].LATITUDE - X[m + 1].LATITUDE) < 1e-8)
        m++;
    if (m != r)
    {
        int f = l, g = m + 1;
        for (int i = l; i <= r; i++)
            if (Y[i].LATITUDE > X[m].LATITUDE)
                Z[g++] = Y[i];
            else
                Z[f++] = Y[i];
    }
    else
    {
        m = (l + r) / 2;
        for (int i = l; i <= r; i++)
            Z[i] = X[i];
        merge_sort(Z, l, m, 1);
        merge_sort(Z, m + 1, r, 1);
    }

    closest(X, Z, Y, l, m, p1, p2, d);
    double dr;    // 右部分的最近点对距离
    Point p3, p4; // 右部分的最近点对
    closest(X, Z, Y, m + 1, r, p3, p4, dr);
    if (dr < d)
    {
        p1 = p3;
        p2 = p4;
        d = dr;
    }

    merge(Z, Y, l, m, r, 1); // 重构Y[]

    int k = l;
    for (int i = l; i <= r; i++)
        if (fabs(Y[i].LATITUDE - X[m].LATITUDE) < d)
            Z[k++] = Y[i];

    // 求解跨越两个子区域的最近点对
    for (int i = l; i < k; i++)
    {
        for (int j = i + 1; j < k && Z[j].LONGITUDE - Z[i].LONGITUDE < d; j++)
        {
            double d1 = distance(Z[i], Z[j]);
            if (!cmp(Z[i], Z[j]))
                d1 = INF;
            if (d1 < d)
            {
                d = d1;
                p1 = Z[i];
                p2 = Z[j];
            }
        }
    }
}