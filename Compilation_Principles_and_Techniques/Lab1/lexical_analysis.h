#ifndef LEICAL_ANALYSIS_H
#define LEICAL_ANALYSIS_H

#include <fstream>
#include <iostream>
#include "token.h"

using namespace std;

// 错误类型
enum ErrorType
{
    UNKNOWN_SYMBOL,
    ILLEGAL_NUMBERS,
    UNCLOSED_STRING,
    UNCLOSED_CHAR,
    ILLEGAL_ESCAPE,
    UNCLOSED_ANNOTATION
};

/**
 * @brief 词法分析
 * 
 * @param fin 输入文件流 
 * @param char_num 读入字符数
 * @param line 当前行数
 * @param col 当前列数
 * @param token_streams 记号流
 * @param id_table 标识符表
 * @param str_table 字符串表
 */
void lexical_analysis(ifstream &fin, int &char_num, int &line, int &col, vector<Token> &token_streams, vector<string> &id_table, vector<string> &str_table);

/**
 * @brief 判断是否为字母
 *
 * @param c 当前读入字符
 */
bool is_letter(char c);

/**
 * @brief 判断是否为数字
 *
 * @param c 当前读入字符
 */
bool is_digit(char c);

/**
 * @brief 向记号流添加记号
 * 
 * @param buffer 当前读入的记号
 * @param token_streams 记号流
 * @param type 记号类型
 * @param id_table 标识符表
 * @param str_table 字符串表 
 * @param line 当前行数
 * @param col 当前列数
 */
void add_token(string buffer, vector<Token> &token_streams, const TokenType type, vector<string> &id_table, vector<string> &str_table, const int line, const int col);

/**
 * @brief 判断是否为关键字，若是则返回关键词的下标，否则返回-1
 *
 * @param buffer 当前读入的记号
 * @return int
 */
int is_keyword(const string &buffer);

/**
 * @brief 在表中查找是否有str，若有则返回下标，否则将str插入表中再返回下标
 *
 * @param table 参照表
 * @param str 查找的字符串
 * @return unsigned int
 */
unsigned int find(vector<string> &table, const string &str);

/**
 * @brief 在运算符表中查找运算符str的下标
 *
 * @param str 查找的运算符
 * @return unsigned int
 */
unsigned int find_operator(const string &str);

/**
 * @brief
 *
 * @param fin 输入文件流
 * @param char_num 读入字符数
 * @param c 当前读入的字符
 * @param buffer 当前缓冲区
 * @param col 当前列数
 */
void next_char(ifstream &fin, int &char_num, char &c, string &buffer, int &col);

/**
 * @brief 回退一个字符
 * 
 * @param fin 输入文件流 
 * @param char_num 读入字符数 
 * @param col 当前列数
 */
void go_back(ifstream &fin, int &char_num, int &col);

/**
 * @brief 输出错误信息
 * 
 * @param error_type 错误类型
 * @param line 当前行数
 * @param col 当前列数
 * @param buffer 错误内容 
 */
void error(ErrorType error_type, int line, int col, const string buffer);

#endif // LEICAL_ANALYSIS_H