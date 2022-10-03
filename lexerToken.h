//
// Created by Ethan on 6/10/2022.
//

#ifndef SPICELANG_LEXERTOKEN_H
#define SPICELANG_LEXERTOKEN_H
#include <string>

enum tokenType{
    TOKEN_ID,           // string not in quotes
    TOKEN_STRING,       // any string in quotes “string”
    TOKEN_NUMERIC,      // any numeric type

    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_LCURLY,       // {
    TOKEN_RCURLY,       // }
    TOKEN_SEMI_COLON,   // ;
    TOKEN_EQUALS,       // =
    TOKEN_EOF,          // \0
    TOKEN_GREATER_THAN, // >
    TOKEN_LESS_THAN,    // <
    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_DASH,         // *
    TOKEN_F_SLASH,      // /
    TOKEN_B_SLASH,      // "\"
    TOKEN_COMMA,        // ,
    TOKEN_BAR,          // |
    TOKEN_AMPER,        // &
    TOKEN_EXCLAMATION // !
};

typedef struct lexerToken {
    enum tokenType type;
    std::string contents;
} lexerToken;

#endif //SPICELANG_LEXERTOKEN_H
