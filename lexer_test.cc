#include <cstdio>
#include <FlexLexer.h>
#include "lexer.h" 

int main() {
    FlexLexer *lexer = new yyFlexLexer();
    int lexical_symbol;
    while ((lexical_symbol = lexer->yylex()) != LEX_EOI)
        printf("lexical symbol: %s (%d)\n",
                lexer->YYText(),
                lexical_symbol);
    return 0;
}
