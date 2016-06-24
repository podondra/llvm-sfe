#include <cstdio>
#include <fstream>
#include <stdexcept>
#include "parser.h"
#include "ast.h"

int main(int argc, char **argv) {
    if (argc != 2)
        return 1;
    auto in = new std::ifstream{argv[1]};
    if (in == nullptr)
        return 1;
    auto parser = yyParser{in};
    ast::node *root = parser.yyparse();
    if (root != nullptr)
        root->dump(0);
    delete root;
    delete in;
    return 0;
}
