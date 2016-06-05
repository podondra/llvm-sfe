%{
#include <cstdlib>
#include <cstdio>
%}

    /* generate c++ */
%option c++
    /* do not generate yywrap function */
%option noyywrap
    /* specify filenames */
%option outfile="scanner.cc" header-file="scanner.h"
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

"array" printf("A keyword array: %s\n", YYText());

"begin" printf("A keyword begin: %s\n", YYText());

"const" printf("A keyword const: %s\n", YYText());

"do" printf("A keyword do: %s\n", YYText());

"end" printf("A keyword end: %s\n", YYText());

"for" printf("A keyword for: %s\n", YYText());

"if" printf("A keyword if: %s\n", YYText());

"integer" printf("A keyword integer: %s\n", YYText());

"of" printf("A keyword of: %s\n", YYText());

"program" printf("A keyword program: %s\n", YYText());

"readln" printf("A keyword readln: %s\n", YYText());

"then" printf("A keyword then: %s\n", YYText());

"to" printf("A keyword to: %s\n", YYText());

"var" printf("A keyword var: %s\n", YYText());

"write" printf("A keyword write: %s\n", YYText());

"writeln" printf("A keyword writeln: %s\n", YYText());

"+" printf("An operator plus: %s\n", YYText());

"-" printf("An operator minus: %s\n", YYText());

"*" printf("An operator multiplication: %s\n", YYText());

"/" printf("An operator division: %s\n", YYText());

"mod" printf("An operator: %s\n", YYText());

":=" printf("An assignment: %s\n", YYText());

"=" printf("An equals sign: %s\n", YYText());

";" printf("A semicolon: %s\n", YYText());

":" printf("A colon: %s\n", YYText());

"." printf("A period: %s\n", YYText());

"[" printf("A left bracket: %s\n", YYText());

"]" printf("A right bracket: %s\n", YYText());

"(" printf("A left round bracket: %s\n", YYText());

")" printf("A right round bracket: %s\n", YYText());

([[:digit:]]{-}[0])[[:digit:]]* {
    printf("A decimal integer: %s\n", YYText());
}

"$"[[:xdigit:]]+ printf("A hexadecimal integer: %s\n", YYText());

"&"[0-7]+ printf("A octal integer: %s\n", YYText());

{IDENTIFIER} printf("An identifier: %s\n", YYText());

    /* eat up whitespace */
[ \t\n]+

. printf("Unrecognized character: %s\n", YYText());

%%

int main(int argc, char** argv) {
    FlexLexer *lexer = new yyFlexLexer();
    while (lexer->yylex() != 0) {}
    return 0;
}
