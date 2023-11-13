#include "grammar.h"
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

const string eps = "epsilon";

void Grammar::GetGrammar(ifstream &fin)
{
    string str;
    getline(fin, str);
    bool isStart = true;
    if (str == "====Nonterminal Symbols====")
    {
        while (true)
        {
            getline(fin, str);
            if (str == "======Terminal Symbols=====")
                break;
            istringstream iss(str);
            string tmp;
            while (iss >> tmp)
            {
                if (isStart)
                {
                    startSymbol = tmp;
                    isStart = false;
                }
                nonterminals.insert(tmp);
            }
        }
        while (true)
        {
            getline(fin, str);
            if (str == "=======Grammar Rules=======")
                break;
            istringstream iss(str);
            string tmp;
            while (iss >> tmp)
                terminals.insert(tmp);
        }
        while (!fin.eof())
        {
            getline(fin, str);
            istringstream iss(str);
            string left, tmp;
            iss >> left >> tmp;
            if (tmp == "->")
            {
                vector<string> right;
                while (!iss.eof())
                {
                    iss >> tmp;
                    if (tmp == "|")
                    {
                        productions[left].push_back(right);
                        right.clear();
                        continue;
                    }
                    right.push_back(tmp);
                }
                productions[left].push_back(right);
            }
        }
    }
}

void Grammar::ExtendGrammar()
{
    string newStartSymbol = "S'";
    nonterminals.insert(newStartSymbol);
    productions[newStartSymbol].push_back({startSymbol});
    startSymbol = newStartSymbol;

    // 输出拓广文法
    cout << "====After Extend====" << endl;
    cout << "start symbol: " << startSymbol << endl;
    for (const auto &nonter : nonterminals)
    {
        cout << nonter << " -> ";
        for (auto it = productions[nonter].begin(); it != productions[nonter].end(); it++)
        {
            for (const auto &sym : *it)
                cout << sym << " ";
            if (it != productions[nonter].end() - 1)
                cout << "| ";
        }
        cout << endl;
    }
    cout << endl;
}

void Grammar::BuildFirst(const string &nonterminal)
{
    if (firstDoned[nonterminal])
        return;
    int index = 0;
    for (const auto &pro : productions[nonterminal])
    {
        int cnt = 0;
        candidateFirst[nonterminal].push_back(unordered_set<string>{});
        for (const auto &fir : pro)
        {
            if (terminals.find(fir) != terminals.end() || fir == eps) // fir是终结符或者epsilon
            {
                candidateFirst[nonterminal][index].insert(fir);
                break;
            }
            // fir是非终结符
            firstRelation[nonterminal][fir] = true;
            if (!firstRelation[fir][nonterminal])
            {
                BuildFirst(fir);
                candidateFirst[nonterminal][index].insert(first[fir].begin(), first[fir].end());
                candidateFirst[nonterminal][index].erase(eps);
            }
            if (first[fir].find(eps) == first[fir].end()) // 若fir的FIRST集不包含epsilon，无需继续往后构造
                break;
            cnt++;
        }
        if (cnt == pro.size()) // 产生式右部全是非终结符，且都能推出epsilon
            candidateFirst[nonterminal][index].insert(eps);
        first[nonterminal].insert(candidateFirst[nonterminal][index].begin(), candidateFirst[nonterminal][index].end());
        index++;
    }
    firstDoned[nonterminal] = true;
}

void Grammar::First()
{
    // 构造非终结符的FIRST集
    for (const auto &nonterminal : nonterminals)
        if (!firstDoned[nonterminal])
            BuildFirst(nonterminal);

    // 处理FIRST集相互包含的情况
    for (const auto &nonterminal1 : nonterminals)
        for (const auto &nonterminal2 : nonterminals)
            if (firstRelation[nonterminal1][nonterminal2] && firstRelation[nonterminal2][nonterminal1])
            {
                first[nonterminal1].insert(first[nonterminal2].begin(), first[nonterminal2].end());
                first[nonterminal2].insert(first[nonterminal1].begin(), first[nonterminal1].end());
            }

    // 输出FIRST集
    cout << "====FIRST Sets====" << endl;
    for (const auto &nonterminal : nonterminals)
    {
        cout << nonterminal << ": ";
        for (const auto &fir : first[nonterminal])
            cout << fir << " ";
        cout << endl;
    }
    cout << endl;
}

void Grammar::Closure(LR1s &lr1s)
{
    while (true)
    {
        LR1s newLR1s = lr1s;
        for (auto const &lr1 : lr1s)
        {
            string nonterminal = lr1.first.first;                                     // 非终结符
            vector<string> pro = lr1.first.second;                                    // 产生式
            int index = lr1.second.first;                                             // 圆点的位置
            string nextSymbols = lr1.second.second;                                   // 向前看符号
            if (index == pro.size() || terminals.find(pro[index]) != terminals.end()) // 圆点在产生式最后或者圆点后面是终结符
                continue;
            // 加入新的LR(1)项目
            for (auto const &p : productions[pro[index]])
            {
                string newNonterminal = pro[index];
                vector<string> newPro = p;
                int newIndex = 0;
                if (index == pro.size() - 1)
                    newLR1s.insert({{newNonterminal, newPro}, {newIndex, nextSymbols}});
                else
                {
                    // pro[index + 1]是非终结符
                    if (nonterminals.find(pro[index + 1]) != nonterminals.end())
                    {
                        int idx = index + 1;
                        while (idx < pro.size() && first[pro[idx]].find(eps) != first[pro[idx]].end())
                        {
                            for (auto const &fir : first[pro[idx]])
                                if (fir != eps)
                                    newLR1s.insert({{newNonterminal, newPro}, {newIndex, fir}});
                            idx++;
                        }
                        if (idx == pro.size())
                            newLR1s.insert({{newNonterminal, newPro}, {newIndex, nextSymbols}});
                        else
                            for (auto const &fir : first[pro[idx]])
                                newLR1s.insert({{newNonterminal, newPro}, {newIndex, fir}});
                    }
                    // pro[index + 1]是终结符
                    else
                        newLR1s.insert({{newNonterminal, newPro}, {newIndex, pro[index + 1]}});
                }
            }
        }
        if (newLR1s.size() == lr1s.size())
            break;
        lr1s = newLR1s;
    }
}

void Grammar::Go(const LR1s &lr1s, const string &symbol, LR1s &newLR1s)
{
    for (auto const &lr1 : lr1s)
    {
        string nonterminal = lr1.first.first;            // 非终结符
        vector<string> pro = lr1.first.second;           // 产生式
        int index = lr1.second.first;                    // 圆点的位置
        string nextSymbols = lr1.second.second;          // 向前看符号
        if (index == pro.size() || pro[index] != symbol) // 圆点在产生式最后或者圆点后面不是symbol
            continue;
        // 加入新的LR(1)项目
        newLR1s.insert({{nonterminal, pro}, {index + 1, nextSymbols}});
    }
    Closure(newLR1s);
}

void Grammar::LivePreDFA()
{
    // 构造识别所有活前缀的DFA
    LR1s tmp = {{{startSymbol, productions[startSymbol][0]}, {0, "$"}}};
    Closure({tmp});
    DFAStates.push_back(tmp);
    int index = 0;
    while (true)
    {
        LR1s lr1s = DFAStates[index];
        for (const auto &symbol : nonterminals)
        {
            LR1s newLR1s = {};
            Go(lr1s, symbol, newLR1s);
            if (newLR1s.size() != 0)
            {
                bool flag = true;
                for (const auto &lr1s1 : DFAStates)
                    if (lr1s1 == newLR1s)
                    {
                        flag = false;
                        break;
                    }
                if (flag)
                {
                    DFAStates.push_back(newLR1s);
                    DFAStateTrans[index][symbol] = DFAStates.size() - 1;
                }
                else
                    DFAStateTrans[index][symbol] = distance(DFAStates.begin(), find(DFAStates.begin(), DFAStates.end(), newLR1s));
            }
        }
        for (const auto &symbol : terminals)
        {
            LR1s newLR1s = {};
            Go(lr1s, symbol, newLR1s);
            if (newLR1s.size() != 0)
            {
                bool flag = true;
                for (const auto &lr1s1 : DFAStates)
                    if (lr1s1 == newLR1s)
                    {
                        flag = false;
                        break;
                    }
                if (flag)
                {
                    DFAStates.push_back(newLR1s);
                    DFAStateTrans[index][symbol] = DFAStates.size() - 1;
                }
                else
                    DFAStateTrans[index][symbol] = distance(DFAStates.begin(), find(DFAStates.begin(), DFAStates.end(), newLR1s));
            }
        }
        if (index == DFAStates.size() - 1)
            break;
        index++;
    }

    // 输出DFA状态集
    cout << "====DFA States====" << endl;
    for (int i = 0; i < DFAStates.size(); i++)
    {
        cout << "I" << i << ":" << endl;
        unordered_map<LR1, bool, LR1sHash> isDoned; // 判断该LR(1)项目是否已经输出
        for (const auto &lr1 : DFAStates[i])
        {
            if (isDoned[lr1])
                continue;
            cout << lr1.first.first << " -> ";
            for (int j = 0; j < lr1.first.second.size(); j++)
            {
                if (j == lr1.second.first)
                    cout << ".";
                cout << lr1.first.second[j] << " ";
            }
            if (lr1.second.first == lr1.first.second.size())
                cout << ".";
            cout << ", " << lr1.second.second << " ";
            for (const auto &lr1t : DFAStates[i])
                if (lr1.first == lr1t.first && lr1.second.first == lr1t.second.first && lr1.second.second != lr1t.second.second)
                {
                    isDoned[lr1t] = true;
                    cout << lr1t.second.second << " ";
                }
            cout << endl;
        }
        cout << endl;
    }
    cout << endl;

    // 输出DFA状态转移
    cout << "====DFA Transitions====" << endl;
    for (auto const &DFAStatesTran : DFAStateTrans)
    {
        int cnt = 0;
        for (auto const &symbol : DFAStatesTran.second)
            if (cnt < 4)
            {
                cout << "I" << DFAStatesTran.first << " -> " << symbol.first << " -> I" << symbol.second << "    ";
                cnt++;
            }
            else
            {
                cout << endl
                     << "I" << DFAStatesTran.first << " -> " << symbol.first << " -> I" << symbol.second << "    ";
                cnt = 0;
            }
        cout << endl;
    }
    cout << endl;
}

bool Grammar::IsLR1()
{
    for (const auto &state : DFAStates)
    {
        for (const auto &lr1 : state)
        {
            if (lr1.second.first == lr1.first.second.size())
            {
                for (const auto &lr1t : state)
                {
                    int index = lr1t.second.first;
                    // 移进-规约冲突
                    if (index != lr1t.first.second.size() && lr1t.first.second[index] == lr1.second.second)
                        return false;
                    // 规约-规约冲突
                    else if (lr1t != lr1 && index == lr1t.first.second.size() && lr1t.second.second == lr1.second.second)
                        return false;
                }
            }
        }
    }
    return true;
}