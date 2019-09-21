#include <stdio.h>
#include "ocbl.hh"

int yyparse(void);

int main(const int argc, const char **argv)
{
    return yyparse();
}

void Ocbl::yyerror(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
}
