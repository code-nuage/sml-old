#ifndef TOKEN_H
#define TOKEN_H

typedef enum sml_token_type {
    T_NUMBER,

    T_LPAREN,
    T_RPAREN,

    T_PLUS,
    T_MINUS,
    T_STAR,
    T_SLASH,

    T_SEMICOLON,

    T_EOF,

    T_UNKNOWN,
} sml_token_type;

typedef struct sml_token {
    sml_token_type type;

    int line;
    int column;

    union {
        float number;
    };
} sml_token;

char *token_display(sml_token token);
void token_print(sml_token token);

#endif // TOKEN_H
