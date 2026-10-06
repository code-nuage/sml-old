#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>

#include "../node/node.h"

typedef struct sml_ast_parser {
    sml_token *tokens;

    int pos;

    sml_ast_node **statements;
    size_t count;
    size_t capacity;
} sml_ast_parser;

sml_ast_parser parser_new();

sml_token parser_current(sml_ast_parser *p);
sml_token parser_peek(sml_ast_parser *p);
sml_token parser_expect(sml_ast_parser *p, sml_token_type type, char *message);

void parser_advance(sml_ast_parser *p);

sml_ast_node *parse_statement(sml_ast_parser *p);
sml_ast_node *parse_expression(sml_ast_parser *p);
sml_ast_node *parse_term(sml_ast_parser *p);
sml_ast_node *parse_unary(sml_ast_parser *p);
sml_ast_node *parse_factor(sml_ast_parser *p);

void parser_parse(sml_ast_parser *p, sml_token *tokens);

#endif // PARSER_H
