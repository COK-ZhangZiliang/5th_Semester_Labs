/**
 * @file LCS_3.cpp
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief 将c数组改为一维数组，减少空间复杂度
 * @date 2023-11-30
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

const int MAX_SIZE = 2000;

int c[MAX_SIZE];
int tmp[MAX_SIZE]; // 记录c[i-1][j-1]

// 求最长公共子序列的长度
void LCSLength(int m, int n, char *x, char *y)
{
    // 初始化，若不初始化，上一次调用LCSLength写入c数组的值会影响本次调用
    memset(c, 0, sizeof(c));

    int i, j;
    for (i = 1; i <= m; i++)
    {
        for (j = 1; j <= n; j++)
        {
            c[j] = max(c[j - 1], c[j]);
            if (x[i] == y[j])
                c[j] = tmp[j - 1] + 1;
        }
        for (j = 1; j <= n; j++)
            tmp[j] = c[j];
    }
}

int main()
{
    char str[4][MAX_SIZE];

    // 读入数据
    ifstream file("DATA/LCS_data.txt", ios::in);
    if (!file.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }
    string line;
    int i = 0;
    while (getline(file, line))
    {
        if (line.size() > 10)
        {
            strcpy(str[i] + 1, line.c_str());
            i++;
        }
    }

    // 求最长公共子序列
    int size_a = strlen(str[0] + 1), size_b = strlen(str[1] + 1), size_c = strlen(str[2] + 1), size_d = strlen(str[3] + 1);
    
    LCSLength(size_a, size_b, str[0], str[1]);
    cout << "=======A-B=======" << endl;
    cout << "The length of LCS is: " << c[size_b] << endl;

    LCSLength(size_c, size_d, str[2], str[3]);
    cout << "=======C-D=======" << endl;
    cout << "The length of LCS is: " << c[size_d] << endl;

    LCSLength(size_a, size_d, str[0], str[3]);
    cout << "=======A-D=======" << endl;
    cout << "The length of LCS is: " << c[size_d] << endl;

    LCSLength(size_b, size_c, str[1], str[2]);
    cout << "=======B-C=======" << endl;
    cout << "The length of LCS is: " << c[size_c] << endl;

    return 0;
}