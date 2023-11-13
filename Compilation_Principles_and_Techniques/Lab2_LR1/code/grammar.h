/**
 * @file grammar.h
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief 文法类，用于读入文法并构造识别拓广文法所有活前缀的DFA
 * @date 2023-11-09
 */
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <fstream>

using namespace std;

extern const string eps;

// 自定义LR(1)项目的哈希函数
struct LR1sHash
{
    size_t operator()(const pair<pair<string, vector<string>>, pair<int, string>> &p) const
    {
        // 计算第一部分的哈希值
        size_t hash1 = hash<string>{}(p.first.first);
        for (const auto &str : p.first.second)
        {
            hash1 ^= hash<string>{}(str);
        }

        // 计算第二部分的哈希值
        size_t hash2 = hash<int>{}(p.second.first) ^ (hash<string>{}(p.second.second));

        // 合并哈希值
        return hash1 ^ (hash2 << 1);
    }
};

typedef pair<pair<string, vector<string>>, pair<int, string>> LR1; // LR(1)项目
typedef unordered_set<LR1, LR1sHash> LR1s;                         // LR(1)项目集

class Grammar
{
public:
    Grammar() = default;

    /**
     * @brief 从文件流中读取文法
     * @param fin 文件流f
     */
    void GetGrammar(ifstream &fin);

    /**
     * @brief 构造拓广文法
     */
    void ExtendGrammar();

    /**
     * @brief 构造非终结符的FIRST集
     * @param nonterminal 非终结符
     */
    void BuildFirst(const string &noterminal);

    /**
     * @brief 构造所有非终结符的FIRST集
     */
    void First();

    /**
     * @brief 构造LR(1)项目集的闭包
     * @param lr1s
     */
    void Closure(LR1s &lr1s);

    /**
     * @brief 构造LR(1)项目集的转移
     * @param lr1s
     * @param symbol
     * @param newLR1s
     */
    void Go(const LR1s &lr1s, const string &symbol, LR1s &newLR1s);

    /**
     * @brief 构造识别该文法所有活前缀的DFA
     */
    void LivePreDFA();

    /**
     * @brief 判断是否为LR(1)文法
     * @return true
     * @return false
     */
    bool IsLR1();

    unordered_set<string> nonterminals;                                  // 非终结符号集
    unordered_set<string> terminals;                                     // 终结符号集
    unordered_map<string, vector<vector<string>>> productions;           // 产生式集
    string startSymbol;                                                  // 起始符号
    unordered_map<string, unordered_set<string>> first;                  // 非终结符号的FIRST集
    unordered_map<string, vector<unordered_set<string>>> candidateFirst; // 候选式的FIRST集
    unordered_map<string, bool> firstDoned;                              // FIRST集是否已经构造完成
    unordered_map<string, unordered_map<string, bool>> firstRelation;    // FIRST集的关系
    vector<LR1s> DFAStates;                                               // DFA状态集/LR(1)项目集规范族
    unordered_map<int, unordered_map<string, int>> DFAStateTrans;        // DFA状态转移
};

#endif // GRAMMAR_H