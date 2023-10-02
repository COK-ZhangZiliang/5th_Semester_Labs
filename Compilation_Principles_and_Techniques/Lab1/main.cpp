#include <iostream>
#include <fstream>
#include "lexical_analysis.h"
#include "output.h"

using namespace std;

int main()
{
    // 读入源程序文件
    string file_name;
    cin >> file_name;
    ifstream fin;
    fin.open("test/" + file_name, ios::in);
    if (!fin.is_open())
    {
        cout << "Error opening file";
        exit(1);
    }
    
    int char_num = 0; // 读入字符数
    int line = 0; // 当前行数
    int col = 0; // 当前列数
    vector<Token> token_streams; // 记号流
    vector<string> id_table; // 标识符表
    vector<string> str_table; // 字符串表
    lexical_analysis(fin, char_num, line, col, token_streams, id_table, str_table);
    output(token_streams, id_table, str_table, char_num, line);
}