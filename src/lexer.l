%{
#include "parser.hh"

int line = 1;
int col = 1;
%}

%option noyywrap

%%

\n          {
                ++line;
                col = 1;
            }
.           ++col;
%%
