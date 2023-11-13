/**
 * @file table.h
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief 预测分析表的构造和输出以及错误处理
 * @date 2023-10-28
 */
#ifndef TABLE_H
#define TABLE_H

#include "grammar.h"

class Table
{
public:
    Table() = default;

    /**
     * @brief 构造预测分析表
     * @param grammar LL(1)文法
     */
    void BuildTable(const Grammar &g);

    /**
     * @brief 输出预测分析表
     */
    void OutputTable() const;

    /**
     * @brief  错误处理
     * @param nonter 非终结符号
     * @param next 输入串的下一个符号
     */
    void Error(const string &nonter, const string &next) const;

    unordered_map<string, unordered_map<string, vector<string>>> parsingTable; // 预测分析表
    unordered_set<string> nonterminals;                                        // 非终结符号集
    unordered_set<string> terminals;                                           // 终结符号集
};

#endif // TABLE_H