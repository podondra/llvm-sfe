%{
#include <cstdio>
#include "lexer.h"
%}

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
