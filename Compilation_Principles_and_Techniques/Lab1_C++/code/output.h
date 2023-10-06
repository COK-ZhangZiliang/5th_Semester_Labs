#ifndef OUTPUT_H
#define OUTPUT_H

#include "token.h"
#include <iostream>

/**
 * @brief 输出记号流
 * 
 * @param token_streams 记号流  
 * @param id_table 标识符表
 * @param str_table 字符串表
 * @param char_num 字符总数
 * @param line 行数
 */
void output(const vector<Token> &token_streams, const vector<string> &id_table, const vector<string> &str_table, const int char_num, const int line);

/**
 * @brief 输出记号
 * 
 * @param type 记号类型
 * @param value 记号属性
 * @param id_table 标识符表
 * @param str_table 字符串表 
 */
void print(const TokenType type, const ValueType value, const vector<string> &id_table, const vector<string> &str_table);

#endif // OUTPUT_H