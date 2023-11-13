/**
 * @file main.cpp
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief LR(1)分析控制程序
 * @date 2023-11-13
 */
#include "grammar.h"
#include "table.h"
#include <iostream>
#include <stack>

using namespace std;

int main()
{
    // 读入文法
    string file_name;
    cout << "Please input the grammar file name: ";
    cin >> file_name;
    file_name = "../grammar/" + file_name;
    ifstream fin;
    fin.open(file_name, ios::in);
    if (!fin.is_open())
    {
        cout << "Error opening file";
        exit(1);
    }

    // 构造识别该文法所有活前缀的DFA
    Grammar grammar;
    grammar.GetGrammar(fin);
    grammar.ExtendGrammar();
    grammar.First();
    grammar.LivePreDFA();
    if (grammar.IsLR1())
        cout << "This grammar is LR(1) grammar." << endl;
    else
        cout << "This grammar is not LR(1) grammar." << endl;
    cout << endl;
    fin.close();

    // 构造分析表
    Table table;
    table.BuildTable(grammar);
    table.OutputTable();

    // LR(1)分析控制程序
    while (true)
    {
        string sentence;
        cout << "Please input the sentence(without spaces and input '$' to end): ";
        cin >> sentence;
        if (sentence == "$")
            break;
        sentence += "$";
        stack<pair<int, string>> stk;
        stk.push({0, "-"});
        int index = 0;
        while (true)
        {
            // state为栈顶状态，next为输入串的下一个符号
            int state = stk.top().first;
            string next = sentence.substr(index, 1);
            if (next[0] >= '0' && next[0] <= '9')
            {
                while (sentence[index] >= '0' && sentence[index] <= '9')
                    index++;
                index--;
                next = "num";
            }

            if (table.parsingTable[state].find(next) == table.parsingTable[state].end())
                table.Error(state, next, index, sentence);
            else if (table.parsingTable[state][next] == acc)
                break;
            else
            {
                if (table.parsingTable[state][next] >= 0)
                {
                    // 移进
                    stk.push({table.parsingTable[state][next], next});
                    index++;
                }
                else
                {
                    // 规约
                    string nonter = table.productions[-table.parsingTable[state][next] - 1].first;
                    vector<string> pro = table.productions[-table.parsingTable[state][next] - 1].second;
                    for (int i = 0; i < pro.size(); i++)
                        stk.pop();
                    state = stk.top().first;
                    stk.push({table.parsingTable[state][nonter], nonter});
                    cout << nonter << " -> ";
                    for (int i = 0; i < pro.size(); i++)
                        cout << pro[i] << " ";
                    cout << endl;
                }
            }
        }
        cout << "success!" << endl;
        cout << endl;
    }

    system("pause");
    return 0;
}