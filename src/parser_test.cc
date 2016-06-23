#include <stdexcept>
#include <cstdio>
#include "parser.h"
#include "ast.h"

int main() {
    auto parser = yyParser{};
    ast::node *root = nullptr;
    try {
        root = parser.yyparse();
    } catch (const std::logic_error& e) {
        printf("%s\n", e.what());
    }
    root->dump(0);
    delete root;
    return 0;
}
