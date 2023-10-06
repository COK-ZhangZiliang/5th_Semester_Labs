%{
#include <stdio.h>
#include "token.h"
#include "error.h"
#include "output.h"

const int MAX_TOKEN_NUM = 1000; // 最大记号数

int char_num = 0;                          // 读入字符数
int line = 0;                              // 当前行数
int col = 0;                               // 当前列数
int token_num = 0;                         // 记号数
struct Token token_streams[MAX_TOKEN_NUM]; // 记号流

void add_token(TokenType type, char *value, int line, int col);
%}

ws        	[ \t]+	
macro		#[^\n]+
letter    	[A-Za-z]
digit     	[0-9]
keyword		(auto)|(break)|(case)|(char)|(const)|(continue)|(default)|(do)|(double)|(else)|(enum)|(extern)|(float)|(for)|(goto)|(if)|(inline)|(int)|(long)|(register)|(restrict)|(return)|(short)|(signed)|(sizeof)|(static)|(struct)|(switch)|(typedef)|(union)|(unsigned)|(void)|(volatile)|(while)|(_Bool)|(_Complex)|(_Complex)|(_Imaginary)
id        	({letter}|_)({letter}|{digit}|_)*
str			\"([^\"\n]|\\.)*\"
char 		\'([^\'\n]|\\.)+\'
octal_digit [0-7]
hex_digit 	[0-9a-fA-F]
int 		[1-9]{digit}*|0{octal_digit}*|0[xX]{hex_digit}+
uint		{int}(u|U)
long		{int}(l|L)
ulong		{int}(u|U)(l|L)
longlong	{int}(l|L)(l|L)
ulonglong	{int}(u|U)(l|L)(l|L)
double 		({digit}+\.{digit}*|\.{digit}+)([eE][\+\-]?{digit}+)?|{digit}+[eE][\+\-]?{digit}+
float		{double}(f|F)
longdouble  {double}(l|L)
relation	(\<)|(\>)|(\<=)|(\>=)|(==)|(!=)
assign 		(=)|(\+=)|(\-=)|(\*=)|(\/=)|(\%=)|(&=)|(\|=)|(\^=)|(\<\<=)|(\>\>=)
agorithm 	(\+)|(\-)|(\*)|(\/)|(\%)|(\+\+)|(\-\-)
logical		(&&)|(\|\|)|(!)
bitwise		(&)|(\|)|(\^)|(~)|(\<\<)|(\>\>)
singleline_annotation		(\/\/)[^\n]*
multiline_annotation		(\/\*)(.|\n)*(\*\/)
unclosed_string			\"[^\"\n]*
unclosed_char			\'[^\'\n]*
empty_char				\'\'
unclosed_annotation		(\/\*)(.*)
unknown_symbol 			[`@$]

%%
"\n"		{char_num += yyleng; line++; col = 0;}
{ws}		{char_num += yyleng; col += yyleng;}
{macro}		{}
{keyword}	{add_token(KEYWORD, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{id} 		{add_token(ID, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{str}		{add_token(STRING, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{char}		{add_token(CHAR, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{int} 		{add_token(INT, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{uint}		{add_token(UINT, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{long}		{add_token(LONG, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{ulong}		{add_token(ULONG, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{longlong}	{add_token(LONGLONG, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{ulonglong} {add_token(ULONGLONG, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{double}	{add_token(DOUBLE, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{float}		{add_token(FLOAT, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{longdouble} {add_token(LONGDOUBLE, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{relation}	{add_token(RELATION_OPERATOR, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{assign}	{add_token(ASSIGN_OPERATOR, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{agorithm}	{add_token(AGORITHM_OPERATOR, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{logical}	{add_token(LOGICAL_OPERATOR, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{bitwise}	{add_token(BITWISE_OPERATOR, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
{singleline_annotation}	{char_num += yyleng;}
{multiline_annotation}	{char_num += yyleng;}
{unclosed_string}		{error(UNCLOSED_STRING, line + 1, col + 1, yytext); char_num += yyleng; col += yyleng;}
{unclosed_char}			{error(UNCLOSED_CHAR, line + 1, col + 1, yytext); char_num += yyleng; col += yyleng;}
{empty_char}			{error(EMPTY_CHAR, line + 1, col + 1, yytext); char_num += yyleng; col += yyleng;}
{unclosed_annotation}	{error(UNCLOSED_ANNOTATION, line + 1, col + 1, yytext); char_num += yyleng; col += yyleng;}
{unknown_symbol} 		{error(UNKNOWN_SYMBOL, line + 1, col + 1, yytext); char_num += yyleng; col += yyleng;}
"?" 		{add_token(QUESTION_MARK, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
":"			{add_token(COLON, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
";"			{add_token(SEMICOLON, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
"["			{add_token(LEFT_SQUARE_BRACKET, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
"]" 		{add_token(RIGHT_SQUARE_BRACKET, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
"("			{add_token(LEFT_PARENTHESE, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
")"			{add_token(RIGHT_PARENTHESE, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
"{"			{add_token(LEFT_BRACE, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
"}"			{add_token(RIGHT_BRACE, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
"."			{add_token(DOT, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
","			{add_token(COMMA, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
"->"		{add_token(ARROW, yytext, line + 1, col + 1); char_num += yyleng; col += yyleng;}
%%

void add_token(TokenType type, char *value, int line, int col){
		token_streams[token_num].type = type;
		strcpy(token_streams[token_num].value, value);
		token_streams[token_num].line = line;
		token_streams[token_num].column = col;
		token_num++;
}

int main(){
	puts("Please input the source file name: ");
	FILE *file;
	char filename[100];
	gets(filename);
	char pre[] = "test/";
	strcat(pre, filename);
	file = fopen(pre, "r");
	if(file)
	  yyin = file;
	else
	{
		printf("Error opening file");
		exit(1);
	}
	yylex();
	puts("");
	output(token_streams, char_num, line, token_num);
	system("pause");
	return 0;
}

int yywrap(){
	return 1;
}