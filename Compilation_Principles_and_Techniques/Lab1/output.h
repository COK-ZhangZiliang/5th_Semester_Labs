#ifndef OUTPUT_H
#define OUTPUT_H

#include "token.h"
#include <iostream>

int keyword_num = 0; // 关键字数
int id_num = 0; // 标识符数
int str_num = 0; // 字符串数
int _char_num = 0; // 字符数
int int_num = 0; // 整数数
int float_num = 0; // 浮点数数
int operator_num = 0; // 运算符数
int other_num = 0; // 其他记号数

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