#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include "token.h"

// 错误类型
enum ErrorType
{
    UNKNOWN_SYMBOL,     // 未知符号
    UNCLOSED_STRING,    // 未闭合字符串
    UNCLOSED_CHAR,      // 未闭合字符
    EMPTY_CHAR,         // 空字符
    UNCLOSED_ANNOTATION // 未闭合注释
};

/**
 * @brief 打印错误信息
 * 
 * @param error_type 错误类型
 * @param line 行号
 * @param col 列号
 * @param buffer 错误内容
 */
void error(enum ErrorType error_type, int line, int col, char* buffer);

#endif // ERROR_H