#include "table.h"
#include <iostream>

int acc = 0x3f3f3f3f;

void Table::BuildTable(const Grammar &g)
{
    // 拷贝DFA状态集和DFA状态转移
    DFAStates = g.DFAStates;
    DFAStateTrans = g.DFAStateTrans;
    // 拷贝非终结符号集和终结符号集
    nonterminals = g.nonterminals;
    terminals = g.terminals;
    terminals.insert("$");

    // 构造LR(1)分析表
    for (int i = 0; i < DFAStates.size(); i++)
    {
        // 移进和待约项目
        for (const auto &tran : DFAStateTrans[i])
        {
            string symbol = tran.first;
            int nextState = tran.second;
            parsingTable[i][symbol] = nextState;
        }
        // 规约项目
        for (const auto &lr1 : DFAStates[i])
        {
            string nonter = lr1.first.first;
            vector<string> pro = lr1.first.second;
            int index = lr1.second.first;
            string nextSymbol = lr1.second.second;
            if (index == pro.size())
            {
                if (nonter == g.startSymbol)
                {
                    parsingTable[i]["$"] = acc;
                    continue;
                }
                bool exist = false;
                for (auto it = productions.begin(); it != productions.end(); it++)
                {
                    if (it->first == nonter && it->second == pro)
                    {
                        exist = true;
                        parsingTable[i][nextSymbol] = -(it - productions.begin() + 1);
                        break;
                    }
                }
                if (!exist)
                {
                    productions.push_back(make_pair(nonter, pro));
                    parsingTable[i][nextSymbol] = -productions.size();
                }
            }
        }
    }
}

void Table::OutputTable() const
{
    cout << "====Productions====" << endl;
    for (int i = 0; i < productions.size(); i++)
    {
        cout << "(" << i << ") " << productions[i].first << " -> ";
        for (const auto &symbol : productions[i].second)
            cout << symbol << " ";
        cout << endl;
    }
    cout << endl;

    cout << "====Parsing Table====" << endl;
    for (int i = 0; i < DFAStates.size(); i++)
    {
        cout << "I" << i << ": {action: [";
        for (const auto terminal : terminals)
        {
            if (parsingTable.at(i).find(terminal) != parsingTable.at(i).end())
            {
                if (parsingTable.at(i).at(terminal) == acc)
                    cout << "(" << terminal << ", ACC) ";
                else
                {
                    if (parsingTable.at(i).at(terminal) >= 0)
                        cout << "(" << terminal << ", S" << parsingTable.at(i).at(terminal) << ") ";
                    else
                        cout << "(" << terminal << ", R" << -parsingTable.at(i).at(terminal) - 1 << ") ";
                }
            }
            else
                cout << "(" << terminal << ", error) ";
        }
        cout << "], goto: [";
        for (const auto nonter : nonterminals)
        {
            if (nonter != "S'")
            {
                if (parsingTable.at(i).find(nonter) != parsingTable.at(i).end())
                    cout << "(" << nonter << ", " << parsingTable.at(i).at(nonter) << ") ";
                else
                    cout << "(" << nonter << ", error) ";
            }
        }
        cout << "]}" << endl;
    }
    cout << endl;
}

void Table::Error(const int state, string &symbol, const int index, string &sentence) const
{
    cout << "Error: ";
    switch (state)
    {
    case 0:
        cout << "Missing arithmetic object!" << endl;
        sentence.insert(index, "1");
        symbol = "num";
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        cout << "Mismatched parentheses!" << endl;
        sentence.erase(index, 1);
        symbol = sentence[index];
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        cout << "Missing arithmetic object!" << endl;
        sentence.insert(index, "1");
        symbol = "num";
        break;
    case 10:
    case 11:
    case 12:
    case 13:
        cout << "Missing closing parenthese!" << endl;
        sentence.insert(index, ")");
        symbol = ")";
        break;
    case 14:
        cout << "Missing arithmetic object!" << endl;
        sentence.insert(index, "1");
        symbol = "num";
        break;
    case 15:
    case 16:
    case 17:
    case 18:
        cout << "Missing operator!" << endl;
        sentence.insert(index, "+");
        symbol = "+";
        break;
    case 19:
    case 20:
        cout << "Missing arithmetic object!" << endl;
        sentence.insert(index, "1");
        symbol = "num";
        break;
    case 21:
        cout << "Missing operator!" << endl;
        sentence.insert(index, "+");
        symbol = "+";
        break;
    case 22:
    case 23:
        cout << "Missing arithmetic object!" << endl;
        sentence.insert(index, "1");
        symbol = "num";
        break;
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
        cout << "Missing opening parenthese or operator!" << endl;
        sentence.insert(index, "(");
        symbol = "(";
        break;
    default:
        break;
    }
}