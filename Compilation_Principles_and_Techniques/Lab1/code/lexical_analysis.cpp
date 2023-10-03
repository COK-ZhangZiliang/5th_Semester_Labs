#include "lexical_analysis.h"

void lexical_analysis(ifstream &fin, int &char_num, int &line, int &col, vector<Token> &token_streams, vector<string> &id_table, vector<string> &str_table)
{
    int state = 0;        // 状态
    string buffer;        // 缓冲区
    char c;               // 当前读入字符
    ErrorType error_type; // 错误类型
    int base = 10;        // 进制
    while (true)
    {
        switch (state)
        {
        case 0:
            next_char(fin, char_num, c, buffer, col);
            buffer.clear(); // 清空缓冲区
            if (is_letter(c) || c == '_')
                state = 1;
            else if (is_digit(c) && c != '0')
                state = 2;
            else if (c == '0')
                state = 3;
            else if (c == '.')
                state = 4;
            else if (c == '#')
                state = 5;
            else if (c == '"')
                state = 6;
            else if (c == '\'')
                state = 7;
            else if (c == '+')
                state = 8;
            else if (c == '-')
                state = 9;
            else if (c == '*')
                state = 10;
            else if (c == '/')
                state = 11;
            else if (c == '%')
                state = 12;
            else if (c == '&')
                state = 13;
            else if (c == '|')
                state = 14;
            else if (c == '^')
                state = 15;
            else if (c == '<')
                state = 16;
            else if (c == '>')
                state = 17;
            else if (c == '=')
                state = 18;
            else if (c == '!')
                state = 19;
            else
            {
                buffer += c;
                if (c == '~')
                    add_token(buffer, token_streams, BITWISE_OPERATOR, id_table, str_table, line, col, base);
                else if (c == '?')
                    add_token(buffer, token_streams, QUESTION_MARK, id_table, str_table, line, col, base);
                else if (c == ':')
                    add_token(buffer, token_streams, COLON, id_table, str_table, line, col, base);
                else if (c == ';')
                    add_token(buffer, token_streams, SEMICOLON, id_table, str_table, line, col, base);
                else if (c == '[')
                    add_token(buffer, token_streams, LEFT_SQUARE_BRACKET, id_table, str_table, line, col, base);
                else if (c == ']')
                    add_token(buffer, token_streams, RIGHT_SQUARE_BRACKET, id_table, str_table, line, col, base);
                else if (c == '(')
                    add_token(buffer, token_streams, LEFT_PARENTHESE, id_table, str_table, line, col, base);
                else if (c == ')')
                    add_token(buffer, token_streams, RIGHT_PARENTHESE, id_table, str_table, line, col, base);
                else if (c == '{')
                    add_token(buffer, token_streams, LEFT_BRACE, id_table, str_table, line, col, base);
                else if (c == '}')
                    add_token(buffer, token_streams, RIGHT_BRACE, id_table, str_table, line, col, base);
                else if (c == ',')
                    add_token(buffer, token_streams, COMMA, id_table, str_table, line, col, base);
                else if (c == ' ' || c == '\t')
                    ;
                else if (c == '\n')
                    line++, col = 0;
                else if (c == EOF)
                {
                    line++;
                    char_num--;
                    return;
                }
                else
                {
                    buffer.erase();
                    error_type = UNKNOWN_SYMBOL;
                    state = 21;
                }
            }
            break;

        // 标识符或关键字
        case 1:
            next_char(fin, char_num, c, buffer, col);
            if (is_letter(c) || is_digit(c) || c == '_')
                state = 1;
            else
            {
                go_back(fin, char_num, col);
                // 判断是否是关键字
                int index = is_keyword(buffer);
                if (index != -1)
                    add_token(buffer, token_streams, KEYWORD, id_table, str_table, line, col, base);
                else
                    add_token(buffer, token_streams, ID, id_table, str_table, line, col, base);
                state = 0;
            }
            break;

        // 常数
        case 2:
            base = 10;
            next_char(fin, char_num, c, buffer, col);
            if (is_digit(c))
                state = 2;
            else if (c == '.')
                state = 22;
            else if (c == 'e' || c == 'E')
                state = 23;
            else if (c == 'u' || c == 'U')
                state = 30;
            else if (c == 'l' || c == 'L')
                state = 31;
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, INT, id_table, str_table, line, col, base);
                state = 0;
            }
            break;

        // 以0开头的常数
        case 3:
            next_char(fin, char_num, c, buffer, col);
            if (c == '.')
                state = 22;
            else if (c == 'e' || c == 'E')
                state = 23;
            else if (c >= '0' && c <= '7')
                state = 26;
            else if (c == '8' || c == '9')
                state = 27;
            else if (c == 'x' || c == 'X')
                state = 28;
            else if (c == 'u' || c == 'U')
                state = 30;
            else if (c == 'l' || c == 'L')
                state = 31;
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, INT, id_table, str_table, line, col, base);
                state = 0;
            }
            break;

        // 以'.'开头的纯小数
        case 4:
            next_char(fin, char_num, c, buffer, col);
            if (is_digit(c))
                state = 22;
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, DOT, id_table, str_table, line, col, base);
                state = 0;
            }
            break;

        // 头文件或宏定义
        case 5:
            char_num--;
            while (c != '\n')
                c = fin.get();
            go_back(fin, char_num, col);
            state = 0;
            break;

        // 字符串常量
        case 6:
            next_char(fin, char_num, c, buffer, col);
            if (c == '\\')
                state = 33;
            else if (c == '"')
            {
                buffer.erase(0, 1);
                add_token(buffer, token_streams, STRING, id_table, str_table, line, col, base);
                state = 0;
            }
            else if (c == '\n' || c == EOF)
            {
                error_type = UNCLOSED_STRING;
                state = 21;
            }
            else
                state = 6;
            break;

        // 字符常量
        case 7:
            next_char(fin, char_num, c, buffer, col);
            if (c == '\\')
                state = 34;
            else if (c == '\'')
            {
                error_type = EMPTY_CHAR;
                state = 21;
            }
            else if (c == '\n' || c == EOF)
            {
                error_type = UNCLOSED_CHAR;
                state = 21;
            }
            else
                state = 35;
            break;

        // +
        case 8:
            next_char(fin, char_num, c, buffer, col);
            if (c == '+')
            {
                buffer += c;
                add_token(buffer, token_streams, AGORITHM_OPERATOR, id_table, str_table, line, col, base);
            }
            else if (c == '=')
            {
                buffer += c;
                add_token(buffer, token_streams, ASSIGN_OPERATOR, id_table, str_table, line, col, base);
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, AGORITHM_OPERATOR, id_table, str_table, line, col, base);
            }
            state = 0;
            break;

        // -
        case 9:
            next_char(fin, char_num, c, buffer, col);
            if (c == '-')
            {
                buffer += c;
                add_token(buffer, token_streams, AGORITHM_OPERATOR, id_table, str_table, line, col, base);
            }
            else if (c == '=')
            {
                buffer += c;
                add_token(buffer, token_streams, ASSIGN_OPERATOR, id_table, str_table, line, col, base);
            }
            else if (c == '>')
            {
                buffer += c;
                add_token(buffer, token_streams, ARROW, id_table, str_table, line, col, base);
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, AGORITHM_OPERATOR, id_table, str_table, line, col, base);
            }
            state = 0;
            break;

        // *
        case 10:
            next_char(fin, char_num, c, buffer, col);
            if (c == '=')
            {
                buffer += c;
                add_token(buffer, token_streams, ASSIGN_OPERATOR, id_table, str_table, line, col, base);
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, AGORITHM_OPERATOR, id_table, str_table, line, col, base);
            }
            state = 0;
            break;

        // /
        case 11:
            next_char(fin, char_num, c, buffer, col);
            if (c == '=')
            {
                buffer += c;
                add_token(buffer, token_streams, ASSIGN_OPERATOR, id_table, str_table, line, col, base);
                state = 0;
            }
            else if (c == '/')
                state = 36;
            else if (c == '*')
                state = 37;
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, AGORITHM_OPERATOR, id_table, str_table, line, col, base);
                state = 0;
            }
            break;

        // %
        case 12:
            next_char(fin, char_num, c, buffer, col);
            if (c == '=')
            {
                buffer += c;
                add_token(buffer, token_streams, ASSIGN_OPERATOR, id_table, str_table, line, col, base);
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, AGORITHM_OPERATOR, id_table, str_table, line, col, base);
            }
            state = 0;
            break;

        // &
        case 13:
            next_char(fin, char_num, c, buffer, col);
            if (c == '&')
            {
                buffer += c;
                add_token(buffer, token_streams, LOGICAL_OPERATOR, id_table, str_table, line, col, base);
            }
            else if (c == '=')
            {
                buffer += c;
                add_token(buffer, token_streams, ASSIGN_OPERATOR, id_table, str_table, line, col, base);
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, BITWISE_OPERATOR, id_table, str_table, line, col, base);
            }
            state = 0;
            break;

        // |
        case 14:
            next_char(fin, char_num, c, buffer, col);
            if (c == '|')
            {
                buffer += c;
                add_token(buffer, token_streams, LOGICAL_OPERATOR, id_table, str_table, line, col, base);
            }
            else if (c == '=')
            {
                buffer += c;
                add_token(buffer, token_streams, ASSIGN_OPERATOR, id_table, str_table, line, col, base);
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, BITWISE_OPERATOR, id_table, str_table, line, col, base);
            }
            state = 0;
            break;

        // ^
        case 15:
            next_char(fin, char_num, c, buffer, col);
            if (c == '=')
            {
                buffer += c;
                add_token(buffer, token_streams, ASSIGN_OPERATOR, id_table, str_table, line, col, base);
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, BITWISE_OPERATOR, id_table, str_table, line, col, base);
            }
            state = 0;
            break;

        // <
        case 16:
            next_char(fin, char_num, c, buffer, col);
            if (c == '<')
                state = 39;
            else if (c == '=')
            {
                buffer += c;
                add_token(buffer, token_streams, RELATION_OPERATOR, id_table, str_table, line, col, base);
                state = 0;
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, RELATION_OPERATOR, id_table, str_table, line, col, base);
                state = 0;
            }
            break;

        // >
        case 17:
            next_char(fin, char_num, c, buffer, col);
            if (c == '>')
                state = 40;
            else if (c == '=')
            {
                buffer += c;
                add_token(buffer, token_streams, RELATION_OPERATOR, id_table, str_table, line, col, base);
                state = 0;
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, RELATION_OPERATOR, id_table, str_table, line, col, base);
                state = 0;
            }
            break;

        // =
        case 18:
            next_char(fin, char_num, c, buffer, col);
            if (c == '=')
            {
                buffer += c;
                add_token(buffer, token_streams, RELATION_OPERATOR, id_table, str_table, line, col, base);
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, ASSIGN_OPERATOR, id_table, str_table, line, col, base);
            }
            state = 0;
            break;

        // !
        case 19:
            next_char(fin, char_num, c, buffer, col);
            if (c == '=')
            {
                buffer += c;
                add_token(buffer, token_streams, RELATION_OPERATOR, id_table, str_table, line, col, base);
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, LOGICAL_OPERATOR, id_table, str_table, line, col, base);
            }
            state = 0;
            break;

        // 词法错误
        case 21:
            if (c == ' ' || c == '\t' || c == '\n' || c == EOF || c == ',' || c == ';')
            {
                go_back(fin, char_num, col);
                error(error_type, line, col, buffer);
                state = 0;
            }
            else
            {
                next_char(fin, char_num, c, buffer, col);
                state = 21;
            }
            break;

        // 小数
        case 22:
            next_char(fin, char_num, c, buffer, col);
            if (is_digit(c))
                state = 22;
            else if (c == 'e' || c == 'E')
                state = 23;
            else if (c == 'f' || c == 'F')
            {
                add_token(buffer, token_streams, FLOAT, id_table, str_table, line, col, base);
                state = 0;
            }
            else if (c == 'l' || c == 'L')
            {
                add_token(buffer, token_streams, LONGDOUBLE, id_table, str_table, line, col, base);
                state = 0;
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, DOUBLE, id_table, str_table, line, col, base);
                state = 0;
            }
            break;

        // 指数形式的小数
        case 23:
            next_char(fin, char_num, c, buffer, col);
            if (c == '+' || c == '-')
                state = 24;
            else if (is_digit(c))
                state = 25;
            else
            {
                error_type = ILLEGAL_NUMBERS;
                state = 21;
            }
            break;

        // 指数为有符号数
        case 24:
            next_char(fin, char_num, c, buffer, col);
            if (is_digit(c))
                state = 25;
            else
            {
                error_type = ILLEGAL_NUMBERS;
                state = 21;
            }
            break;

        // 指数的绝对值
        case 25:
            next_char(fin, char_num, c, buffer, col);
            if (is_digit(c))
                state = 25;
            else if (c == 'f' || c == 'F')
            {
                add_token(buffer, token_streams, FLOAT, id_table, str_table, line, col, base);
                state = 0;
            }
            else if (c == 'l' || c == 'L')
            {
                add_token(buffer, token_streams, LONGDOUBLE, id_table, str_table, line, col, base);
                state = 0;
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, DOUBLE, id_table, str_table, line, col, base);
                state = 0;
            }
            break;

        // 8进制常数
        case 26:
            base = 8;
            next_char(fin, char_num, c, buffer, col);
            if (c == '.')
                state = 22;
            else if (c == 'e' || c == 'E')
                state = 23;
            else if (c >= '0' && c <= '7')
                state = 26;
            else if (c == '8' || c == '9')
                state = 27;
            else if (c == 'u' || c == 'U')
                state = 30;
            else if (c == 'l' || c == 'L')
                state = 31;
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, INT, id_table, str_table, line, col, base);
                state = 0;
            }
            break;

        // 以0开头的实型常数
        case 27:
            next_char(fin, char_num, c, buffer, col);
            if (c == '.')
                state = 22;
            else if (c == 'e' || c == 'E')
                state = 23;
            else if (isdigit(c))
                state = 27;
            else
            {
                error_type = ILLEGAL_NUMBERS;
                state = 21;
            }
            break;

        // 16进制常数
        case 28:
            base = 16;
            next_char(fin, char_num, c, buffer, col);
            if (is_digit(c) || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F')
                state = 29;
            else
            {
                error_type = ILLEGAL_NUMBERS;
                state = 21;
            }
            break;
        case 29:
            next_char(fin, char_num, c, buffer, col);
            if (is_digit(c) || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F')
                state = 29;
            else if (c == 'u' || c == 'U')
                state = 30;
            else if (c == 'l' || c == 'L')
                state = 31;
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, INT, id_table, str_table, line, col, base);
                state = 0;
            }
            break;

        // UINT
        case 30:
            c = fin.get();
            char_num++;
            if (c == 'l' || c == 'L')
                state = 32;
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, UINT, id_table, str_table, line, col, base);
                state = 0;
            }
            break;

        // LONG
        case 31:
            c = fin.get();
            char_num++;
            if (c == 'l' || c == 'L')
            {
                add_token(buffer, token_streams, LONGLONG, id_table, str_table, line, col, base);
                state = 0;
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, LONG, id_table, str_table, line, col, base);
                state = 0;
            }
            break;

        // ULONG
        case 32:
            c = fin.get();
            char_num++;
            if (c == 'l' || c == 'L')
            {
                add_token(buffer, token_streams, ULONGLONG, id_table, str_table, line, col, base);
                state = 0;
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, ULONG, id_table, str_table, line, col, base);
                state = 0;
            }
            break;

        // 字符串常量中的转义字符
        case 33:
            next_char(fin, char_num, c, buffer, col);
            if (c == '0' || c == 'a' || c == 'b' || c == 't' || c == 'n' || c == 'v' || c == 'f' || c == 'r' || c == '"' || c == '\'' || c == '?' || c == '\\')
                state = 6;
            else
            {
                error_type = ILLEGAL_ESCAPE;
                state = 21;
            }
            break;

        // 字符常量为转移字符
        case 34:
            next_char(fin, char_num, c, buffer, col);
            if (c == '0' || c == 'a' || c == 'b' || c == 't' || c == 'n' || c == 'v' || c == 'f' || c == 'r' || c == '"' || c == '\'' || c == '?' || c == '\\')
                state = 35;
            else
            {
                error_type = ILLEGAL_ESCAPE;
                state = 21;
            }
            break;
        case 35:
            next_char(fin, char_num, c, buffer, col);
            if (c == '\'')
            {
                buffer.erase(0, 1);
                add_token(buffer, token_streams, CHAR, id_table, str_table, line, col, base);
                state = 0;
            }
            else
            {
                error_type = UNCLOSED_CHAR;
                state = 21;
            }
            break;

        // 单行注释
        case 36:
            next_char(fin, char_num, c, buffer, col);
            if (c == '\n' || c == EOF)
            {
                go_back(fin, char_num, col);
                state = 0;
            }
            else
                state = 36;
            break;

        // 多行注释
        case 37:
            next_char(fin, char_num, c, buffer, col);
            if (c == '*')
                state = 38;
            else if (c == EOF)
            {
                error_type = UNCLOSED_ANNOTATION;
                state = 21;
            }
            else if (c == '\n')
                line++, col = 0;
            else
                state = 37;
            break;
        case 38:
            next_char(fin, char_num, c, buffer, col);
            if (c == '/')
                state = 0;
            else if (c == EOF)
            {
                error_type = UNCLOSED_ANNOTATION;
                state = 21;
            }
            else if (c == '\n')
                line++, col = 0;
            else if (c == '*')
                state = 38;
            else
                state = 37;
            break;

        // <<
        case 39:
            next_char(fin, char_num, c, buffer, col);
            if (c == '=')
            {
                buffer += c;
                add_token(buffer, token_streams, ASSIGN_OPERATOR, id_table, str_table, line, col, base);
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, BITWISE_OPERATOR, id_table, str_table, line, col, base);
            }
            state = 0;
            break;

        // >>
        case 40:
            next_char(fin, char_num, c, buffer, col);
            if (c == '=')
            {
                buffer += c;
                add_token(buffer, token_streams, ASSIGN_OPERATOR, id_table, str_table, line, col, base);
            }
            else
            {
                go_back(fin, char_num, col);
                add_token(buffer, token_streams, BITWISE_OPERATOR, id_table, str_table, line, col, base);
            }
            state = 0;
            break;
        }
    }
}

bool is_letter(char c)
{
    if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
        return true;
    else
        return false;
}

bool is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return true;
    else
        return false;
}

void add_token(string buffer, vector<Token> &token_streams, const TokenType type, vector<string> &id_table, vector<string> &str_table, const int line, const int col, const int base)
{
    Token token;
    token.type = type;
    token.line = line + 1;
    token.column = col - buffer.size() + 1;
    switch (type)
    {
    case KEYWORD:
        token.value.uintValue = is_keyword(buffer);
        break;
    case ID:
        // 在符号表中查找
        token.value.uintValue = find(id_table, buffer);
        break;
    case STRING:
        // 在字符串表中查找
        token.value.uintValue = find(str_table, buffer);
        break;
    case CHAR:
        if (buffer.size() > 1)
        {
            switch (buffer[1])
            {
            case '0':
                token.value.charValue = '\0';
                break;
            case 'a':
                token.value.charValue = '\a';
                break;
            case 'b':
                token.value.charValue = '\b';
                break;
            case 't':
                token.value.charValue = '\t';
                break;
            case 'n':
                token.value.charValue = '\n';
                break;
            case 'v':
                token.value.charValue = '\v';
                break;
            case 'f':
                token.value.charValue = '\f';
                break;
            case 'r':
                token.value.charValue = '\r';
                break;
            case '"':
                token.value.charValue = '\"';
                break;
            case '\'':
                token.value.charValue = '\'';
                break;
            case '?':
                token.value.charValue = '\?';
                break;
            case '\\':
                token.value.charValue = '\\';
                break;
            default:
                break;
            }
        }
        else
            token.value.charValue = buffer[0];
        break;
    case INT:
        token.value.intValue = stoi(buffer, 0, base);
        break;
    case UINT:
        token.value.uintValue = stoi(buffer, 0, base);
        break;
    case LONG:
        token.value.longValue = stol(buffer, 0, base);
        break;
    case ULONG:
        token.value.ulongValue = stoul(buffer, 0, base);
        break;
    case LONGLONG:
        token.value.longlongValue = stoll(buffer, 0, base);
        break;
    case ULONGLONG:
        token.value.ulonglongValue = stoull(buffer, 0, base);
        break;
    case FLOAT:
        token.value.floatValue = stof(buffer);
        break;
    case DOUBLE:
        token.value.doubleValue = stod(buffer);
        break;
    case LONGDOUBLE:
        token.value.longdoubleValue = stold(buffer);
        break;
    case RELATION_OPERATOR:
    case ASSIGN_OPERATOR:
    case AGORITHM_OPERATOR:
    case LOGICAL_OPERATOR:
    case BITWISE_OPERATOR:
        token.value.uintValue = find_operator(buffer);
        break;
    }
    token_streams.push_back(token);
}

int is_keyword(const string &buffer)
{
    auto it = find(KeyWord.begin(), KeyWord.end(), buffer);
    if (it == KeyWord.end())
        return -1;
    else
        return it - KeyWord.begin();
}

unsigned int find(vector<string> &table, const string &str)
{
    auto it = find(table.begin(), table.end(), str);
    if (it == table.end())
    {
        table.push_back(str);
        return table.size() - 1;
    }
    else
        return it - table.begin();
}

unsigned int find_operator(const string &str)
{
    auto it = find(Operator.begin(), Operator.end(), str);
    return it - Operator.begin();
}

void next_char(ifstream &fin, int &char_num, char &c, string &buffer, int &col)
{
    buffer += c;
    c = fin.get();
    char_num++;
    col++;
}

void go_back(ifstream &fin, int &char_num, int &col)
{
    fin.unget();
    char_num--;
    col--;
}

void error(ErrorType error_type, int line, int col, const string buffer)
{
    if (col < buffer.size())
        col = buffer.size();
    cout << "Error: " << line + 1 << ":" << col - buffer.size() + 1 << ":";
    switch (error_type)
    {
    case UNKNOWN_SYMBOL:
        cout << "Unknown symbol: ";
        break;
    case ILLEGAL_NUMBERS:
        cout << "Illegal const numbers: ";
        break;
    case UNCLOSED_STRING:
        cout << "Unclosed const string: ";
        break;
    case UNCLOSED_CHAR:
        cout << "Unclosed const char: ";
        break;
    case ILLEGAL_ESCAPE:
        cout << "Illegal escape character: ";
        break;
    case UNCLOSED_ANNOTATION:
        cout << "Unclosed annotation: ";
        break;
    case EMPTY_CHAR:
        cout << "Empty const char: ";
        break;
    default:
        break;
    }
    cout << buffer << endl;
}