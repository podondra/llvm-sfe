#include <cstdio>
#include <FlexLexer.h>
#include "lexer.h"

int main() {
    auto lexer = yyFlexLexer{};
    auto lex_symbol = 0;
    while ((lex_symbol = lexer.yylex()) != LEX_EOI)
        if (lex_symbol == LEX_NUMB)
            printf("number: %s (%ld)\n",
                    lexer.YYText(),
                    yynumbval);
        else
            printf("lexical symbol: %s (%d)\n",
                    lexer.YYText(),
                    lex_symbol);
    return 0;
}
