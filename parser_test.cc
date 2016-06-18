#include "parser.h"

int main() {
    auto parser = yyParser{};
    parser.yyparse();
    return 0;
}
