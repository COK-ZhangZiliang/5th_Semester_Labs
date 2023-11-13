#include "table.h"
#include <iostream>

void Table::BuildTable(const Grammar &g)
{
    // 拷贝文法的非终结符和终结符
    nonterminals = g.nonterminals;
    terminals = g.terminals;
    terminals.insert("$");
    // 构造预测分析表
    for (const auto &nonter : g.nonterminals)
    {
        for (auto it = g.productions.at(nonter).begin(); it != g.productions.at(nonter).end(); it++)
        {
            int index = it - g.productions.at(nonter).begin();
            vector<unordered_set<string>> first = g.candidateFirst.at(nonter);
            for (const auto &fir : first[index])
                if (fir != eps)
                    parsingTable[nonter][fir] = *it;
            // 候选式的FIRST集中包含epsilon
            if (first[index].find(eps) != first[index].end())
            {
                for (const auto &fol : g.follow.at(nonter))
                    parsingTable[nonter][fol] = *it;
            }
        }
    }
}

void Table::OutputTable() const
{
    cout << "====Parsing Table====" << endl;
    for (auto const &nonter : nonterminals)
    {
        cout << nonter << " [ ";
        for (auto const &ter : terminals)
        {
            if (parsingTable.at(nonter).find(ter) != parsingTable.at(nonter).end())
            {
                cout << "( " << ter << ": " << nonter << " -> ";
                for (auto const &str : parsingTable.at(nonter).at(ter))
                    cout << str << " ";
                cout << ") ";
            }
            else
            {
                cout << "( " << ter << ": error ) ";
            }
        }
        cout << ']' << endl;
    }
    cout << endl;
}

void Table::Error(const string &top, const string &next) const
{
    cout << "Error: ";
    switch (top[0])
    {
    case 'E':
        cout << "Missing arithmetic object!" << endl;
        break;
    case 'T':
        if (next == "(")
            cout << "Missing operator!" << endl;
        else
            cout << "Missing arithmetic object!" << endl;
        break;
    case 'F':
        cout << "Missing arithmetic object!" << endl;
        break;
    case '$':
        cout << "Missing opening parenthese or operator!" << endl;
        break;
    case ')':
        cout << "Missing closing parenthese!" << endl;
        break;
    default:
        break;
    }
}