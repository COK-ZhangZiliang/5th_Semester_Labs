/**
 * @file LCS_2.cpp
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief 去掉b数组，减少空间复杂度
 * @date 2023-11-30
 */
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

const int MAX_SIZE = 2000;

int c[MAX_SIZE][MAX_SIZE];

// 求最长公共子序列的长度
void LCSLength(int m, int n, char *x, char *y)
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
void LCS(int i, int j, char *x, char *y)
{
    if (i == 0 || j == 0)
        return;
    if (x[i] == y[j])
    {
        LCS(i - 1, j - 1, x, y);
        cout << x[i];
    }
    else if (c[i][j] == c[i - 1][j])
        LCS(i - 1, j, x, y);
    else
        LCS(i, j - 1, x, y);
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
    cout << "The length of LCS is: " << c[size_a][size_b] << endl;
    cout << "The LCS is: ";
    LCS(size_a, size_b, str[0], str[1]);
    cout << endl << endl;

    LCSLength(size_c, size_d, str[2], str[3]);
    cout << "=======C-D=======" << endl;
    cout << "The length of LCS is: " << c[size_c][size_d] << endl;
    cout << "The LCS is: ";
    LCS(size_c, size_d, str[2], str[3]);
    cout << endl << endl;

    LCSLength(size_a, size_d, str[0], str[3]);
    cout << "=======A-D=======" << endl;
    cout << "The length of LCS is: " << c[size_a][size_d] << endl;
    cout << "The LCS is: ";
    LCS(size_a, size_d, str[0], str[3]);
    cout << endl << endl;

    LCSLength(size_b, size_c, str[1], str[2]);
    cout << "=======B-C=======" << endl;
    cout << "The length of LCS is: " << c[size_b][size_c] << endl;
    cout << "The LCS is: ";
    LCS(size_b, size_c, str[1], str[2]);
    cout << endl << endl;

    return 0;
}