/**
 * @file table.h
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief 预测分析表类，用于构造LR(1)分析表
 * @date 2023-11-10
 */
#ifndef TABLE_H
#define TABLE_H

#include "grammar.h"
#include <map>
#include <stack>

extern int acc;

class Table
{
public:
    Table() = default;

    /**
     * @brief 构造LR(1)分析表
     * @param grammar LR(1)文法
     */
    void BuildTable(const Grammar &g);

    /**
     * @brief 输出LR(1)分析表
     */
    void OutputTable() const;

    /**
     * @brief  错误处理
     * @param state 当前状态
     * @param symbol 输入串的下一个符号
     * @param index 输入串的下一个符号的下标
     * @param sentence 输入串
     */
    void Error(const int state, string &symbol, const int index, string &sentence) const;

    unordered_set<string> nonterminals;                           // 非终结符号集
    unordered_set<string> terminals;                              // 终结符号集
    vector<LR1s> DFAStates;                                       // DFA状态集/LR(1)项目集规范族
    unordered_map<int, unordered_map<string, int>> DFAStateTrans; // DFA状态转移
    vector<pair<string, vector<string>>> productions;             // 产生式集合
    unordered_map<int, unordered_map<string, int>> parsingTable;  // 预测分析表
};

#endif // TABLE_H