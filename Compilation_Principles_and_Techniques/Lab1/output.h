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
 */
void output(const vector<Token> &token_streams, const vector<string> &id_table, const vector<string> &str_table);

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