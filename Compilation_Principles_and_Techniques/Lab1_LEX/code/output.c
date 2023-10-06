#include "output.h"

int keyword_num = 0;  // 关键字数
int id_num = 0;       // 标识符数
int str_num = 0;      // 字符串数
int _char_num = 0;    // 字符数
int int_num = 0;      // 整数数
int float_num = 0;    // 浮点数数
int operator_num = 0; // 运算符数
int other_num = 0;    // 其他记号数

void output(struct Token *token_streams, int char_num, int line, int token_num)
{
    printf("========================Token Streams========================\n");
    int i = 0;
    while (i < token_num)
    {
        for (int j = 0; j < 2 && i < token_num; i++, j++)
        {
            printf("<%d:%d>", token_streams[i].line, token_streams[i].column);
            print(token_streams[i].type, token_streams[i].value);
            printf("\t");
        }
        puts("");
    }
    puts("");

    printf("========Analysis Result========\n");
    printf("Total lines: %d\n", line + 1);
    printf("Total characters: %d\n", char_num);
    printf("Total tokens: %d\n", token_num);
    printf("Total keywords: %d\n", keyword_num);
    printf("Total identifiers: %d\n", id_num);
    printf("Total const strings: %d\n", str_num);
    printf("Total const characters: %d\n", _char_num);
    printf("Total const integers: %d\n", int_num);
    printf("Total const floats: %d\n", float_num);
    printf("Total operators: %d\n", operator_num);
    printf("Total others: %d\n", other_num);
}

void print(enum TokenType type, char *value)
{
    switch (type)
    {
    case KEYWORD:
        keyword_num++;
        printf("<Keyword, %s>", value);
        break;
    case ID:
        id_num++;
        printf("<Identifier, %s>", value);
        break;
    case STRING:
        str_num++;
        printf("<String, %s>", value);
        break;
    case CHAR:
        _char_num++;
        printf("<Char, %s>", value);
        break;
    case INT:
        int_num++;
        printf("<Int, %s>", value);
        break;
    case UINT:
        int_num++;
        printf("<Unsigned Int, %s>", value);
        break;
    case LONG:
        int_num++;
        printf("<Long, %s>", value);
        break;
    case ULONG:
        int_num++;
        printf("<Unsigned Long, %s>", value);
        break;
    case LONGLONG:
        int_num++;
        printf("<Long Long, %s>", value);
        break;
    case ULONGLONG:
        int_num++;
        printf("<Unsigned Long Long, %s>", value);
        break;
    case FLOAT:
        float_num++;
        printf("<Float, %s>", value);
        break;
    case DOUBLE:
        float_num++;
        printf("<Double, %s>", value);
        break;
    case LONGDOUBLE:
        float_num++;
        printf("<Long Double, %s>", value);
        break;
    case RELATION_OPERATOR:
        operator_num++;
        printf("<Relation Operator, %s>", value);
        break;
    case ASSIGN_OPERATOR:
        operator_num++;
        printf("<Assign Operator, %s>", value);
        break;
    case AGORITHM_OPERATOR:
        operator_num++;
        printf("<Agorithm Operator, %s>", value);
        break;
    case LOGICAL_OPERATOR:
        operator_num++;
        printf("<Logical Operator, %s>", value);
        break;
    case BITWISE_OPERATOR:
        operator_num++;
        printf("<Bitwise Operator, %s>", value);
        break;
    case QUESTION_MARK:
        other_num++;
        printf("<Question Mark, %s>", value);
        break;
    case COLON:
        other_num++;
        printf("<Colon, %s>", value);
        break;
    case SEMICOLON:
        other_num++;
        printf("<Semicolon, %s>", value);
        break;
    case LEFT_SQUARE_BRACKET:
        other_num++;
        printf("<Left Square Bracket, %s>", value);
        break;
    case RIGHT_SQUARE_BRACKET:
        other_num++;
        printf("<Right Square Bracket, %s>", value);
        break;
    case LEFT_PARENTHESE:
        other_num++;
        printf("<Left Parenthese, %s>", value);
        break;
    case RIGHT_PARENTHESE:
        other_num++;
        printf("<Right Parenthese, %s>", value);
        break;
    case LEFT_BRACE:
        other_num++;
        printf("<Left Brace, %s>", value);
        break;
    case RIGHT_BRACE:
        other_num++;
        printf("<Right Brace, %s>", value);
        break;
    case DOT:
        other_num++;
        printf("<Dot, %s>", value);
        break;
    case COMMA:
        other_num++;
        printf("<Comma, %s>", value);
        break;
    case ARROW:
        other_num++;
        printf("<Arrow, %s>", value);
        break;
    }
}