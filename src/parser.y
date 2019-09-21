%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *s);
%}

%token NAMESPACE "namespace"
%token IMPORT "import"
%token CLASS "class"
%token INTERFACE "interface"
%token PRIVATE "private"
%token NAME
%token STRING

%%

module: namespace imports units;

namespace: /* %empty */ | NAMESPACE global_name ";";

imports: /* %empty */ | imports import;

import: IMPORT global_name ";";

global_name: NAME | global_name "." NAME;

units: unit | unit units;

unit: class | interface;

class: CLASS "{" "}";

interface: INTERFACE "{" "}";

%%

int main(const int argc, const char **argv)
{
    return yyparse();
}

void yyerror(const char *s)
{
    fprintf(stderr, "%s\n", s);
}
