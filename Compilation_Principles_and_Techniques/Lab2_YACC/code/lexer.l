%{
#include "y.tab.h"
%}

%%
[1-9][0-9]* {return NUM;}
[-+*/()]   	{return yytext[0];}
"\n"		{return '\n';}
[ \t]+      {}
.           {fprintf(stderr, "Invalid input: %s\n\n", yytext);}
%%

int yywrap() {
    return 1;
}
