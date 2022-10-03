//
// Created by Ethan on 6/10/2022.
//

#include "lexer.h"
#include <iostream>
#include <utility>
#include <algorithm>

lexer::lexer(const std::string &text_input) {
    if(text_input.length() == 0){
        std::cout << "Input file has a length of zero." << std::endl;
        exit(1);
    }
    index = 0;
    input = text_input;
    curChar = input[0];
}

lexerToken *lexer::get_next_token() {
    while(!completed()){
        skip_whitespace();

        if(std::isdigit(curChar) || curChar == '.')
            return collect_numeric_type();

        if(curChar == '"')
            return collect_string();

        if(std::isalnum(curChar))
            return collect_ID();

        tokenType t;
        switch(curChar){
            case '(': t = TOKEN_LPAREN; break;
            case ')': t = TOKEN_RPAREN; break;
            case '{': t = TOKEN_LCURLY; break;
            case '}': t = TOKEN_RCURLY; break;
            case ';': t = TOKEN_SEMI_COLON; break;
            case '=': t = TOKEN_EQUALS; break;
            case '>': t = TOKEN_GREATER_THAN; break;
            case '<': t = TOKEN_LESS_THAN; break;
            case '+': t = TOKEN_PLUS; break;
            case '-': t = TOKEN_MINUS; break;
            case '*': t = TOKEN_DASH; break;
            case '/': t = TOKEN_F_SLASH; break;
            case '\\': t = TOKEN_B_SLASH; break;
            case ',': t = TOKEN_COMMA; break;
            case '|': t = TOKEN_BAR; break;
            case '&': t = TOKEN_AMPER; break;
            case '!': t = TOKEN_EXCLAMATION; break;
            default:
                std::cout << "Error: unexpected character. Could not tokenize input" << std::endl;
                exit(1);
        }
        std::string s;
        s.push_back(curChar);
        advance();
        auto *token = create_token(t, s);
        return token;
    }
    auto* token = create_token(TOKEN_EOF, "\0");
    return token;
}

void lexer::advance() {
    if(!completed()){
        index++;
        curChar = input[index];
    }
}

bool lexer::completed() {
    if(index >= input.length()){
        return true;
    }else{
        return false;
    }
}

void lexer::skip_whitespace() {
    while( isspace(curChar))
        advance();
}

lexerToken *lexer::create_token(enum tokenType t, std::string c) {
    auto* ptr = new lexerToken;
    ptr->type = t;
    ptr->contents = std::move(c);
    return ptr;
}

lexerToken *lexer::collect_numeric_type() {
    std::string s;
    while(std::isdigit(curChar) || curChar == '.'){
        s.push_back(curChar);
        advance();
    }
    if(std::count(s.begin(), s.end(), '.') > 1){
        std::cout << "Error: Could not parse double, contains multiple '.' characters." << std::endl;
        exit(1);
    }
    auto *token = create_token(TOKEN_NUMERIC, s);
    return token;
}

lexerToken *lexer::collect_ID() {
    std::string s;
    while(std::isalnum(curChar)){
        s.push_back(curChar);
        advance();
    }
    auto *token = create_token(TOKEN_ID, s);
    return token;
}

lexerToken *lexer::collect_string() {
    std::string s;
    // gets rid of left "
    advance();
    char prev = 0;
    while(curChar != '"' && prev != 92){
        s.push_back(curChar);
        prev = curChar;
        advance();
    }
    advance();
    auto *token = create_token(TOKEN_STRING, s);
    return token;
}
