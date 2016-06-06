%{
#include <cstdio>
#include "lexer.h"
%}

    /* generate c++ */
%option c++
    /* do not generate yywrap function */
%option noyywrap
    /* specify filenames */
%option outfile="lexer.cc"
    /*
     * instructs flex to trade off larger tables in the generated scanner for
     * faster performance because the elements of the tables are better
     * aligned for memory access and computation
     */
%option align
    /*
     * specifies that the full scanner tables should be generated -
     * flex should not compress the tables by taking advantages of
     * similar transition functions for different states
     */
%option full
    /*
     * generate an 8-bit scanner, one which can recognize 8-bit
     * characters
     */
%option 8bit
    /* warn about certain things */
%option warn

IDENTIFIER [[:alpha:]][[:alnum:]]*

%%

"array" return KW_ARRAY;

"begin" return KW_BEGIN;

"const" return KW_CONST;

"do" return KW_DO;

"end" return KW_END;

"for" return KW_FOR;

"if" return KW_IF;

"integer" return KW_INTEGER;

"of" return KW_OF;

"program" return KW_PROGRAM;

"readln" return KW_READLN;

"then" return KW_THEN;

"to" return KW_TO;

"var" return KW_VAR;

"write" return KW_WRITE;

"writeln" return KW_WRITELN;

"+" return PLUS;

"-" return MINUS;

"*" return MULTIPLICATION;

"/" return DIVISION;

"mod" return MODULO;

":=" return ASSIGNMENT;

"=" return EQUALS_SIGN;

";" return SEMICOLON;

":" return COLON;

"." return PERIOD;

"[" return LEFT_BRACKET;

"]" return RIGHT_BRACKET;

"(" return LEFT_ROUND_BRACKET;

")" return RIGHT_ROUND_BRACKET;

([[:digit:]]{-}[0])[[:digit:]]* return DECIMAL;

"$"[[:xdigit:]]+ return HEXADECIMAL;

"&"[0-7]+ return OCTAL;

{IDENTIFIER} return IDENTIFIER;

[ \t\n]+ /* eat up whitespace */

. printf("Unrecognized character: %s\n", YYText());

<<EOF>> return EOI;
