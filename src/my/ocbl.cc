#include <stdio.h>
#include "parser.hh"

using namespace yy;

int main(const int argc, const char **argv)
{
    parser p;

    return p.parse();
}

void parser::error(const std::string& msg)
{
    fprintf(stderr, "%s\n", msg.c_str());
}
