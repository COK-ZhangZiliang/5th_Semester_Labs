#ifndef TOKEN_H
#define TOKEN_H

#include <vector>
#include <string>

using namespace std;

// C语言关键字
const vector<string> KeyWord = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double",
    "else", "enum", "extern", "float", "for", "goto", "if", "inline", "int", "long",
    "register", "restrict", "return", "short", "signed", "sizeof", "static", "struct",
    "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "_Bool",
    "_Complex", "_Imaginary"};

// C语言运算符
const vector<string> Operator = {
    "+", "-", "*", "/", "%", "++", "--", "&&", "||", "!", "&", "|", "~", "^", "<<",
    ">>", "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", "==",
    "!=", ">", "<", ">=", "<="};

// C语言记号类型
enum TokenType
{
    KEYWORD,           // 关键字
    ID,                // 标志符
    STRING,            // 字符串常量
    CHAR,              // 字符常量
    INT,               // 整型常量
    UINT,              // 无符号整型常量
    LONG,              // 长整型常量
    ULONG,             // 无符号长整型
    LONGLONG,          // 长长整型常量
    ULONGLONG,         // 无符号长长整型常量
    FLOAT,             // 单精度浮点数
    DOUBLE,            // 双精度浮点数
    LONGDOUBLE,        // 长双精度浮点数
    RELATION_OPERATOR, // 关系运算符
    ASSIGN_OPERATOR,   // 赋值运算符
    AGORITHM_OPERATOR, // 算术运算符
    // PLUS,                 //"+"
    // MINUS,                //"-"
    // MULTIPLY,             //"*"
    // DIVIDE,               //"/"
    // MOD,                  //"%"
    // INC,                  //"++"
    // DEC,                  //"--"
    LOGICAL_OPERATOR, // 逻辑运算符
    // LOGICAL_AND,          //"&&"
    // LOGICAL_OR,           //"||"
    // LOGICAL_NEGATION,     //"!"
    BITWISE_OPERATOR, // 位运算符
    // BITWISE_AND,          //"&"
    // BITWISE_OR,           //"|"
    // BITWISE_NEGATION,     //"~"
    // BITWISE_XOR,          //"^"
    // BITWISE_LSHIFT,       //"<<"
    // BITWISE_RSHIFT,       //">>"
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

    // // 关系运算符属性
    // GREATER,       //">"
    // GREATER_EQUAL, //">="
    // LESS,          //"<"
    // LESS_EQUAL,    //"<="
    // EQUAL,         //"=="
    // UNEQUAL,       //"!="

    // // 赋值运算符属性
    // SIMPLE_EQUAL,   //"="
    // PLUS_EQUAL,     //"+="
    // MINUS_EQUAL,    //"-="
    // MULTIPLY_EQUAL, //"*="
    // DIVIDE_EQUAL,   //"/="
    // MOD_EQUAL,      //"%="
    // AND_EQUAL,      //"&="
    // OR_EQUAL,       //"|="
    // XOR_EQUAL,      //"^="
    // LSHIFT_EQUAL,   //"<<="
    // RSHIFT_EQUAL,   //">>="
};

union ValueType
{
    char charValue;
    int intValue;
    unsigned int uintValue;
    long longValue;
    unsigned long ulongValue;
    long long longlongValue;
    unsigned long long ulonglongValue;
    float floatValue;
    double doubleValue;
    long double longdoubleValue;
};

struct Token
{
    TokenType type;  // 记号类型
    ValueType value; // 记号值
    int line;        // 记号所在行数
    int column;      // 记号所在列数
};

#endif // TOKEN_H