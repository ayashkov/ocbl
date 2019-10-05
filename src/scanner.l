%{
#include "parser.hh"
#include "scanner.hh"
#include "driver.hh"

/*  Defines some macros to update locations */

#define STEP()                                      \
    do {                                            \
        driver.location_->step();                   \
    } while (0)

#define COL(col)				                    \
    driver.location_->columns(col)

#define LINE(line)				                    \
    do {						                    \
        driver.location_->lines(line);		        \
    } while (0)

#define YY_USER_ACTION				                \
    COL(yyleng);

typedef parser::Parser::token token;
typedef parser::Parser::token_type token_type;

#define yyterminate() return token::TOK_EOF
%}

%option debug
%option c++
%option noyywrap
%option never-interactive
%option yylineno
%option nounput
%option batch
%option prefix="parser"

/* Abbreviations.  */

blank       [ \t]+
eol         [\n\r]+

%%

 /* The rules.  */
%{
    STEP();
%}


{blank}     STEP();

{eol}       LINE(yyleng);
.           {
                std::cerr << *driver.location_ << "Unexpected token: "
                    << *yytext << std::endl;
                driver.error_ = (driver.error_ == 127 ? 127 : driver.error_ + 1);
                STEP();
            }

%%

namespace parser {
    Scanner::Scanner(): parserFlexLexer()
    {
    }

    Scanner::~Scanner()
    {
    }

    void Scanner::setDebug(bool b)
    {
        yy_flex_debug = b;
    }
}

#ifdef yylex
#undef yylex
#endif

int parserFlexLexer::yylex()
{
    std::cerr << "call parserFlexLexer::yylex()!" << std::endl;

    return 0;
}
