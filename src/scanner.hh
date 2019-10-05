#ifndef __SCANNER_HH
#define __SCANNER_HH

#include "parser.hh"

#ifndef YY_DECL
#define YY_DECL parser::Parser::token_type parser::Scanner::yylex(  \
    parser::Parser::semantic_type* yylval,                          \
    parser::Parser::location_type*, parser::Driver& driver)
# endif

#ifndef __FLEX_LEXER_H
#define yyFlexLexer parserFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

namespace parser {
    class Scanner: public parserFlexLexer {
    public:
        Scanner();

        virtual ~Scanner();

        virtual Parser::token_type yylex(Parser::semantic_type* yylval,
            Parser::location_type* l, Driver& driver);

        void setDebug(bool b);
    };
}

#endif // __SCANNER_HH
