//
// Created by Ethan on 6/10/2022.
//

#ifndef SPICELANG_LEXER_H
#define SPICELANG_LEXER_H
#include <string>
#include "lexerToken.h"
class lexer {
    std::string input;
    int index;
    char curChar;
public:
    explicit lexer(std::string const &text_input);
    ~lexer() = default;

    // returns the next token
    lexerToken* get_next_token();
private:
    // moves forward one character
    void advance();

    // returns true if on the last character in the input
    bool completed();

    // skips all consecutive whitespace
    void skip_whitespace();

    // initialize token
    static lexerToken* create_token(enum tokenType t, std::string c);

    // collects the number in the next characters
    lexerToken* collect_numeric_type();

    // collects any ID
    lexerToken* collect_ID();

    // collects string until matching " char
    lexerToken* collect_string();
};


#endif //SPICELANG_LEXER_H
