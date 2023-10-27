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

void Grammar::EliminateLeftRecursion()
{
    for (const auto &nonter : nonterminals)
    {
        bool isLeftRecursion = false;
        for (const auto &pro : productions[nonter])
        {
            if (pro[0] == nonter)
                isLeftRecursion = true;
            break;
        }
        // 若存在左递归
        if (isLeftRecursion)
        {
            unordered_map<string, vector<vector<string>>> tmpPro = productions;
            productions[nonter].clear();
            nonterminals.insert(nonter + "'");
            productions[nonter + "'"].push_back({eps});
            for (const auto &pro : tmpPro[nonter])
            {
                if (pro[0] == nonter)
                {
                    vector<string> tmp;
                    for (int i = 1; i < pro.size(); i++)
                        tmp.push_back(pro[i]);
                    tmp.push_back(nonter + "'");
                    productions[nonter + "'"].push_back(tmp);
                }
                else
                {
                    vector<string> tmp = pro;
                    tmp.push_back(nonter + "'");
                    productions[nonter].push_back(tmp);
                }
            }
        }
    }
}

void Grammar::ExtractLeftCommon()
{
    for (const auto &nonter : nonterminals)
    {
        bool haveLeftCommon = false;
        do
        {
            vector<string> leftCommon;
            // 判断是否有左公因子
            for (auto it = productions[nonter].begin(); it != productions[nonter].end(); it++)
            {
                for (auto it2 = it + 1; it2 != productions[nonter].end(); it2++)
                {
                    if ((*it)[0] == (*it2)[0])
                    {
                        leftCommon.push_back((*it)[0]);
                        haveLeftCommon = true;
                        break;
                    }
                }
                if (haveLeftCommon)
                    break;
            }
            if (haveLeftCommon)
            {
                string newNonter = "A";
                while (nonterminals.find(newNonter) != nonterminals.end())
                    newNonter[0]++;
                nonterminals.insert(newNonter);
                vector<vector<string>> tmp;
                // 找到所有含有左公因子的产生式
                for (const auto &pro : productions[nonter])
                {
                    if (pro[0] == leftCommon[0])
                    {
                        tmp.push_back(pro);
                        productions[nonter].erase(find(productions[nonter].begin(), productions[nonter].end(), pro));
                    }
                }
                // 求解最长左公因子
                int maxLen = 1;
                while (true)
                {
                    bool isSame = true;
                    string cur = tmp[0][maxLen];
                    for (const auto &pro : tmp)
                        if (maxLen >= pro.size() || pro[maxLen] != cur)
                        {
                            isSame = false;
                            break;
                        }
                    if (!isSame)
                        break;
                    leftCommon.push_back(cur);
                    maxLen++;
                }
                // 构造新的产生式
                leftCommon.push_back(newNonter);
                productions[nonter].push_back(leftCommon);
                for (const auto &pro : tmp)
                    productions[newNonter].push_back(vector<string>(pro.begin() + maxLen, pro.end()));
            }
        } while (haveLeftCommon);
    }
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
            // fir是非终结符，递归构造
            BuildFirst(fir);
            candidateFirst[nonterminal][index].insert(first[fir].begin(), first[fir].end());
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

void Grammar::BuildFollow(const string &nonterminal)
{
    if (followDoned[nonterminal])
        return;
    for (const auto &nonter : nonterminals)
    {
        for (const auto &pro : productions[nonter])
        {
            if (auto it = find(pro.begin(), pro.end(), nonterminal); it != pro.end())
            {
                int index = it - pro.begin() + 1;
                for (; index < pro.size(); index++)
                {
                    if (terminals.find(pro[index]) != terminals.end()) // pro[index]是终结符
                    {
                        follow[nonterminal].insert(pro[index]);
                        break;
                    }
                    // pro[index]是非终结符
                    follow[nonterminal].insert(first[pro[index]].begin(), first[pro[index]].end());
                    follow[nonterminal].erase(eps);
                    if (first[pro[index]].find(eps) == first[pro[index]].end()) // 若pro[index]的FIRST集不包含epsilon，无需继续往后构造
                        break;
                }
                if (index == pro.size() && nonter != nonterminal)
                {
                    followRelation[nonterminal][nonter] = true;
                    if (!followRelation[nonter][nonterminal])
                        BuildFollow(nonter);
                    follow[nonterminal].insert(follow[nonter].begin(), follow[nonter].end());
                }
            }
        }
    }
    followDoned[nonterminal] = true;
}

void Grammar::CovertGrammar()
{
    // 消除左递归
    EliminateLeftRecursion();
    // 输出消除左递归后的文法
    cout << "====After Eliminate Left Recursion====" << endl;
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

    // 提取左公因子
    ExtractLeftCommon();
    // 输出提取左公因子后的文法
    cout << "====After Extract Left Common====" << endl;
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

    // 构造FIRST集
    for (const auto &nonterminal : nonterminals)
        if (!firstDoned[nonterminal])
            BuildFirst(nonterminal);
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

    // 构造FOLLOW集
    follow[startSymbol].insert("$");
    for (const auto &nonterminal : nonterminals)
        if (!followDoned[nonterminal])
            BuildFollow(nonterminal);
    // 处理FOLLOW集相互包含的情况
    for (const auto &nonterminal1 : nonterminals)
        for (const auto &nonterminal2 : nonterminals)
            if (followRelation[nonterminal1][nonterminal2] && followRelation[nonterminal2][nonterminal1])
            {
                follow[nonterminal1].insert(follow[nonterminal2].begin(), follow[nonterminal2].end());
                follow[nonterminal2].insert(follow[nonterminal1].begin(), follow[nonterminal1].end());
            }
    // 输出FOLLOW集
    cout << "====FOLLOW Sets====" << endl;
    for (const auto &nonterminal : nonterminals)
    {
        cout << nonterminal << ": ";
        for (const auto &fol : follow[nonterminal])
            cout << fol << " ";
        cout << endl;
    }
    cout << endl;

    // 判断是否是LL(1)文法
    if (IsLL1())
        cout << "The grammar can be converted to a LL(1) grammar." << endl;
    else
        cout << "The grammar can't be converted to a LL(1) grammar." << endl;
    cout << endl;
}

bool Grammar::IsLL1() const
{
    for (const auto &nonter : nonterminals)
    {
        for (auto it = candidateFirst.at(nonter).begin(); it != candidateFirst.at(nonter).end(); it++)
        {
            for (auto it2 = it + 1; it2 != candidateFirst.at(nonter).end(); it2++)
            {
                // 判断FIRST[*it]和FIRST[*it2]是否有交集
                unordered_set<string> tmp;
                set_intersection(it->begin(), it->end(), it2->begin(), it2->end(), inserter(tmp, tmp.begin()));
                if (!tmp.empty())
                    return false;
                // 若epsilon在FIRST[*it]中，判断FIRST[*it2]和FOLLOW[nonter]是否有交集
                if (it->find(eps) != it->end())
                {
                    unordered_set<string> tmp;
                    set_intersection(it2->begin(), it2->end(), follow.at(nonter).begin(), follow.at(nonter).end(), inserter(tmp, tmp.begin()));
                    if (!tmp.empty())
                        return false;
                }
                // 若epsilon在FIRST[*it2]中，判断FIRST[*it]和FOLLOW[nonter]是否有交集
                if (it2->find(eps) != it2->end())
                {
                    unordered_set<string> tmp;
                    set_intersection(it->begin(), it->end(), follow.at(nonter).begin(), follow.at(nonter).end(), inserter(tmp, tmp.begin()));
                    if (!tmp.empty())
                        return false;
                }
            }
        }
    }
    return true;
}