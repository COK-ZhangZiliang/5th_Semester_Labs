/**
 * @file grammar.h
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief 文法类，用于读入文法，将文法转化为LL(1)文法
 * @date 2023-10-26
 */
#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <fstream>

using namespace std;

extern const string eps;

class Grammar
{
public:
    Grammar() = default;

    /**
     * @brief 从文件流中读取文法
     * @param fin 文件流
     */
    void GetGrammar(ifstream &fin);

    /**
     * @brief 消除左递归
     */
    void EliminateLeftRecursion();

    /**
     * @brief 提取左公因子
     */
    void ExtractLeftCommon();

    /**
     * @brief 构造非终结符的FIRST集
     * @param nonterminal 非终结符
     */
    void BuildFirst(const string &noterminal);

    /**
     * @brief 构造非终结符的FOLLOW集
     * @param noterminal 非终结符
     */
    void BuildFollow(const string &noterminal);

    /**
     * @brief 判断是否是LL(1)文法
     * @return 如果是LL(1)文法，返回true；否则返回false
     */
    bool IsLL1() const;

    /**
     * @brief 将文法转换为LL(1)文法
     */
    void CovertGrammar();

    unordered_set<string> nonterminals;                                  // 非终结符号集
    unordered_set<string> terminals;                                     // 终结符号集
    unordered_map<string, vector<vector<string>>> productions;           // 产生式集
    string startSymbol;                                                  // 起始符号
    unordered_map<string, vector<unordered_set<string>>> candidateFirst; // 候选式的FIRST集
    unordered_map<string, unordered_set<string>> first;                  // 非终结符号的FIRST集
    unordered_map<string, unordered_set<string>> follow;                 // 非终结符号的FOLLOW集
    unordered_map<string, bool> firstDoned;                              // FIRST集是否已经构造完成
    unordered_map<string, bool> followDoned;                             // FOLLOW集是否已经构造完成
    unordered_map<string, unordered_map<string, bool>> followRelation;   // FOLLOW集的关系
};

#endif // GRAMMAR_H