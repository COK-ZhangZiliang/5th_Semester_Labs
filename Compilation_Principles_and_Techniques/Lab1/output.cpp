#include "output.h"

void output(const vector<Token> &token_streams, const vector<string> &id_table, const vector<string> &str_table)
{
    cout << "==========Token Streams:=========" << endl;
    for (int i = 0; i < token_streams.size(); i++)
    {
        for (int j = 0; j < 4 && i < token_streams.size(); j++, i++)
        {
            cout << "<" << token_streams[i].line << ":" << token_streams[i].column << ">";
            print(token_streams[i].type, token_streams[i].value, id_table, str_table);
            cout << "\t";
        }
        cout << endl;
    }
}

void print(const TokenType type, const ValueType value, const vector<string> &id_table, const vector<string> &str_table)
{
    switch (type)
    {
    case KEYWORD:
        cout << "<Keyword, " << KeyWord[value.uintValue] << ">";
        break;
    case ID:
        cout << "<Identifier, " << id_table[value.uintValue] << ">";
        break;
    case STRING:
        cout << "<String, " << str_table[value.uintValue] << ">";
        break;
    case CHAR:
        cout << "<Char, " << value.charValue << ">";
        break;
    case INT:
        cout << "<Int, " << value.intValue << ">";
        break;
    case UINT:
        cout << "<Unsigned Int, " << value.uintValue << ">";
        break;
    case LONG:
        cout << "<Long, " << value.longValue << ">";
        break;
    case ULONG:
        cout << "<Unsigned Long, " << value.ulongValue << ">";
        break;
    case LONGLONG:
        cout << "<Long Long, " << value.longlongValue << ">";
        break;
    case ULONGLONG:
        cout << "<Unsigned Long Long, " << value.ulonglongValue << ">";
        break;
    case FLOAT:
        cout << "<Float, " << value.floatValue << ">";
        break;
    case DOUBLE:
        cout << "<Double, " << value.doubleValue << ">";
        break;
    case LONGDOUBLE:
        cout << "<Long Double, " << value.longdoubleValue << ">";
        break;
    case RELATION_OPERATOR:
        cout << "<Relation Operator, " << Operator[value.uintValue] << ">";
        break;
    case ASSIGN_OPERATOR:
        cout << "<Assign Operator, " << Operator[value.uintValue] << ">";
        break;
    case AGORITHM_OPERATOR:
        cout << "<Agorithm Operator, " << Operator[value.uintValue] << ">";
        break;
    case LOGICAL_OPERATOR:
        cout << "<Logical Operator, " << Operator[value.uintValue] << ">";
        break;
    case BITWISE_OPERATOR:
        cout << "<Bitwise Operator, " << Operator[value.uintValue] << ">";
        break;
    case QUESTION_MARK:
        cout << "<Question Mark, ?>";
        break;
    case COLON:
        cout << "<Colon, :>";
        break;
    case SEMICOLON:
        cout << "<Semicolon, ;>";
        break;
    case LEFT_SQUARE_BRACKET:
        cout << "<Left Square Bracket, [>";
        break;
    case RIGHT_SQUARE_BRACKET:
        cout << "<Right Square Bracket, ]>";
        break;
    case LEFT_PARENTHESE:
        cout << "<Left Parenthese, (>";
        break;
    case RIGHT_PARENTHESE:
        cout << "<Right Parenthese, )>";
        break;
    case LEFT_BRACE:
        cout << "<Left Brace, {>";
        break;
    case RIGHT_BRACE:
        cout << "<Right Brace, }>";
        break;
    case DOT:
        cout << "<Dot, .>";
        break;
    case COMMA:
        cout << "<Comma, ,>";
        break;
    case ARROW:
        cout << "<Arrow, ->>";
        break;
    }
}