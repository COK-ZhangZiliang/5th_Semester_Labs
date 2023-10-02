#include "output.h"

void output(const vector<Token> &token_streams, const vector<string> &id_table, const vector<string> &str_table, const int char_num, const int line)
{
    cout << "========Token Streams========" << endl;
    for (int i = 0; i < token_streams.size(); i++)
    {
        for (int j = 0; j < 3 && i < token_streams.size(); j++, i++)
        {
            cout << "<" << token_streams[i].line << ":" << token_streams[i].column << ">";
            print(token_streams[i].type, token_streams[i].value, id_table, str_table);
            cout << "\t";
        }
        cout << endl;
    }
    cout << endl;

    cout << "========Analysis Result========" << endl;
    cout << "Total lines: " << line << endl;
    cout << "Total characters: " << char_num << endl;
    cout << "Total tokens: " << token_streams.size() << endl;
    cout << "Total keywords: " << keyword_num << endl;
    cout << "Total identifiers: " << id_num << endl;
    cout << "Total const strings: " << str_num << endl;
    cout << "Total const characters: " << _char_num << endl;
    cout << "Total const integers: " << int_num << endl;
    cout << "Total const floats: " << float_num << endl;
    cout << "Total operators: " << operator_num << endl;
    cout << "Total others: " << other_num << endl;
}

void print(const TokenType type, const ValueType value, const vector<string> &id_table, const vector<string> &str_table)
{
    switch (type)
    {
    case KEYWORD:
        keyword_num++;
        cout << "<Keyword, " << KeyWord[value.uintValue] << ">";
        break;
    case ID:
        id_num++;
        cout << "<Identifier, " << id_table[value.uintValue] << ">";
        break;
    case STRING:
        str_num++;
        cout << "<String, " << str_table[value.uintValue] << ">";
        break;
    case CHAR:
        _char_num++;
        cout << "<Char, " << value.charValue << ">";
        break;
    case INT:
        int_num++;
        cout << "<Int, " << value.intValue << ">";
        break;
    case UINT:
        int_num++;
        cout << "<Unsigned Int, " << value.uintValue << ">";
        break;
    case LONG:
        int_num++;
        cout << "<Long, " << value.longValue << ">";
        break;
    case ULONG:
        int_num++;
        cout << "<Unsigned Long, " << value.ulongValue << ">";
        break;
    case LONGLONG:
        int_num++;
        cout << "<Long Long, " << value.longlongValue << ">";
        break;
    case ULONGLONG:
        int_num++;
        cout << "<Unsigned Long Long, " << value.ulonglongValue << ">";
        break;
    case FLOAT:
        float_num++;
        cout << "<Float, " << value.floatValue << ">";
        break;
    case DOUBLE:
        float_num++;
        cout << "<Double, " << value.doubleValue << ">";
        break;
    case LONGDOUBLE:
        float_num++;
        cout << "<Long Double, " << value.longdoubleValue << ">";
        break;
    case RELATION_OPERATOR:
        operator_num++;
        cout << "<Relation Operator, " << Operator[value.uintValue] << ">";
        break;
    case ASSIGN_OPERATOR:
        operator_num++;
        cout << "<Assign Operator, " << Operator[value.uintValue] << ">";
        break;
    case AGORITHM_OPERATOR:
        operator_num++;
        cout << "<Agorithm Operator, " << Operator[value.uintValue] << ">";
        break;
    case LOGICAL_OPERATOR:
        operator_num++;
        cout << "<Logical Operator, " << Operator[value.uintValue] << ">";
        break;
    case BITWISE_OPERATOR:
        operator_num++;
        cout << "<Bitwise Operator, " << Operator[value.uintValue] << ">";
        break;
    case QUESTION_MARK:
        other_num++;
        cout << "<Question Mark, ?>";
        break;
    case COLON:
        other_num++;
        cout << "<Colon, :>";
        break;
    case SEMICOLON:
        other_num++;
        cout << "<Semicolon, ;>";
        break;
    case LEFT_SQUARE_BRACKET:
        other_num++;
        cout << "<Left Square Bracket, [>";
        break;
    case RIGHT_SQUARE_BRACKET:
        other_num++;
        cout << "<Right Square Bracket, ]>";
        break;
    case LEFT_PARENTHESE:
        other_num++;
        cout << "<Left Parenthese, (>";
        break;
    case RIGHT_PARENTHESE:
        other_num++;
        cout << "<Right Parenthese, )>";
        break;
    case LEFT_BRACE:
        other_num++;
        cout << "<Left Brace, {>";
        break;
    case RIGHT_BRACE:
        other_num++;
        cout << "<Right Brace, }>";
        break;
    case DOT:
        other_num++;
        cout << "<Dot, .>";
        break;
    case COMMA:
        other_num++;
        cout << "<Comma, ,>";
        break;
    case ARROW:
        other_num++;
        cout << "<Arrow, ->>";
        break;
    }
}