#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    T_lte = 256, T_gte, T_eql, T_neq, T_intconst,
    T_identifier, T_void, T_int, T_while,
    T_if, T_else, T_return, T_comment
} TokenType;

static void print_token(int token) {
    static char* token_strs[] = {
        "T_lte", "T_gte", "T_eql", "T_neq", "T_intconst",
        "T_identifier", "T_void", "T_int", "T_while",
        "T_if", "T_else", "T_return", "T_comment"
    };
    if (token < 256) {
        printf("%-20c", token);
    } else {
        printf("%-20s", token_strs[token-256]);
    }
}

#endif
