%{
#include <cstdio>
#include "lexer.h"
%}

IDENTIFIER  [[:alpha:]_][[:alnum:]_]*
DECIMAL     (([[:digit:]]{-}[0])[[:digit:]]*|"0")
HEXADECIMAL "$"[[:xdigit:]]+
OCTAL       "&"[0-7]+

%%

"array"     return LEX_ARRAY;
"begin"     return LEX_BEGIN;
"const"     return LEX_CONST;
"do"        return LEX_DO;
"downto"    return LEX_DO;
"else"      return LEX_ELSE;
"end"       return LEX_END;
"exit"      return LEX_EXIT;
"for"       return LEX_FOR;
"forward"   return LEX_FORW;
"funciton"  return LEX_FORW;
"if"        return LEX_IF;
"integer"   return LEX_INT;
"of"        return LEX_OF;
"procedure" return LEX_PROC;
"program"   return LEX_PROGRAM;
"readln"    return LEX_READLN;
"then"      return LEX_THEN;
"to"        return LEX_TO;
"var"       return LEX_VAR;
"while"     return LEX_WHILE;
"write"     return LEX_WRITE;
"writeln"   return LEX_WRITELN;
"/"         return LEX_DIV;
"**"        return LEX_EXP;
"-"         return LEX_MINUS;
"mod"       return LEX_MOD;
"*"         return LEX_MUL;
"+"         return LEX_PLUS;
":="        return LEX_ASSIGN;
"="         return LEX_EQ;
"<>"        return LEX_NE;
"<"         return LEX_LT;
">"         return LEX_GT;
"<="        return LEX_LE;
">="        return LEX_GE;
"and"       return LEX_AND;
"not"       return LEX_NOT;
"or"        return LEX_OR;
":"         return LEX_COLON;
","         return LEX_COMMA;
"."         return LEX_DOT;
".."        return LEX_DOTDOT;
"["         return LEX_LBRAC;
"]"         return LEX_RBRAC;
"("         return LEX_LRBRAC;
")"         return LEX_RRBRAC;
";"         return LEX_SEMICOLON;

({DECIMAL}|{HEXADECIMAL}|{OCTAL}) return LEX_NUMB;

{IDENTIFIER} return LEX_IDENT;

[ \t\n]+ /* eat up whitespace */

<<EOF>> return LEX_EOI;
. printf("Unrecognized character: %s\n", YYText());
