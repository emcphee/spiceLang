#include <iostream>

#include "lexer.h"
#include "misc.h"
#include <vector>
#include "parser.h"
int main() {

    std::string filepath = "../examples/sample.spice";
    std::string input = file_to_string(filepath);

    auto *lex = new lexer(input);

    auto p = new parser(lex);
    print_AST_tree(p->parse_expression());
}
