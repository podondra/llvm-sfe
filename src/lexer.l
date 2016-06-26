%{
#include <cstdio>
#include <cstdlib>
#include "lexer.h"

long int yynumbval;
%}

IDENTIFIER  [[:alpha:]_][[:alnum:]_]*
DECIMAL     (([[:digit:]]{-}[0])[[:digit:]]*|"0")
HEXADECIMAL "$"[[:xdigit:]]+
OCTAL       "&"[0-7]+

%%

"array"     return LEX_ARRAY;
"begin"     return LEX_BEGIN;
"break"     return LEX_BRK;
"const"     return LEX_CONST;
"dec"       return LEX_DEC;
"do"        return LEX_DO;
"downto"    return LEX_DOWNTO;
"else"      return LEX_ELSE;
"end"       return LEX_END;
"exit"      return LEX_EXIT;
"for"       return LEX_FOR;
"forward"   return LEX_FORW;
"function"  return LEX_FUNC;
"inc"       return LEX_INC;
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
"div"       return LEX_DIV;
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

({DECIMAL}|{HEXADECIMAL}|{OCTAL}) {
    if (YYText()[0] == '$')
        yynumbval = std::strtoul(YYText() + 1, nullptr, 16);
    else if (YYText()[0] == '&')
        yynumbval = std::strtoul(YYText() + 1, nullptr, 8);
    else
        yynumbval = std::strtoul(YYText(), nullptr, 10);
    return LEX_NUMB;
}

{IDENTIFIER} return LEX_IDENT;

[ \t\n]+ /* eat up whitespace */

<<EOF>> return LEX_EOI;
. printf("Unrecognized character: %s\n", YYText());
