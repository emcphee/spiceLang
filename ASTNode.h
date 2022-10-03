//
// Created by Ethan on 6/10/2022.
//

#ifndef SPICELANG_ASTNODE_H
#define SPICELANG_ASTNODE_H
#include <string>
#include <vector>

enum AST_type{
    ASSIGNMENT, // assignment

    VAR_REFERENCE, // variable reference

    FUNC_CALL, // function call

    STRING_LITERAL, // string
    NUMERIC_LITERAL, // numeric type
    BOOL_LITERAL, // boolean

    LIST_LITERAL, // list of anything

    USER_DEFINED_FUNCTION, // user-defined functions

    RETURN_STATEMENT,

    WHILE_STATEMENT,
    FOR_STATEMENT,

    CODE_BLOCK, // basically an array of statements. Statement sequence

    // operations
    BINARY_OP, // + - * / ** ops
    COMPARE_OP, // > < == !=
    LOGIC_OP, // || and &&

    NO_OP // NO OPERATION
};


class ASTNode {
public:
    AST_type type;

    /* ASSIGNMENT */
    bool variable_initialized{};
    std::string variable_definition_variable_name;
    ASTNode* variable_contents{};

    /* VAR_REFERENCE */
    std::string variable_name;

    /* FUNC_CALL */
    std::string function_call_name;
    std::vector<ASTNode*> function_args;

    /* STRING_LITERAL */
    std::string string_value;

    /* DOUBLE_LITERAL */
    double double_value{};

    /* BOOL_LITERAL */
    bool bool_value{};

    /* list */
    std::vector<ASTNode*> list_vect;

    /* USER_DEFINED_FUNCTION */
    std::string function_name;
    ASTNode* function_root{};

    /* RETURN */
    ASTNode* return_expr_root{};

    /* WHILE / FOR*/
    ASTNode* loop_block{};
    ASTNode* condition_root{};

    /* CODE_BLOCK */
    std::vector<ASTNode*> compound_value;

public:
    explicit ASTNode(AST_type _type);


    std::string operation;
/* BINARY_OP / COMPARE_OP / LOGIC_OP */
    ASTNode* leftOp{};
    ASTNode* rightOp{};
};


#endif //SPICELANG_ASTNODE_H
