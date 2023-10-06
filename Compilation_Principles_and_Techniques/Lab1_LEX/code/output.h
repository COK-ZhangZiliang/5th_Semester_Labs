#ifndef OUTPUT_H
#define OUTPUT_H

#include "token.h"
#include <stdio.h>

/**
 * @brief 输出记号流
 * 
 * @param token_streams 记号流 
 * @param char_num 字符数
 * @param line 行数
 * @param token_num 记号数 
 */
void output(struct Token *token_streams, int char_num, int line, int token_num);

/**
 * @brief 输出记号 
 * 
 * @param type 记号类型 
 * @param value 记号值 
 */
void print(enum TokenType type, char *value);

#endif // OUTPUT_H