/**
 * @file LDS_2.cpp
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief 最长递减子序列问题的O(nlogn)算法
 * @date 2023-11-30
 */

/*
长度为i的递减子序列，只需要保存下结尾数字最大的一个
因为结尾数字最大，能够保证后序子序列能够全部探测到
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const int N = 1000;
const int INF = 0x3f3f3f3f;

int f[N]; // f[i]存储长度为i的递减子序列中最大结尾数字
// f[N]一定是单调递减的

void LDS(const vector<int> &a)
{
    /*
    找到大于等于a[i]的最小的f[l]
    f[l + 1]一定小于a[i]
    用a[i]去替换f[l + 1]
    */
    int len = 0;
    f[0] = 2 * 1e9;
    for (int i = 1; i <= a.size() - 1; i++)
    {
        int l = 0, r = len;
        while (l < r)
        {
            int mid = l + r + 1 >> 1;
            if (f[mid] >= a[i])
                l = mid;
            else
                r = mid - 1;
        }
        f[l + 1] = a[i];
        if (l + 1 > len)
            len = l + 1;
    }
    cout << len << endl;
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

    // 计算LDS
    cout << "The length of LDS of sequence1: ";
    LDS(a);
    cout << "The length of LDS of sequence2: ";
    LDS(b);

    return 0;
}