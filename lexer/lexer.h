#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

#include "../token/token.h"

typedef struct sml_lexer {
    char *content;

    int line;
    int column;

    int pos;

    sml_token *tokens;
    size_t count;
    size_t capacity;
} sml_lexer;

sml_lexer lexer_new();

char lexer_current(sml_lexer *l);
char lexer_peek(sml_lexer *l);
char lexer_peek_next(sml_lexer *l);

void lexer_advance(sml_lexer *l);

sml_token lexer_number(sml_lexer *l, sml_token t);

void lexer_skip_whitespaces(sml_lexer *l);
sml_token lexer_next_token(sml_lexer *l);
void lexer_lex(sml_lexer *l, char *content);

#endif // LEXER_H
