//
// Created by Ethan on 6/10/2022.
//

#ifndef SPICELANG_MISC_H
#define SPICELANG_MISC_H

#endif //SPICELANG_MISC_H
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "lexerToken.h"
#include "ASTNode.h"

std::string file_to_string(const std::string& filepath);

void print_token_type(enum tokenType type);
void print_AST_tree(ASTNode* head);
void print_AST_tree_helper(ASTNode* node, int depth);