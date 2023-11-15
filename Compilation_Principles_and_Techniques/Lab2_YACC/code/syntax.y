%{
#include <stdio.h>
int yylex();
void yyerror(const char* s);
%}

%token NUM

%%
line:line expr'\n'  {printf("ACCEPT\n\n");}
    |               {} 
    |line'\n'       {}
    |error'\n'      {yyerror("Please retry!\n");yyerrok;}
;

expr:expr'+'term    {printf("E->E+T\n");}
    |expr'-'term    {printf("E->E-F\n");}
    |term           {printf("E->T\n");}
    ;

term:term'*'factor  {printf("T->T*F\n");}
    |term'/'factor  {printf("T->T/F\n");}
    |factor         {printf("T->F\n");}
    ;

factor:'('expr')'   {printf("F->(E)\n");}
    |NUM            {printf("F->num\n");}
    ;
%%

void yyerror(const char* s)
{
    printf("%s\n", s);
}

int main() {
    yyparse();
    return 0;
}
