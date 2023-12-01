/**
 * @file LDS_1.cpp
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief 利用最长公共子序列求解最长递减子序列问题
 * @date 2023-11-30
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_SIZE = 2000;
const int INF = 0x3f3f3f3f;

int c[MAX_SIZE][MAX_SIZE];

// 求最长公共子序列的长度
void LCSLength(int m, int n, const vector<int> &x, const vector<int> &y)
{
    int i, j;
    for (i = 1; i <= m; i++)
        for (j = 1; j <= n; j++)
        {
            if (x[i] == y[j])
                c[i][j] = c[i - 1][j - 1] + 1;
            else if (c[i - 1][j] >= c[i][j - 1])
                c[i][j] = c[i - 1][j];
            else
                c[i][j] = c[i][j - 1];
        }
}

// 输出最长公共子序列
void LCS(int i, int j, const vector<int> &x, const vector<int> &y)
{
    if (i == 0 || j == 0)
        return;
    if (x[i] == y[j])
    {
        LCS(i - 1, j - 1, x, y);
        cout << x[i] << " ";
    }
    else if (c[i][j] == c[i - 1][j])
        LCS(i - 1, j, x, y);
    else
        LCS(i, j - 1, x, y);
}

int main()
{
    vector<int> a, b;
    a.push_back(INF);
    b.push_back(INF);

    // 读入数据
    ifstream file("DATA/LDS_data1.txt", ios::in);
    if (!file.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }
    string line;
    while (getline(file, line) && line != "")
        a.push_back(stoi(line));

    ifstream file2("DATA/LDS_data2.txt", ios::in);
    if (!file2.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }
    while (getline(file2, line))
        b.push_back(stoi(line));

    // 将原序列降序排列
    vector<int> a_t = a, b_t = b;
    sort(a_t.begin(), a_t.end(), [](int a, int b)
         { return a > b; });
    sort(b_t.begin(), b_t.end(), [](int a, int b)
         { return a > b; });

    // 求最长递减子序列
    LCSLength(a.size() - 1, a_t.size() - 1, a, a_t);
    cout << "The length of LDS of sequence1: " << c[a.size() - 1][a_t.size() - 1] << endl;
    cout << "The LDS of sequence1: ";
    LCS(a.size() - 1, a_t.size() - 1, a, a_t);
    cout << endl;
    LCSLength(b.size() - 1, b_t.size() - 1, b, b_t);
    cout << "The length of LDS of sequence2: " << c[b.size() - 1][b_t.size() - 1] << endl;
    cout << "The LDS of sequence2: ";
    LCS(b.size() - 1, b_t.size() - 1, b, b_t);

    return 0;
}