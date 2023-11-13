/**
 * @file main.cpp
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief 预测分析控制程序
 * @date 2023-10-28
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

    // 转换为LL(1)文法
    Grammar grammar;
    grammar.GetGrammar(fin);
    grammar.CovertGrammar();
    fin.close();

    // 构造预测分析表
    Table table;
    table.BuildTable(grammar);
    table.OutputTable();

    // 预测分析控制程序
    while (true)
    {
        string sentence;
        cout << "Please input the sentence(without spaces and input '$' to end): ";
        cin >> sentence;
        if (sentence == "$")
            break;
        sentence += "$";
        stack<string> stk;
        stk.push("$");
        int index = 0;
        stk.push(grammar.startSymbol);
        while (!stk.empty())
        {
            // top为栈顶元素，next为输入串的下一个符号
            string top = stk.top();
            string next;
            if (index >= sentence.length())
                next = "";
            else
                next = sentence.substr(index, 1);
            if (next[0] >= '0' && next[0] <= '9')
            {
                while (sentence[index] >= '0' && sentence[index] <= '9')
                    index++;
                index--;
                next = "num";
            }

            // 如果栈顶元素为终结符号或$
            if (grammar.terminals.find(top) != grammar.terminals.end() || top == "$")
            {
                if (top == next)
                {
                    stk.pop();
                    index++;
                }
                else
                {
                    table.Error(top, next);
                    stk.pop();
                }
            }
            // 如果栈顶元素为非终结符号
            else
            {
                // 如果预测分析表中存在对应的产生式
                if (table.parsingTable.at(top).find(next) != table.parsingTable.at(top).end())
                {
                    stk.pop();
                    vector<string> production = table.parsingTable.at(top).at(next);
                    // 将产生式逆序入栈
                    for (auto it = production.rbegin(); it != production.rend(); it++)
                        if (*it != eps)
                            stk.push(*it);
                    cout << top << " -> ";
                    for (auto const &str : production)
                        cout << str << " ";
                    cout << endl;
                }
                else
                {
                    table.Error(top, next);
                    if (grammar.follow[top].find(next) != grammar.follow[top].end())
                        stk.pop();
                    else
                        index++;
                }
            }
        }
        cout << "success!" << endl;
        cout << endl;
    }

    system("pause");
    return 0;
}