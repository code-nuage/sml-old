#include "lexer.h"

#include <errno.h>
#include <ctype.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include "../token/token.h"
#include "../error/error.h"
#include "../color/color.h"

sml_lexer lexer_new() {
    sml_lexer l = {
        .line = 1,
        .column = 0,
        .pos = 0,
        .count = 0,
        .tokens = NULL
    };

    return l;
}

char lexer_current(sml_lexer *l) {
    return l->content[l->pos];
}



void lexer_advance(sml_lexer *l) {
    if (lexer_current(l) == '\n') {
        l->line++;
        l->column = 1;
    } else l->column++;
    l->pos++;
}

void lexer_skip_whitespaces(sml_lexer *l) {
    while (lexer_current(l) == ' '
        || lexer_current(l) == '\t'
        || lexer_current(l) == '\r') lexer_advance(l);
}



sml_token lexer_number(sml_lexer *l, sml_token t) {
    float value = 0;
    int dot_encountered = 0;
    int digit = 0;

    while(isdigit(lexer_current(l))
        || lexer_current(l) == '.') {
        if (lexer_current(l) == '.') {
            if (dot_encountered) {
                sml_error(l->line, l->column, FG_RED "<Syntaxic error> " RESET "Number can have only one comma", 2);
                break;
            }

            dot_encountered = 1;
        } else if (dot_encountered) {
            digit++;

            value = value + (float)(lexer_current(l) - '0') / (float)pow(10, digit);
        } else {
            value = value * 10 + (float)(lexer_current(l) - '0');
        }
        lexer_advance(l);
    }

    t.type = T_NUMBER;
    t.number = value;

    return t;
}



sml_token lexer_next_token(sml_lexer *l) {
    lexer_skip_whitespaces(l);

    char c = lexer_current(l);

    int line = l->line; int column = l->column;

    sml_token t = {
        .line = line,
        .column = column
    };

    if (isdigit(c)) return lexer_number(l, t);

    switch(c) {
        case '(': lexer_advance(l); t.type = T_LPAREN; break;
        case ')': lexer_advance(l); t.type = T_RPAREN; break;

        case '+': lexer_advance(l); t.type = T_PLUS; break;
        case '-': lexer_advance(l); t.type = T_MINUS; break;
        case '*': lexer_advance(l); t.type = T_STAR; break;
        case '/': lexer_advance(l); t.type = T_SLASH; break;

        case ';': lexer_advance(l); t.type = T_SEMICOLON; break;

        case '\n': lexer_advance(l); t.type = T_EOL; break;
        case '\0': t.type = T_EOF; break;

        default:
            lexer_advance(l);
            t.type = T_UNKNOWN;
            sml_error(line, column, FG_RED "<Syntaxic error> " RESET "Unrecognized token", 2);
            break;
    }

    return t;
}

void lexer_lex(sml_lexer *l, char *content) {
    l->content = content;

    l->capacity = 16;
    l->tokens = malloc(sizeof(sml_token) * l->capacity);
    if (!l->tokens) { sml_error(0, 0, FG_RED "<Fatal error> Memory allocation failed", ENOMEM); };

    sml_token t = {};

    do {
        t = lexer_next_token(l);

        if (l->count >= l->capacity) {
            l->capacity *= 2;
            l->tokens = realloc(l->tokens, sizeof(sml_token) * l->capacity);
            if (!l->tokens) { sml_error(0, 0, FG_RED "<Fatal error> Memory allocation failed", ENOMEM); };
        }

        l->tokens[l->count] = t;
        l->count++;
    } while (t.type != T_EOF);
}
