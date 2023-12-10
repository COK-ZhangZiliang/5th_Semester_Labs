/**
 * @file huffman.cpp
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief 哈夫曼编码
 * @date 2023-12-09
 */
#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

// 哈夫曼树节点的结构体
struct HuffmanNode
{
    char data;                 // 存储字符
    int frequency;             // 存储频率
    HuffmanNode *left, *right; // 左右子节点
};

unordered_map<char, pair<int, string>> codes; // 用于保存字符和对应的哈夫曼编码

// 小根堆化操作
void minHeapify(vector<HuffmanNode *> &heap, int i, int heapSize)
{
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // 找出左右子节点中最小的节点
    if (left < heapSize && heap[left]->frequency < heap[smallest]->frequency)
        smallest = left;

    if (right < heapSize && heap[right]->frequency < heap[smallest]->frequency)
        smallest = right;

    // 如果最小的节点不是当前节点，则交换二者位置并递归调用小根堆化操作
    if (smallest != i)
    {
        swap(heap[i], heap[smallest]);
        minHeapify(heap, smallest, heapSize);
    }
}

// 提取最小节点操作
HuffmanNode *extractMin(vector<HuffmanNode *> &heap, int &heapSize)
{
    HuffmanNode *root = heap[0];
    heap[0] = heap[heapSize - 1];
    heapSize--;
    heap.pop_back();
    minHeapify(heap, 0, heapSize);
    return root;
}

// 插入节点操作
void insertNode(vector<HuffmanNode *> &heap, HuffmanNode *node, int &heapSize)
{
    heapSize++;
    int i = heapSize - 1;
    heap.push_back(node);

    // 如果节点比其父节点小，则交换二者位置并递归调用插入节点操作
    while (i != 0 && heap[(i - 1) / 2]->frequency > heap[i]->frequency)
    {
        swap(heap[i], heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// 保存哈夫曼编码
void saveCodes(HuffmanNode *root, string str)
{
    if (!root)
        return;

    // 如果节点是叶子节点，则将字符和对应的编码保存到codes中
    if (root->data != '$')
        codes[root->data] = {root->frequency, str};

    // 分别递归遍历左右子树，并将当前节点的编码加上'0'或'1'
    saveCodes(root->left, str + "0");
    saveCodes(root->right, str + "1");
}

// 构建哈夫曼树并保存哈夫曼编码
void HuffmanCodes(unordered_map<char, int> freq)
{
    vector<HuffmanNode *> heap;
    int heapSize = 0;

    // 将字符和对应的频率放入小根堆中
    for (const auto &p : freq)
    {
        heap.push_back(new HuffmanNode{p.first, p.second, nullptr, nullptr});
        heapSize++;
    }

    while (heapSize > 1)
    {
        // 提取两个最小节点并合并为一个新节点
        HuffmanNode *left = extractMin(heap, heapSize);
        HuffmanNode *right = extractMin(heap, heapSize);
        HuffmanNode *top = new HuffmanNode{'$', left->frequency + right->frequency, left, right};

        // 将新节点插入小根堆中
        insertNode(heap, top, heapSize);
    }

    // 保存哈夫曼编码
    saveCodes(heap[0], "");
}

int main()
{
    // 读入数据
    ifstream file("DATA/huffman.txt", ios::in);
    if (!file.is_open())
    {
        cerr << "Failed to open file." << endl;
        return 1;
    }
    unordered_map<char, int> freq; // 用于统计字符频率

    char c;
    while (file.get(c))
    {
        if (isalnum(c) || isspace(c) || ispunct(c) || iscntrl(c))
        {
            freq[c]++;
        }
    }

    HuffmanCodes(freq);

    // 打印字符和对应的哈夫曼编码
    cout << "=======================Huffman Codes=======================\n";
    int cnt = 0;
    for (const auto &p : codes)
    {
        string s(1, p.first);
        if (s == "\n")
            s = "\\n";
        cout << setw(20) << left << "<" + s + " " + to_string(p.second.first) + "> " + p.second.second;
        if (++cnt % 3 == 0)
            cout << endl;
    }
    cout << endl
         << endl;

    // 与等长编码比较
    cout << "===================Compare===================\n";
    int sum = 0;
    for (const auto &p : codes)
        sum += p.second.first * p.second.second.size();
    int sum2 = 0;
    for (const auto &p : freq)
        sum2 += p.second;
    sum2 *= ceil(log2(freq.size()));
    cout << "The length of Huffman codes is " << sum << "bits." << endl;
    cout << "The length of equal-length codes is " << sum2 << "bits." << endl;
    cout << "Saved " << double(sum2 - sum) / sum2 * 100 << "% space." << endl;

    return 0;
}
