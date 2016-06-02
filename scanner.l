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
     * specifies that the full scanner tables should be generated - flex
     * should not compress the tables by taking advantages of similar
     * transition functions for different states
     */
%option full
    /* generate an 8-bit scanner, one which can recognize 8-bit characters */
%option 8bit
    /* warn about certain things */
%option warn

DIGIT [0-9]
ID    [a-zA-Z][a-zA-Z0-9]*

%%

{DIGIT}+ {
    printf("An integer: %s (%d)\n", YYText(), atoi(YYText()));
}

if|then|begin|end|procedure|function {
    printf("A keyword: %s\n", YYText());
}

{ID} printf("An identifier: %s\n", YYText());

"+"|"-"|"*"|"/" {
    printf("An operator: %s\n", YYText());
}

    /* eat up whitespace */
[ \t\n]+

. printf("Unrecognized character: %s\n", YYText());

%%

int main(int argc, char** argv) {
    FlexLexer *lexer = new yyFlexLexer();
    while (lexer->yylex() != 0) {}
    return 0;
}
