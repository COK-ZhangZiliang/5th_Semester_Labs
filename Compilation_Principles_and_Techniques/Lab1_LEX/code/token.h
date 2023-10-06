#ifndef TOKEN_H
#define TOKEN_H

// C语言记号类型
enum TokenType
{
    KEYWORD,              // 关键字
    ID,                   // 标志符
    STRING,               // 字符串常量
    CHAR,                 // 字符常量
    INT,                  // 整型常量
    UINT,                 // 无符号整型常量
    LONG,                 // 长整型常量
    ULONG,                // 无符号长整型
    LONGLONG,             // 长长整型常量
    ULONGLONG,            // 无符号长长整型常量
    FLOAT,                // 单精度浮点数
    DOUBLE,               // 双精度浮点数
    LONGDOUBLE,           // 长双精度浮点数
    RELATION_OPERATOR,    // 关系运算符
    ASSIGN_OPERATOR,      // 赋值运算符
    AGORITHM_OPERATOR,    // 算术运算符
    LOGICAL_OPERATOR,     // 逻辑运算符
    BITWISE_OPERATOR,     // 位运算符
    QUESTION_MARK,        //"?"
    COLON,                //":"
    SEMICOLON,            //";"
    LEFT_SQUARE_BRACKET,  //"["
    RIGHT_SQUARE_BRACKET, //"]"
    LEFT_PARENTHESE,      //"("
    RIGHT_PARENTHESE,     //")"
    LEFT_BRACE,           //"{"
    RIGHT_BRACE,          //"}"
    DOT,                  //"."
    COMMA,                //","
    ARROW,                //"->"
    ANNOTATION,           // 注释
};

// 记号
struct Token
{
    enum TokenType type;  // 记号类型
    char value[200]; // 记号值
    int line;        // 记号所在行数
    int column;      // 记号所在列数
};

#endif // TOKEN_H