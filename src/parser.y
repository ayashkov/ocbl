%{
#include "parser.hh"
#include "scanner.hh"

#define yylex driver.scanner->yylex
%}

%code requires {
    #include <iostream>
    #include "driver.hh"
    #include "location.hh"
    #include "position.hh"
}

%code provides {
    namespace parser {
        // Forward declaration of the Driver class
        class Driver;

        inline void yyerror(const char* msg) {
            std::cerr << msg << std::endl;
        }
    }
}

%require "2.4"
%language "C++"
%locations
%defines
%debug
%define api.namespace {parser}
%define api.parser.class {Parser}
%define parse.error verbose
%parse-param {Driver &driver}
%lex-param {Driver &driver}
%union {
 /* YYLTYPE */
}

/* Tokens */
%token TOK_EOF 0
/* Entry point of grammar */
%start start

%%

start:
     /* empty */
;

%%

namespace parser
{
    void Parser::error(const location&, const std::string& m)
    {
        std::cerr << *driver.location_ << ": " << m << std::endl;
        driver.error_ = (driver.error_ == 127 ? 127 : driver.error_ + 1);
    }
}
