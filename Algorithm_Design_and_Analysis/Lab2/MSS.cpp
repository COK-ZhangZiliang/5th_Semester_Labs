/**
 * @file MSS.cpp
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief 最大子段和算法
 * @date 2023-12-01 
 */
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_SIZE = 1000;
const int INF = 0x3f3f3f3f;

int best_i, best_j; // 最大子段的起始位置和结束位置

// 求解最大子段和
int MaxSum(int n, int *a)
{
    int max_sum = -INF, sum = 0; // 最大子段和，当前子段和
    int cur_i, cur_j;            // 当前子段的起始位置和结束位置
    for (int i = 1; i <= n; i++)
    {
        if (sum > 0)
        {
            cur_j = i;
            sum += a[i];
        }
        else
        {
            cur_i = i;
            cur_j = i;
            sum = a[i];
        }
        if (sum > max_sum)
        {
            best_i = cur_i;
            best_j = cur_j;
            max_sum = sum;
        }
    }
    return max_sum;
}

int main()
{
    int a[MAX_SIZE], b[MAX_SIZE];
    int n = 0, m = 0;

    // 读入数据
    ifstream file("DATA/MSS_data1.txt", ios::in);
    if (!file.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }
    string line;
    while (getline(file, line) && line != "")
        a[++n] = stoi(line);

    ifstream file2("DATA/MSS_data2.txt", ios::in);
    if (!file2.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }
    while (getline(file2, line))
        b[++m] = stoi(line);

    // 求解最大子段和
    int max_sum = MaxSum(n, a);
    cout << "=====Sequence1=====" << endl;
    cout << "Best i = " << best_i << ", best j = " << best_j << endl;
    cout << "Max subsequence sum of sequence1 is: " << max_sum << endl
         << endl;
    max_sum = MaxSum(m, b);
    cout << "=====Sequence2=====" << endl;
    cout << "Best i = " << best_i << ", best j = " << best_j << endl;
    cout << "Max subsequence sum of sequence2 is: " << max_sum << endl
         << endl;

    // 验证结果
    cout << "Verifying..." << endl;
    max_sum = -INF;
    for (int i = 1; i <= n; i++)
    {
        int sum = 0;
        for (int j = i; j <= n; j++)
        {
            sum += a[j];
            if (sum > max_sum)
            {
                max_sum = sum;
                best_i = i;
                best_j = j;
            }
        }
    }
    cout << "=====Sequence1=====" << endl;
    cout << "Best i = " << best_i << ", best j = " << best_j << endl;
    cout << "Max subsequence sum of sequence1 is: " << max_sum << endl
         << endl;

    max_sum = -INF;
    for (int i = 1; i <= m; i++)
    {
        int sum = 0;
        for (int j = i; j <= m; j++)
        {
            sum += b[j];
            if (sum > max_sum)
            {
                max_sum = sum;
                best_i = i;
                best_j = j;
            }
        }
    }
    cout << "=====Sequence2=====" << endl;
    cout << "Best i = " << best_i << ", best j = " << best_j << endl;
    cout << "Max subsequence sum of sequence2 is: " << max_sum << endl;

    return 0;
}