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
    void buildTable(const Grammar &g);

    /**
     * @brief 输出预测分析表
     */
    void outputTable() const;

    /**
     * @brief  错误处理
     */
    void error(const string &nonter) const;

    unordered_map<string, unordered_map<string, vector<string>>> parsingTable;
    unordered_set<string> nonterminals; // 非终结符号集
    unordered_set<string> terminals;    // 终结符号集
};

#endif // TABLE_H