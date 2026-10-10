#include "parser.h"

#include <stdlib.h>

#include "../error/error.h"

sml_ast_parser parser_new() {
    sml_ast_parser p = {
        .pos = 0,
    };

    return p;
}

sml_token parser_current(sml_ast_parser *p) {
    return p->tokens[p->pos];
}

sml_token parser_peek(sml_ast_parser *p) {
    return p->tokens[p->pos + 1];
}

sml_token parser_expect(sml_ast_parser *p, sml_token_type type, char *message) {
    sml_token t = parser_current(p);

    if (t.type != type) {
        sml_error(t.line, t.column, message, 2);
    }

    parser_advance(p);
    return t;
}



void parser_advance(sml_ast_parser *p) {
    if (parser_current(p).type != T_EOF) {
        p->pos++;
    }
}



sml_ast_node *parse_statement(sml_ast_parser *p) {
    sml_ast_node *expression = parse_expression(p);
    parser_expect(p, T_SEMICOLON, "Expected a semicolon terminating statement");
    return expression;
};

sml_ast_node *parse_expression(sml_ast_parser *p) {
    sml_ast_node *left = parse_term(p);

    while (parser_current(p).type == T_PLUS
        || parser_current(p).type == T_MINUS) {
        sml_token op = parser_current(p);
        parser_advance(p);

        sml_ast_node *right = parse_term(p);

        sml_ast_node *node = malloc(sizeof(sml_ast_node));
        node->type = N_BINARY;
        node->data.binary.left = left;
        node->data.binary.right = right;
        node->data.binary.op = op;

        left = node;
    }

    return left;
}

sml_ast_node *parse_term(sml_ast_parser *p) {
    sml_ast_node *left = parse_unary(p);

    while (parser_current(p).type == T_STAR
        || parser_current(p).type == T_SLASH) {
        sml_token op = parser_current(p);
        parser_advance(p);

        sml_ast_node *right = parse_unary(p);

        sml_ast_node *node = malloc(sizeof(sml_ast_node));
        node->type = N_BINARY;
        node->data.binary.left = left;
        node->data.binary.right = right;
        node->data.binary.op = op;

        left = node;
    }

    return left;
}

sml_ast_node *parse_unary(sml_ast_parser *p) {
    if (parser_current(p).type == T_PLUS
        || parser_current(p).type == T_MINUS) {
        sml_token op = parser_current(p);
        parser_advance(p);

        sml_ast_node *factor = parse_factor(p);

        sml_ast_node *node = malloc(sizeof(sml_ast_node));
        node->type = N_UNARY;
        node->data.unary.factor = factor;
        node->data.unary.op = op;

        return node;
    }

    return parse_factor(p);
}

sml_ast_node *parse_factor(sml_ast_parser *p) {
    sml_token current = parser_current(p);

    if (current.type == T_NUMBER) {
        sml_ast_node *node = malloc(sizeof(sml_ast_node));
        node->type = N_NUMBER;
        node->data.number.number = current;

        parser_advance(p);
        return node;
    }

    if (current.type == T_LPAREN) {
        parser_advance(p);
        sml_ast_node *expression = parse_expression(p);
        parser_expect(p, T_RPAREN, "Expected a closing parenthesis after expression");
        return expression;
    }

    sml_error(current.line, current.column, "Expected a number or an expression starting by an opening parenthesis", 2);

    return NULL;
}

void parser_parse(sml_ast_parser *p, sml_token *tokens) {
    p->tokens = tokens;

    p->capacity = 16;
    p->statements = malloc(sizeof(sml_ast_node) * p->capacity);

    sml_ast_node *s = {};

    do {
        s = parse_statement(p);

        if (p->count >= p->capacity) {
            p->capacity *= 2;
            p->statements = realloc(p->statements, sizeof(sml_ast_node) * p->capacity);
        }

        p->statements[p->count] = s;
        p->count++;
    } while (parser_current(p).type != T_EOF);
}
