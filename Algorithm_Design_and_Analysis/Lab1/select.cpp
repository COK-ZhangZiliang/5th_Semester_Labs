/**
 * @file select.cpp
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief 线性时间选择算法
 * @date 2023-11-22
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

const int MAX_NUM = 2000; // 最大元素个数
int level = 0;            // 递归深度

void BubbleSort(double a[], int p, int r);
int Partition(double a[], int p, int r, double x);
double Select(double a[], int p, int r, int k, int cur_level);
void KthSmallest(double a[], int p, int r, int k);

int main()
{
    // 读入数据
    ifstream file("DATA/data1.txt", ios::in);
    if (!file.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }

    double k_dist[MAX_NUM];
    int row_idx = 0;
    string line;
    while (row_idx < MAX_NUM && getline(file, line))
    {
        k_dist[row_idx] = stod(line);
        row_idx++;
    }
    cout << row_idx << " rows read." << endl;

    file.close();

    // 选择第k小的元素
    KthSmallest(k_dist, 0, row_idx - 1, 1);
    KthSmallest(k_dist, 0, row_idx - 1, 5);
    KthSmallest(k_dist, 0, row_idx - 1, 50);
    KthSmallest(k_dist, 0, row_idx - 1, row_idx);

    return 0;
}

// 冒泡排序
void BubbleSort(double a[], int p, int r)
{
    for (int i = p; i < r; i++)
        for (int j = r; j > i; j--)
            if (a[j] < a[j - 1])
                swap(a[j], a[j - 1]);
}

// 以 x 为主元划分
int Partition(double a[], int p, int r, double x)
{
    // 将 x 放到最左边
    swap(a[p + (r - p - 4) / 10], a[p]);

    int i = p, j = r + 1;
    while (true)
    {
        while (a[++i] < x && i < r)
            ;
        while (a[--j] > x)
            ;
        if (i >= j)
            break;
        swap(a[i], a[j]);
    }
    a[p] = a[j];
    a[j] = x;
    return j;
}

// 递归查找第k小的元素
double Select(double a[], int p, int r, int k, int cur_level)
{
    // 更新递归深度
    if (cur_level > level)
        level = cur_level;

    // 规模小于20的子问题直接用冒泡排序查找
    if (r - p < 20)
    {
        BubbleSort(a, p, r);
        return a[p + k - 1];
    };

    // 分成 n/5 组并找到每组的中位数，若最后一段不足5个元素则不考虑
    for (int i = 0; i <= (r - p - 4) / 5; i++)
    {
        int s = p + 5 * i;
        int t = s + 4;
        BubbleSort(a, s, t);
        swap(a[p + i], a[s + 2]);
    }

    // 找到所有中位数的中位数
    double x = Select(a, p, p + (r - p - 4) / 5, (r - p + 6) / 10, cur_level + 1);

    // 以中位数的中位数为主元划分
    int i = Partition(a, p, r, x);
    int j = i - p + 1; // 左部元素个数

    // 一分为二
    if (k <= j)
        return Select(a, p, i, k, cur_level + 1);
    else
        return Select(a, i + 1, r, k - j, cur_level + 1);

    // 一分为三
    // if (k == j)
    //     return x;
    // else
    // {
    //     if (k < j)
    //         return Select(a, p, i, k, cur_level + 1);
    //     else
    //         return Select(a, i + 1, r, k - j, cur_level + 1);
    // }
}

// 找到第k小的元素
void KthSmallest(double a[], int p, int r, int k)
{
    level = 0;
    double kth_smallest = Select(a, p, r, k, 1);
    cout << "The " << k << "th smallest element is " << kth_smallest << endl;
    cout << "The recursion depth is " << level << endl;
}